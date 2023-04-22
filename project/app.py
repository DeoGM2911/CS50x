import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from werkzeug.utils import secure_filename
from validator_collection import is_email
from datetime import datetime, timedelta

from funcs import login_required, apology, allowed_file, roles

# Configure application
app = Flask(__name__)

# Store images here
UPLOAD_FOLDER = "static/images"

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
app.config["UPLOAD_FOLDER"] = UPLOAD_FOLDER
Session(app)

# Connect to SQLite database and create a dbsor for fetching data
db = SQL("sqlite:///admin.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    # Avoid cache the response
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Display info. about the user and the UI, allow the change of the profile image"""
    if request.method == "POST":
        if 'imgprof' not in request.files:
            flash("No image found!")
            return apology("No image found1!")
        
        image = request.files["imgprof"]
        
        if image.filename == '':
            flash('No selected file')
            return apology("No image found2!")
        
        if image and allowed_file(image.filename):
            
            if len(db.execute("SELECT worker_id FROM imgs WHERE worker_id = ?", session["user_id"])) == 0:
                db.execute("INSERT INTO imgs(worker_id, src) VALUES(?, ?)", session["user_id"], "../static/images/default.png")
            
            # Ensure unique file name
            email = db.execute("SELECT email FROM workers WHERE id = ?", session["user_id"])[0]["email"].split("@")[0]
            name_of_file = secure_filename(f"{email}.png")
            
            # Save the image
            image.save(os.path.join(app.config["UPLOAD_FOLDER"], name_of_file))
        
            db.execute("UPDATE imgs SET src = ? WHERE worker_id = ?", f"../static/images/{name_of_file}", session["user_id"])
            return redirect("/")
            
        else:
            return apology("Wrong format or an error has happened! Please report the the devs!")
        
    else:
        # Get data in the database
        data = db.execute("""SELECT workers.name as worker, workers.email, workers.birth, teams.name, workers.role FROM 
                            workers JOIN teams ON workers.team_id = teams.id WHERE
                            workers.id = ?""", session["user_id"])[0]
        
        if len(fetch := db.execute("SELECT src FROM imgs WHERE worker_id = ?", session["user_id"])) == 0:
            img = "../static/images/default.png"
        else:
            img = fetch[0]["src"]
        
        name, birth, email, team, role, birthday = data["worker"], data["birth"], data["email"], data["name"], data["role"], False
        
        if birth == datetime.now().strftime("%d/%m/%Y"):
            birthday = True
        
        return render_template("index.html", name=name, birth=birth, team=team, role=role, email=email, img=img, birthday=birthday)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Allow users to log in and create cookie and check in at work"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("email"):
            return apology("must provide email", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        fetch = db.execute("SELECT id, role, pwd_hash FROM workers WHERE email = ?", request.form.get("email"))
        if len(fetch) != 1:
            return apology("Incorrect email or password!", 403)
        else:
            user_id, role, pwd_hash = fetch[0]["id"], fetch[0]["role"], fetch[0]["pwd_hash"]
        # Ensure username exists and password is correct
        if not check_password_hash(pwd_hash, request.form.get("password")):
            return apology("Invalid email or password!", 403)

        # Remember which user has logged in
        session["user_id"] = user_id
        session["role"] = role

        # Redirect user to home page
        return redirect("/time_keep")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout", methods=["GET", "POST"])
def logout():
    """Allow users to log out and clear cookie and check out at work"""
    # Clear session and redirect to index
    session.clear()
    return redirect("/")


# ! Only for Admin role
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        name = request.form.get("name").strip()
        email = request.form.get("email").strip()
        password = request.form.get("password")
        role = request.form.get("role").strip().title()
        year = request.form.get("year")
        month = request.form.get("month")
        day = request.form.get("day")
        team = request.form.get("team")
        
        if not is_email(email):
            return apology("You must enter your email!")

        elif not name:
            return apology("You must enter your name!")
        
        elif not password:
            return apology("You must enter your password!")

        elif request.form.get("confirmation") != password:
            return apology("Your passwords must match!")
        
        elif not role:
            return apology("you must enter a role!")
        
        elif role.title() not in roles:
            return apology("Not an appropriate role!")
        
        teams = [name["name"] for name in db.execute("SELECT name FROM teams")]
        
        if team.title() not in teams:
            return apology("Not a valid team!")

        try:
            y, m, d = map(int, [year, month, day])
            day = datetime(y, m, d)
        except ValueError:
            return apology("Inappropriate birthday!")
        
        fetch = db.execute("SELECT id, mem_count from teams WHERE name = ?", team.title())[0]
        team_id, curr = fetch["id"], fetch["mem_count"]
        
        db.execute("UPDATE teams SET mem_count = ? WHERE id = ?", int(curr) + 1, team_id)
        
        db.execute("INSERT INTO workers(name, email, role, pwd_hash, team_id, birth) VALUES(?, ?, ?, ?, ?, ?)",
                    name, email, role.title(), generate_password_hash(password), team_id, day.strftime("%d/%m/%Y"))
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/change", methods=["GET", "POST"])
def change():
    """Allow users to change password"""
    if request.method == "POST":
        old = request.form.get("old")
        new = request.form.get("new")
        re_new = request.form.get("confirm")

        password = db.execute("SELECT pwd_hash FROM workers WHERE id = ?", session["user_id"])[0]["pwd_hash"]

        if not check_password_hash(password, old):
            return apology("Incorrect password!")

        if not new:
            return apology("You must enter a new password!")

        if new != re_new:
            return apology("Retype password isn't matched!")

        db.execute("UPDATE workers SET pwd_hash = ? WHERE id = ?", generate_password_hash(new), session["user_id"])

        return redirect("/")
    else:
        return render_template("change.html")


@app.route("/create_team", methods=["GET", "POST"])
@login_required
def create_team():
    """Allow administrators to create teams or add member"""
    if request.method == "POST":
        team_name = request.form.get("team_name").title()
        team_leader = request.form.get("team_leader")
        email = request.form.get("team_leader_email")
        confirm = request.form.get("confirm")

        
        # Validate the team name
        if confirm is None or confirm == "no":
            return apology("Action canceled!")
        
        if not team_name.startswith("Tech") and not team_name.startswith("Hr") and not team_name.startswith("Executives"):
            return apology("Not a valid team name (either starts with Tech, HR, or Executives)!")
        
        if len(db.execute("SELECT id FROM teams WHERE name = ?", team_name) != 0):
            return apology("A team with that name already exists!")
        
        # Check if the leader is actually in the company
        if len(fetch := db.execute("SELECT id, team_id FROM workers WHERE name = ? AND email = ?", team_leader, email)) == 0:
            return apology("No such person exists in the company!")
        
        worker_id, cur_team_id = fetch[0]["id"], fetch[0]["team_id"]
        
        current = db.execute("SELECT mem_count FROM teams WHERE id = ?", cur_team_id)[0]["mem_count"]
        
        # Update the database
        db.execute("UPDATE teams SET mem_count = ? WHERE id = ?", int(current) - 1, cur_team_id)
        
        db.execute("INSERT INTO teams(name, mem_count) VALUES(?, 1)", team_name)
        
        team_id = db.execute("SELECT id FROM teams WHERE name = ? AND mem_count = 1", team_name)[0]["id"]
        
        if team_name.startswith("Tech"):
            role = "Tech Lead"
        elif team_name.startswith("Hr"):
            role = "Hr Manager"
        else:
            role = "Director"
        
        db.execute("""UPDATE workers
                   SET team_id = ?, role = ?
                   WHERE id = ?""", team_id, role, worker_id)
        
        return redirect("/create_team")
        
    else:
        return render_template("create_team.html")


@app.route("/delete_team", methods=["GET", "POST"])
@login_required
def delete_team():
    if request.method == "POST":
        team = request.form.get("team_name").strip().title()
        confirm = request.form.get("confirm")
        
        teams = [name["name"] for name in db.execute("SELECT name FROM teams")]

        if team not in teams:
            return apology("No such team exits!")
        
        if confirm is None or confirm == "no":
            return apology("Action canceled!")
        
        team_id = db.execute("SELECT id FROM teams WHERE name = ?", team)[0]["id"]
        
        db.execute("UPDATE workers SET team_id = NULL WHERE team_id = ?", team_id)
        db.execute("DELETE FROM teams WHERE id = ?", team_id)
        
        return redirect("/delete_team")
    
    else:
        return render_template("delete_team.html")


@app.route("/add_worker", methods=["GET", "POST"])
@login_required
def add_worker():
    #* Quite like register; However register is for admin role only
    """Allow administrators to add member to a team"""
    if request.method == "POST":
        name = request.form.get("name").strip()
        email = request.form.get("email").strip()
        password = request.form.get("password")
        role = request.form.get("role").strip().title()
        year = request.form.get("year")
        month = request.form.get("month")
        day = request.form.get("day")
        team = request.form.get("team").title()
        
        if not is_email(email):
            return apology("You must enter your email!")

        elif not name:
            return apology("You must enter your name!")
        
        elif not password:
            return apology("You must enter your password!")

        elif request.form.get("confirmation") != password:
            return apology("Your passwords must match!")
        
        elif not role:
            return apology("you must enter a role!")
        
        elif role.title() not in roles:
            return apology("Not an appropriate role!")
        
        teams = [name["name"] for name in db.execute("SELECT name FROM teams")]
        
        if team not in teams:
            return apology("Not a valid team!")

        try:
            y, m, d = map(int, [year, month, day])
            day = datetime(y, m, d)
        except ValueError:
            return apology("Inappropriate birthday!")
        
        fetch = db.execute("SELECT id, mem_count from teams WHERE name = ?", team)[0]
        team_id, curr = fetch["id"], fetch["mem_count"]
        
        db.execute("UPDATE teams SET mem_count = ? WHERE id = ?", int(curr) + 1, team_id)
        
        db.execute("INSERT INTO workers(name, email, role, pwd_hash, team_id, birth) VALUES(?, ?, ?, ?, ?, ?)",
                    name, email, role, generate_password_hash(password), team_id, day.strftime("%d/%m/%Y"))
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/redirect_worker", methods=["GET", "POST"])
@login_required
def redirect_wroker():
    """Allow administrators to change member's team"""
    if request.method == "POST":
        
        if not (name := request.form.get("name")):
            return apology("No name detected!")
        
        if not (email := request.form.get("email")):
            return apology("You must enter the worker's email!")
        
        if not (cur_team := request.form.get("current_team").title()):
            return apology("You must enter the current team the worker is in!")
        
        if not (new_team := request.form.get("new_team").title()):
            return apology("You must enter the new team the worker will be in!")
        
        if not (new_role := request.form.get("new_role").title()):
            return apology("You must specify the worker's new role!")
        
        if new_role.split(" ")[0] not in ["Ceo", "Director"] and new_role.split(" ")[1] not in ["Lead", "Manager", "Worker"]:
            return apology("Not a valid role1!")
        
        if new_role.startswith("Tech") and not (new_role.endswith("Lead") or new_role.endswith("Worker")):
            return apology("Not a valid role2!")
        
        if new_role.startswith("Hr") and not (new_role.endswith("Manager") or new_role.endswith("Worker")):
            return apology("Not a valid role3!")
        
        if new_team.startswith("Tech") and new_role.split(" ")[0] != "Tech":
            return apology("The role doesn't match with the team!")
        
        if new_team.startswith("Hr") and new_role.split(" ")[0] != "Hr":
            return apology("The role doesn't match with the team!")        
        
        if new_team == "Executives" and new_role not in ["Ceo", "Director"]:
            return apology("The role doesn't match with the team!")        
            
        if len(fetch := db.execute("SELECT id, team_id FROM workers WHERE name = ? AND email = ?", name, email)) == 0:
            return apology("No such worker found!")
        
        cur_team_id = fetch[0]["team_id"]
        
        if db.execute("SELECT name FROM teams WHERE id = ?", cur_team_id)[0]["name"] != cur_team:
            return apology("The worker currently work in a different team!")
        
        if len(new_team_id := db.execute("SELECT id FROM teams WHERE name = ?", new_team)) == 0:
            return apology("No such new team found!")
        
        worker_id = fetch[0]["id"]
        team_id = new_team_id[0]["id"]
        
        current_count = db.execute("SELECT mem_count FROM teams WHERE id = ?", cur_team_id)[0]["mem_count"]
        new_count = db.execute("SELECT mem_count FROM teams WHERE id = ?", team_id)[0]["mem_count"]
        
        db.execute("""UPDATE teams
                   SET mem_count = ? WHERE id = ? """, int(current_count) - 1, cur_team_id)
        
        db.execute("""UPDATE teams
                   SET mem_count = ? WHERE id = ? """, int(new_count) + 1, team_id)
        
        db.execute("UPDATE workers SET team_id = ?, role = ? WHERE id = ?", team_id, new_role, worker_id)
        
        return redirect("/redirect_worker")
        
    return render_template("redirect_worker.html")
    

@app.route("/sack", methods=["GET", "POST"])
@login_required
def sack():
    """Allow administrators to sack worker"""
    if request.method == "POST":
        name = request.form.get("name")
        email = request.form.get("email")
        
        if not (confirm := request.form.get("confirm")):
            return apology("Please confirm your action!")
        
        if confirm == "no":
            return apology("Action canceled!")

        # Verify that the worker exists
        if len(fetch := db.execute("SELECT id, team_id FROM workers WHERE name = ? AND email = ?", name, email)) < 1:
            return apology("No such worker exists!")
        
        worker_id, team_id = fetch[0]["id"], fetch[0]["team_id"]
        
        current = db.execute("SELECT mem_count FROM teams WHERE id = ?", team_id)[0]["mem_count"]
        
        # Update all tables 
        db.execute("UPDATE teams SET mem_count = ? WHERE id = ?", int(current) - 1, team_id)
        db.execute("DELETE FROM work WHERE worker_id = ?", worker_id)
        db.execute("DELETE FROM work WHERE reviewer_id = ?", worker_id)
        db.execute("DELETE FROM in_meetings WHERE worker_id = ?", worker_id)
        db.execute("DELETE FROM busi_meetings WHERE worker_id = ?", worker_id)
        db.execute("DELETE FROM timekeep WHERE worker_id = ?", worker_id)
        db.execute("DELETE FROM imgs WHERE worker_id = ?", worker_id)
        db.execute("DELETE FROM workers WHERE id = ?", worker_id)  
        
        return redirect("/sack") 
        
    else:
        return render_template("sack.html")


@app.route("/delegate_task", methods=["GET", "POST"])
@login_required
def delegate_task():
    """Allow administrators to create tasks for workers"""
    if request.method == "POST":
        
        if not (name := request.form.get("name")):
            return apology("No name found!")
        
        if not (email := request.form.get("email")):
            return apology("You must enter the worker's email!")
        
        if not (project := request.form.get("project")):
            return apology("You must enter the worker's email!")
        
        if not (info := request.form.get("info")):
            return apology("You must provide description for the worker!")
        
        if not (deadline := request.form.get("deadline")):
            return apology("You must enter a deadline for your work!")
        
        try:
            if datetime.strptime(deadline, "%d/%m/%Y") <= datetime.now():
                return apology("Not a valid deadline!")
        
        except ValueError:
            return apology("Inappropriate date format!")
        
        partner_rev = True
        if not (partner := request.form.get("partner")):
            partner_rev = False
        
        fetch3 = db.execute("SELECT id FROM partners WHERE name = ?", partner)
        if partner_rev and len(fetch3) < 1:
            return apology("No such partner found!")
        else:
            if partner_rev:
                partner_id = fetch3[0]["id"]
            else:
                partner_id = None
        
        if len(fetch1 := db.execute("SELECT id FROM workers WHERE name = ? AND email = ?", name, email)) < 1:
            return apology("No such worker found!")
        worker_id = fetch1[0]["id"]
        
        if len(fetch2 := db.execute("SELECT id FROM projects WHERE name = ?", project)) < 1:
            return apology("No such project found!")
        project_id = fetch2[0]["id"]
        
        if partner_rev:
            db.execute("""INSERT INTO work(worker_id, project_id, description, deadline, reviewer_id, partner_id)
                    VALUES(?, ?, ?, ?, ?, ?)""", worker_id, project_id, info, deadline, session["user_id"], partner_id)
        else:
            db.execute("""INSERT INTO work(worker_id, project_id, description, deadline, reviewer_id, partner_id)
                    VALUES(?, ?, ?, ?, ?, NULL)""", worker_id, project_id, info, deadline, session["user_id"])
            
        return redirect("/delegate_task")
        
    return render_template("delegate_task.html")
        

@app.route("/create_project", methods=["GET", "POST"])
@login_required
def create_project():
    """Allow administrators to create projects cooperating with or without partners"""
    if request.method == "POST":
        if not (name := request.form.get("name").title()):
            return apology("No")
        
        if (request.form.get("confirm") == "no"):
            return apology("Action canceled!")
        
        if len(db.execute("SELECT id FROM projects WHERE name = ?", name)) != 0:
            return apology("Another project also named {}!".format(name))
        
        if not (partner := request.form.get("partner").title()):
            db.execute("INSERT INTO projects(name, partner_id) VALUES(?, NULL)", name)
            
        else:
            if len(fetch := db.execute("SELECT id FROM partners WHERE name = ?", partner)) == 0:
                db.execute("INSERT INTO partners(name) VALUES(?)", partner)
                partner_id = db.execute("SELECT id FROM partners WHERE name = ?", partner)[0]["id"]
            
            else:
                partner_id = fetch[0]["id"]
            
            db.execute("INSERT INTO projects(name, partner_id) VALUES(?, ?)", name, partner_id)
            
        return redirect("/create_project")
    
    else:
        projects = []
        
        projcts = db.execute("SELECT name, partner_id FROM projects")
        
        for projct in projcts:
            partner = db.execute("SELECT name FROM partners WHERE id = ?", projct["partner_id"])
            
            if len(partner) == 0:
                partner = "None"
                
            else:
                partner = partner[0]["name"]
            
            projects.append({
                "name": projct["name"],
                "partner": partner
                })
        
        return render_template("project.html", projects=projects)


@app.route("/delete_project", methods=["GET", "POST"])
@login_required
def delete_project():
    if request.method == "POST":
        if not (name := request.form.get("name").title()):
            return apology("No project's name found!")
        
        if len(project_id := db.execute("SELECT id FROM projects WHERE name = ?", name)) == 0:
            return apology("No such project found!")
        
        if (request.form.get("confirm") == "no"):
            return apology("Action canceled!")
        
        db.execute("DELETE FROM work WHERE project_id = ?", project_id)
        db.execute("DELETE FROM in_meetings WHERE project_id = ?", project_id)
        db.execute("DELETE FROM busi_meetings WHERE project_id = ?", project_id)
        db.execute("DELETE FROM projects WHERE name = ?", name)
        
        return redirect("/create_project")
    
    else:
        return redirect("/create_project")


@app.route("/work", methods=["GET", "POST"])
@login_required
def work():
    """Remind user of what work they have"""
    if request.method == "POST":
        if not (project := request.form.get("project").title()):
            return apology("You must enter a project that the work belongs to!")
        
        if not (info := request.form.get("info")):
            return apology("You must add description for the work!")
        
        if not (deadline := request.form.get("deadline")):
            return apology("You must enter a deadline for your work!")
        
        try:
            if datetime.strptime(deadline, "%d/%m/%Y") <= datetime.now():
                return apology("Not a valid deadline!")
        
        except ValueError:
            return apology("Inappropriate date format!")
        
        review = True
        reviewer_email = request.form.get("reviewer_email")
        if not (reviewer := request.form.get("reviewer")) and not (reviewer_email):
            review = False
        
        partner_rev = True
        if not (partner := request.form.get("partner").title()):
            partner_rev = False
        
        if len(fetch1 := db.execute("SELECT id FROM projects WHERE name = ?", project)) < 1:
            return apology("No such project!")
        project_id = fetch1[0]["id"]
        
        fetch2 = db.execute("SELECT id FROM workers WHERE name = ? AND email = ?", reviewer, reviewer_email)
        if review and len(fetch2) < 1:
            return apology("No such reviewer!")
        if len(fetch2) == 0:
            reviewer_id = None
        else:
            reviewer_id = fetch2[0]["id"]
        
        fetch3 = db.execute("SELECT id FROM partners WHERE name = ?", partner)
        if partner_rev and len(fetch3) < 1:
            return apology("No such partner!")
        if len(fetch3) == 0:
            partner_id = None
        else:
            partner_id = fetch3[0]["id"]
           
        if review:
            if partner_rev:
                db.execute("""INSERT INTO work(worker_id, project_id, description, deadline, reviewer_id, partner_id)
                        VALUES(?, ?, ?, ?, ?, ?)""", session["user_id"], project_id, info, deadline, reviewer_id, partner_id)
            else:
                db.execute("""INSERT INTO work(worker_id, project_id, description, deadline, reviewer_id, partner_id)
                        VALUES(?, ?, ?, ?, ?, NULL)""", session["user_id"], project_id, info, deadline, reviewer_id)
        
        else:
            if partner_id:
                db.execute("""INSERT INTO work(worker_id, project_id, description, deadline, reviewer_id, partner_id)
                            VALUES(?, ?, ?, ?, NULL, ?)""", session["user_id"], project_id, info, deadline, partner_id)
            else:
                db.execute("""INSERT INTO work(worker_id, project_id, description, deadline, reviewer_id, partner_id)
                        VALUES(?, ?, ?, ?, NULL, NULL)""", session["user_id"], project_id, info, deadline)
            
        return redirect("/work")
        
    else:
        works = []
        
        work = db.execute("SELECT * FROM work WHERE worker_id = ?", session["user_id"])
        
        for item in work:
            if datetime.strptime(item["deadline"], "%d/%m/%Y") < datetime.now():
                continue
            if len(fetch1 := db.execute("SELECT name, email FROM workers WHERE workers.id = ?", item["reviewer_id"])) == 0:
                reviewer, re_email = "None", "None"
            else:
                reviewer, re_email = fetch1[0]["name"], fetch1[0]["email"],
                
            if len(fetch2 := db.execute("SELECT name FROM partners WHERE partners.id = ?", item["partner_id"])) == 0:
                partner = "None"
            else:
                partner = fetch2[0]["name"]
                    
            project = db.execute("SELECT name FROM projects WHERE id = ?", item["project_id"])[0]["name"]
            
            works.append([ item["id"], 
                {"reviewer": reviewer,
                "email": re_email,
                "project": project,
                "partner": partner,
                "description": item["description"],
                "deadline": item["deadline"]
                }])
            
            # Let the closest deadline pop up first 
            works = sorted(works, key=lambda data: datetime.strptime(data[1]["deadline"], "%d/%m/%Y"), reverse=False)
        
        return render_template("work.html", works=works)


@app.route("/delete_work", methods=["POST"])
@login_required
def delete_work():
    work_id = request.form.get("id")
    db.execute("DELETE FROM work WHERE id = ?", work_id)
    
    return redirect("/work")


@app.route("/in_meeting", methods=["GET", "POST"])
@login_required
def in_meeting():
    """Allow workers to add and watch meetings within the company"""
    if request.method == "POST":
        # Allow each users to add new in-company meeting
        day = request.form.get("day")
        hour = request.form.get("hour")
        project = request.form.get("project").title()
        dest = request.form.get("dest").strip()
        info = request.form.get("info").strip()
        
        if len(projct := db.execute("SELECT id FROM projects WHERE name = ?", project)) == 0:
            return apology(f"No project named {project} is found")
        project_id = projct[0]["id"]
        
        time = day + ", " + hour
        try:
            if (datetime.strptime(time, "%d/%m/%Y, %H:%M") <= datetime.now()):
                return apology("You can't set meeting time in the past!")
        
        except ValueError:
            return apology("Please enter the correct time format!")
        
        db.execute("""INSERT INTO in_meetings(time, destination, project_id, info, worker_id) 
                   VALUES(?, ?, ?, ?, ?)""", time, dest, project_id, info, session["user_id"])
        
        return redirect("/in_meeting")
    
    else:
        # Display a table about in-company meetings
        in_meetings = []
        
        in_meets = db.execute("SELECT * FROM in_meetings WHERE worker_id = ?", session["user_id"])
        for item in in_meets:
            if datetime.strptime(item["time"], "%d/%m/%Y, %H:%M") < datetime.now():
                continue
            
            project = db.execute("SELECT name FROM projects WHERE id = ?", item["project_id"])[0]["name"]
            
            in_meetings.append([item["id"],
                {"project": project,
                "info": item["info"],
                "time": item["time"],
                "dest": item["destination"]
                }])    
        
        in_meetings = sorted(in_meetings, key=lambda data: datetime.strptime(data[1]["time"], "%d/%m/%Y, %H:%M"), reverse=False)
        
        return render_template("in_meeting.html", in_meetings=in_meetings)


@app.route("/delete_in_meet", methods=["POST"])
@login_required
def delete_in_meet():
    meet_id = request.form.get("id")
    db.execute("DELETE FROM in_meetings WHERE id = ?", meet_id)
    
    return redirect("/in_meeting")


@app.route("/out_meeting", methods=["GET", "POST"])
@login_required
def out_meeting():
    """Allow workers to add meetings with any partners"""
    if request.method == "POST":
        # Allow each users to add new outside-company meeting
        day = request.form.get("day")
        hour = request.form.get("hour")
        project = request.form.get("project").title()
        partner = request.form.get("partner").title()
        dest = request.form.get("dest")
        info = request.form.get("info")
        
        if len(projct := db.execute("SELECT id FROM projects WHERE name = ?", project)) == 0:
            return apology(f"No project named {project} is found")
        project_id = projct[0]["id"]
        
        if partner:
            if len(partnr := db.execute("SELECT id FROM partners WHERE name = ?", partner)) == 0:
                return apology(f"No partner named {partner} is found")
            partner_id = partnr[0]["id"]
        
        time = f"{day}, {hour}"
        try:
            if (datetime.strptime(time, "%d/%m/%Y, %H:%M") <= datetime.now()):
                return apology("You can't set meeting time in the past!")
            
        except ValueError:
            return apology("Please enter a correct time format!")
        if partner:
            db.execute("""INSERT INTO busi_meetings(time, destination, project_id, partner_id, info, worker_id) 
                    VALUES(?, ?, ?, ?, ?, ?)""", time, dest, project_id, partner_id, info, session["user_id"])
        
        else:
            db.execute("""INSERT INTO busi_meetings(time, destination, project_id, partner_id, info, worker_id) 
                    VALUES(?, ?, ?, NULL, ?, ?)""", time, dest, project_id, info, session["user_id"])
        
        return redirect("/out_meeting")
    
    else:
        # Display a table about outside-company meetings
        out_meetings = []
        
        out_meets = db.execute("SELECT * FROM busi_meetings WHERE worker_id = ?", session["user_id"])
        for item in out_meets:
            
            if datetime.strptime(item["deadline"], "%d/%m/%Y, %H:%M") < datetime.now():
                continue
            
            project = db.execute("SELECT name FROM projects WHERE id = ?", item["project_id"])[0]["name"]
            if item["partner_id"]:
                partner = db.execute("SELECT name FROM partners WHERE id = ?", item["partner_id"])[0]["name"]
            
            else:
                partner = "None"
            
            out_meetings.append([item["id"],
                {"project": project,
                "partner": partner,
                "info": item["info"],
                "time": item["time"],
                "dest": item["destination"]
                }])    
        
        out_meetings = sorted(out_meetings, key=lambda data: datetime.strptime(data[1]["time"], "%d/%m/%Y, %H:%M"), reverse=False)
        
        return render_template("out_meeting.html", out_meetings=out_meetings)


@app.route("/delete_out_meet", methods=["POST"])
@login_required
def delete_out_meet():
    meet_id = request.form.get("id")
    db.execute("DELETE FROM busi_meetings WHERE id = ?", meet_id)
    
    return redirect("/out_meeting")


@app.route("/time_keep", methods=["GET", "POST"])
@login_required
def timekeep():
    if request.method == "POST":
        mark = request.form.get("timekeep")
        now = datetime.now()
        date, time = datetime.strftime(now, "%Y/%m/%d"), datetime.strftime(now, "%H:%M:%S")
        if mark == "start": 
            if len(db.execute("SELECT time_in FROM timekeep WHERE worker_id = ? AND date = ?",
                                session["user_id"], date)) != 0:
                return apology("You can only hit start once every day!")
            
            else:
                db.execute("""INSERT INTO timekeep(worker_id, date, time_in, time_out, work_time)
                       VALUES(?, ?, ?, NULL, NULL)""", session["user_id"], date, time)
        
        else:
            try:
                time_in = db.execute("SELECT time_in FROM timekeep WHERE worker_id = ? AND date = ?",
                                session["user_id"], date)[0]["time_in"]
                start = datetime.strptime(f"{date} {time_in}", "%Y/%m/%d %H:%M:%S")
                
            except:
                return apology("You havent't check in yet! Please contact the admin to add your work time!")
            
            delta = int((now - start).total_seconds())
            hours, min_sec = divmod(delta, 3600)
            mins, secs = divmod(min_sec, 60)
            delta = f"{hours}:{mins}:{secs}"
            
            
            db.execute("""UPDATE timekeep SET time_out = ?, work_time = ? WHERE worker_id = ? AND date = ? AND time_in = ?""",
                       time, delta, session["user_id"], date, time_in)
        
        return redirect("/")
        
    else:
        time_keeping = []
        for day in db.execute("SELECT * FROM timekeep WHERE worker_id = ? ORDER BY date DESC LIMIT 7", session["user_id"]):
            time_keeping.append({
                "date": day["date"],
                "time_in": day["time_in"],
                "time_out": str(day["time_out"]),
                "work_time": str(day["work_time"])
                })
        
        return render_template("timekeep.html", time_keeping=time_keeping)
