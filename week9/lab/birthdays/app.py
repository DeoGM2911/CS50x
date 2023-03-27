from cs50 import SQL
from flask import Flask, render_template, request
from datetime import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    status = "wait"
    people = db.execute("SELECT * FROM birthdays")
    if request.method == "POST":
        
        status = True
        name = request.form["name"]
        month = request.form["month"]
        day = request.form["day"]
        
        # Validate the entries
        try:
            if not name or not datetime(2000, int(month), int(day)):
                raise ValueError
        except ValueError:
            status = False
            return render_template("index.html", people=people, status=status)

        # Add user's birthday
        try:
            added = db.execute("INSERT INTO birthdays(name, month, day) VALUES(?, ?, ?)", name, month, day)
        except ValueError:
            status = False
        except RuntimeError:
            status = False
            
        if not added:
            status = False
            return render_template("index.html", people=people, status=status)

        return render_template("index.html", people=people, status=status)

    else:
        status = "wait"
        return render_template("index.html", people=people, status=status)
