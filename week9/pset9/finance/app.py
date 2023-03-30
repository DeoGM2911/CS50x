import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    # Avoid cache the response
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_data = []

    # Grab the user's data from the database
    cash = db.execute("SELECT cash FROM users WHERE users.id = ?", session["user_id"])[0]["cash"]
    companies = db.execute("""SELECT companies.symbol, stocks.shares
                            FROM stocks JOIN companies
                            ON companies.id = stocks.company_id
                            WHERE stocks.user_id = ?""", session["user_id"])

    # Compute user's stocks data
    stock_tot_val = 0
    for com in companies:
        com_data = lookup(com["symbol"])
        stock_val = int(com["shares"]) * com_data["price"]
        stock_tot_val += stock_val
        user_data.append({
            "symbol": com_data["symbol"],
            "name": com_data["name"],
            "shares": com["shares"],
            "price": com_data["price"],
            "stock_val": stock_val})

    return render_template("index.html", cash=cash, total=cash + stock_tot_val, user_data=user_data)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("You must enter shares as a positive integer!")

        if symbol is None:
            return apology("You must enter a company's qoute!")

        elif (data := lookup(symbol)) is None:
            return apology(f"Unable to find {symbol}'s data!")

        if shares <= 0:
            return apology("You must enter shares as a positive integer!")

        # Check and change the cash the user has
        db.execute("INSERT OR IGNORE INTO companies(name, symbol) VALUES(?, ?);",
                   data["name"], data["symbol"])
        cash = float(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])
        if (remain := (cash - data["price"] * shares)) < 0:
            return apology("You don't have enough money to buy such amount of shares!")

        else:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", remain, session["user_id"])
            day = datetime.now()
            company_id = db.execute("SELECT id FROM companies WHERE symbol = ?;", data["symbol"])[0]["id"]

            # Add data to stocks table
            try:
                exist = db.execute("SELECT shares FROM stocks WHERE user_id = ? AND company_id = ?",
                                   session["user_id"], company_id)[0]["shares"]
            except IndexError:
                exist = 0
                db.execute("INSERT INTO stocks(user_id, shares, company_id) VALUES(?, ?, ?);",
                           session["user_id"], exist, company_id)
            db.execute("UPDATE stocks SET shares = ? WHERE user_id = ? AND company_id = ?",
                       shares + exist, session["user_id"], company_id)

            # Add data about the buy action
            db.execute("INSERT OR IGNORE INTO purchases(date, type, user_id, company_id, shares, price) VALUES (?, ?, ?, ?, ?, ?)",
                       day.strftime("%m/%d/%Y, %H:%M:%S"), "buy", session["user_id"], company_id, shares, shares * data["price"])

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    his = []
    # Select all the transaction the user use
    trans_data = db.execute("""SELECT date, type, company_id, shares, price
               FROM purchases WHERE purchases.user_id = ?""", session["user_id"])

    for transac in trans_data:
        if transac["shares"] == "0":
            continue
        company = db.execute("SELECT symbol, name FROM companies WHERE id = ?", transac["company_id"])[0]
        his.append({
            "time": transac["date"],
            "type": transac["type"].title(),
            "symbol": company["symbol"],
            "name": company["name"],
            "shares": transac["shares"],
            "price": transac["price"]})

    return render_template("history.html", history=his)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        # Validate user's input
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("You must enter a company's qoute!")

        else:
            if (data := lookup(symbol)) is None:
                return apology(f"Unable to find {symbol}'s data!")

            else:
                # Add data about the company
                db.execute("INSERT OR IGNORE INTO companies(name, symbol) VALUES(?, ?);",
                           data["name"], data["symbol"])
                return render_template("quoted.html",
                                       name=data["name"],
                                       price=data["price"],
                                       symbol=data["symbol"])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        if not username:
            return apology("You must enter your username!")

        elif not password:
            return apology("You must enter your password!")

        elif request.form.get("confirmation") != password:
            return apology("Your passwords must match!")

        if len(db.execute("SELECT username FROM users WHERE username = ?", username)) == 1:
            return apology("The username already exist!")

        db.execute("INSERT INTO users(username, hash) VALUES(?, ?)", username, generate_password_hash(password))
        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if (shares := int(request.form.get("shares"))) < 0:
            return apology("The number of shares you sell must be a positive integer!")

        # Validate user's input
        if (com_data := lookup(symbol)) is None:
            return apology("There's no such symbol!")

        company = db.execute("SELECT id FROM companies WHERE symbol = ?", symbol)
        current_shares = db.execute("SELECT shares FROM stocks WHERE user_id = ? AND company_id = ?",
                                    session["user_id"], company[0]["id"])
        if len(company) < 1 or len(current_shares) < 1:
            return apology("You don't have any shares from this company!")

        current = int(current_shares[0]["shares"])

        if (remain := current - shares) < 0:
            return apology("You don't have enough shares to sell!")

        price = com_data["price"]
        day = datetime.now()
        db.execute("INSERT OR IGNORE INTO purchases(date, type, user_id, company_id, shares, price) VALUES (?, ?, ?, ?, ?, ?)",
                   day.strftime("%m/%d/%Y, %H:%M:%S"), "sell",
                   session["user_id"], company[0]["id"], shares, shares * price)

        cash = float(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + shares * price, session["user_id"])

        db.execute("UPDATE stocks SET shares = ? WHERE user_id = ? AND company_id = ?",
                   remain, session["user_id"], company[0]["id"])

        return redirect("/")

    else:
        companies = db.execute("""SELECT companies.symbol FROM companies JOIN stocks
                               ON companies.id = stocks.company_id WHERE stocks.user_id = ?""", session["user_id"])
        return render_template("sell.html", companies=companies)


@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    if request.method == "POST":
        old = request.form.get("old")
        new = request.form.get("new")
        re_new = request.form.get("confirm")

        password = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])[0]["hash"]

        if not check_password_hash(password, old):
            return apology("Incorrect old password!")

        if not new:
            return apology("You must enter a new password!")

        if new != re_new:
            return apology("Retype password isn't matched!")

        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(new), session["user_id"])

        return redirect("/")
    else:
        return render_template("change.html")