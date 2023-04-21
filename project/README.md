# **Adminine** (Admin Online)

## 1. *Video Demo*:  <url_here>

### 2. **Description**

### 2.1 **Overview**

- Adminine (or Admin Online) is a web-based application aim to porvide some flexibility to work with some major features such as adding and viewing pending work, meetings in or outside the company, etc.. For each role in the model, there is specific duties or abilities that they have.
- This project is based on a simple model of a company with CEOs and Directors in the Executives team, Technical Leaders and Workers (Tech Lead and Worker) in Technical team (Tech), and Human Resource (HR) Manager and Worker (Hr Manager and Worker) in Human Resource team (Hr).
- This project uses Python, the Flask framwork along with SQLite database and the UI coded with HTML and CSS. In addition, I **also use the apology function and the login_required wrapper in funcs.py from PSET 9 - Finance, so I want to have a special thanks and shout out to the CS50 team members who worte the code**
*Remarks*: This project mostly aim for back-end skills; therefore, the UI isn't given much attention and there is no JS files! I probably should also learn more about front-end and JS afterwards.

### 2.2 **Details about the application**

- Configure the application

```Python
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
```

- Database schema:

```SQL
CREATE TABLE teams (
    id INTEGER,
    name TEXT NOT NULL UNIQUE,
    mem_count INTEGER NOT NULL DEFAULT (0),
    PRIMARY KEY(id)
);

CREATE TABLE workers (
    id INTEGER,
    name TEXT NOT NULL,
    email TEXT NOT NULL UNIQUE,
    pwd_hash TEXT NOT NULL,
    team_id INTEGER NOT NULL,
    birth NUMERIC NOT NULL,
    role TEXT NOT NULL,
    PRIMARY KEY(id),
    FOREIGN KEY(team_id) REFERENCES teams(id)
);

CREATE TABLE imgs (
    worker_id INTEGER,
    src TEXT NOT NULL,
    FOREIGN KEY(worker_id) REFERENCES workers(id)
);

CREATE TABLE work (
    id INTEGER,
    worker_id INTEGER,
    project_id INTEGER,
    description TEXT NOT NULL,
    deadline NUMERIC NOT NULL,
    reviewer_id INTEGER,
    partner_id INTEGER,
    PRIMARY KEY(id),
    FOREIGN KEY(worker_id) REFERENCES workers(id),
    FOREIGN KEY(reviewer_id) REFERENCES workers(id),
    FOREIGN KEY(project_id) REFERENCES projects(id),
    FOREIGN KEY(partner_id) REFERENCES partners(id)
);

CREATE TABLE partners (
    id INTEGER,
    name TEXT NOT NULL,
    PRIMARY KEY(id)
);

CREATE TABLE projects (
    id INTEGER,
    name TEXT NOT NULL UNIQUE,
    partner_id INTEGER, 
    PRIMARY KEY(id),
    FOREIGN KEY(partner_id) REFERENCES partners(id)
);

CREATE TABLE timekeep (
    worker_id INTEGER,
    date TEXT NOT NULL,
    time_in NUMERIC,
    time_out NUMERIC,
    work_time REAL,
    FOREIGN KEY(worker_id) REFERENCES workers(id)
);

-- In-company meetings
CREATE TABLE in_meetings (
    id INTEGER,
    worker_id INTEGER,
    project_id INTEGER,
    info TEXT NOT NULL,
    time NUMERIC NOT NULL,
    destination TEXT NOT NULL,
    PRIMARY KEY(id),
    FOREIGN KEY(worker_id) REFERENCES workers(id),
    FOREIGN KEY(project_id) REFERENCES projects(id)
);

-- partner meetings
CREATE TABLE busi_meetings (
    id INTEGER,
    worker_id INTEGER,
    partner_id INTEGER,
    project_id INTEGER,
    info NOT NULL,
    time NUMERIC NOT NULL,
    destination TEXT NOT NULL,
    PRIMARY KEY(id),
    FOREIGN KEY(worker_id) REFERENCES workers(id),
    FOREIGN KEY(partner_id) REFERENCES partners(id),
    FOREIGN KEY(project_id) REFERENCES projects(id)
);
```

#### 1. Log in, log out, time keep, change, and user's profile

- *layout.html*

The layout.html file contains block with Jinja conditions like this. By doing so, it helps to set different abilities to each user that has a specific role. This is an unorder list with each element being an anchor lead to another route

```HTML
{% if session["role"] == "Admin" %}
          <ul class="navbar-nav me-auto mt-2">
            <li class="nav-item dropdown">
              <a
                class="nav-link dropdown-toggle"
                href="#"
                id="navbarScrollingDropdown"
                role="button"
                data-bs-toggle="dropdown"
                aria-expanded="false"
              >
                Others
              </a>
              <ul
                class="dropdown-menu"
                aria-labelledby="navbarScrollingDropdown"
              >
                <li class="nav-item">
                  <a class="nav-link" href="/register">Register a Role</a>
                </li>
              </ul>
            </li>
          </ul>
{% endif %}
```

#### a. Log_in (route: /login)

- The implementation of log in consists of three main steps. First, delete any session (or user_id), then the app gets the user's email and password and connect to the database to check whether the provided information is matched.

```Python
fetch = db.execute("SELECT id, role, pwd_hash FROM workers WHERE email = ?", request.form.get("email"))
if len(fetch) != 1:
    return apology("Incorrect email or password!", 403)
else:
    user_id, role, pwd_hash = fetch[0]["id"], fetch[0]["role"], fetch[0]["pwd_hash"]
# Ensure username exists and password is correct
if not check_password_hash(pwd_hash, request.form.get("password")):
    return apology("Invalid email or password!", 403)
```

- After that, a user is assign with 2 session values: user_id and role. The user_id helps with accessing data relating to the user in the database, while the role will help distinguish the abilities that each user has depending on their position in the company. The login.html file also contains useful and friendly messages for users.

```Python
session["user_id"] = user_id
session["role"] = role
```

- This is a list of accpected roles.

```Python
roles = ["Ceo","Tech Lead", "Hr Manager", "Hr Worker", "Tech Worker", "Director", "Admin"]
```

#### b. Log out (route: /logout)

- Log out the current user simply by clear the session (both user_id and role) and redirect them to the log in page.

``` Python
@app.route("/logout", methods=["GET", "POST"])
def logout():
    session.clear()
    return redirect("/")
```

#### c. Time keeping (route: /time_keep)

- This route allow users to record the start of their working time (**Notice**: The worker can only hit start ONCE!) and end their working time by hitting end. They can hit end multiple time, and therefore, are able to know exactly how many hours they have work. However, please notice that SQLite support date format as YYYY/MM/DD.

```Python
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
```

-The timekeep.html file also has a table that shows the working hour of the last seven working days!

```Python
time_keeping = []
for day in db.execute("SELECT * FROM timekeep WHERE worker_id = ? ORDER BY date DESC LIMIT 7", session["user_id"]):
    time_keeping.append({
        "date": day["date"],
        "time_in": day["time_in"],
        "time_out": str(day["time_out"]),
        "work_time": str(day["work_time"])
        })

return render_template("timekeep.html", time_keeping=time_keeping)
```

#### d. Change (route: /change)

- This route allow user to change password, requiring him/her to enter the old password and the new one, then check the old and allow the change after all the conditions bellow is qualified.

```Python
if not check_password_hash(password, old):
    return apology("Incorrect password!")

if not new:
    return apology("You must enter a new password!")

if new != re_new:
    return apology("Retype password isn't matched!")

db.execute("UPDATE workers SET pwd_hash = ? WHERE id = ?", generate_password_hash(new), session["user_id"])
```

#### e. User's profile (route: /)

- The user profile contains basic information about the user like his/her name, date of birth, role, team, email, and a profile image of him/her. The user can change his/her profile image by uploading his/her own image (the images will be stored in UPLOADED_FOLDER); however, it's noticed that the image should be like a image on an ID or the like (avoiding overflow or too big picture).

```Python
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
```

- index.html file is extended from layout.html, containing a navigation bar that help the user to access other features that are listed in the following sections.  

#### 2. Register, add worker, redirect worker, and sack

#### a. Register

- Register is a feature that's only for the Admin. This functionality is used for developmental phase to add workers easily. It requires the admin to add basic information about the added worker, then verify the data, and insert them into the database.

```Python
if request.method == "POST":
  name = request.form.get("name").strip()
  email = request.form.get("email").strip()
  password = request.form.get("password")
  role = request.form.get("role").strip().title()
  year = request.form.get("year")
  month = request.form.get("month")
  day = request.form.get("day")
  team = request.form.get("team")
  
  # Validation process here
  ...

  fetch = db.execute("SELECT id, mem_count from teams WHERE name = ?", team.title())[0]
  team_id, curr = fetch["id"], fetch["mem_count"]
  
  db.execute("UPDATE teams SET mem_count = ? WHERE id = ?", int(curr) + 1, team_id)
  
  db.execute("INSERT INTO workers(name, email, role, pwd_hash, team_id, birth) VALUES(?, ?, ?, ?, ?, ?)",
              name, email, role.title(), generate_password_hash(password), team_id, day.strftime("%d/%m/%Y"))
  return redirect("/login")
```

#### b. Add worker

- This feature is *EXACTLY* the same as register, however, this is created for roles such as Team leaders and executives.

#### c. Redirect worker

- This route help team leaders and executives to change the team that a worker is working in. It requires the name, email, the current and the new team of the worker. Then the data is validated and the database is updated. The member count of each team is also updated along with the worker's team_id.

```Python
current_count = db.execute("SELECT mem_count FROM teams WHERE id = ?", cur_team_id)[0]["mem_count"]
new_count = db.execute("SELECT mem_count FROM teams WHERE id = ?", team_id)[0]["mem_count"]

db.execute("""UPDATE teams
            SET mem_count = ? WHERE id = ? """, int(current_count) - 1, cur_team_id)

db.execute("""UPDATE teams
            SET mem_count = ? WHERE id = ? """, int(new_count) + 1, team_id)

db.execute("UPDATE workers SET team_id = ?, role = ? WHERE id = ?", team_id, new_role, worker_id)
```

#### c. Sack

- Sack allows managers and leaders to sack a member in a team provided the name and email of the worker. Sack will delete any information about the user in the database and update the member count in the team that the sacked member used to work in.

```Python
# Validation process here

# Update all tables 
db.execute("UPDATE teams SET mem_count = ? WHERE id = ?", int(current) - 1, team_id)
db.execute("DELETE FROM work WHERE worker_id = ?", worker_id)
db.execute("DELETE FROM work WHERE reviewer_id = ?", worker_id)
db.execute("DELETE FROM in_meetings WHERE worker_id = ?", worker_id)
db.execute("DELETE FROM busi_meetings WHERE worker_id = ?", worker_id)
db.execute("DELETE FROM timekeep WHERE worker_id = ?", worker_id)
db.execute("DELETE FROM imgs WHERE worker_id = ?", worker_id)
db.execute("DELETE FROM workers WHERE id = ?", worker_id) 
```

#### 3. Create and delete project, create and delete team, delegate tasks, and work and delete work

#### a. Create and delete project

- These features are only for executives roles. When creating a project name is prompted and the user can optionally add a partner for that project. While deleting one, the project name is needed and all information about that project (in projects, work, in_meetings, and busi_meetings tables).

```Python
# Create a project
if len(fetch := db.execute("SELECT id FROM partners WHERE name = ?", partner)) == 0:
db.execute("INSERT INTO partners(name) VALUES(?)", partner)
partner_id = db.execute("SELECT id FROM partners WHERE name = ?", partner)[0]["id"]

else:
partner_id = fetch[0]["id"]

db.execute("INSERT INTO projects(name, partner_id) VALUES(?, ?)", name, partner_id)

# Delete a project
db.execute("DELETE FROM work WHERE project_id = ?", project_id)
db.execute("DELETE FROM in_meetings WHERE project_id = ?", project_id)
db.execute("DELETE FROM busi_meetings WHERE project_id = ?", project_id)
db.execute("DELETE FROM projects WHERE name = ?", name)
```

- The html for creating and deleting also contains a table that lists all projects recorded (in other words, in the database).

```Python
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
```

```HTML
<table class="table table-striped table-bordered table-hover">
    <caption style="font-size:large; padding: 5px; margin:10px;">Projects</caption>
    <thead>
        <th scope="col">No.</th>
        <th scope="col">Project</th>
        <th scope="col">Partner</th>
    </thead>
    <tbody>
        {% for project in projects %}
        <tr>
            <th scope="row">{{ loop.index }}</th>
            <td>{{ project["name"] }}</td>
            <td>{{ project["partner"] }}</td>
        </tr>
        {% endfor %}
    </tbody>
</table>
```

#### b. Create and delete team

- This functionality allows executives to create and delete a team. It requires the user to input the team name and the team leader (who must already be in the company) when creating a team, and the team name when deleting. Notice: when deleting a team, all member in that team will belong to NO team; therefore, it's recommended to sack the member then add them back with a whole new account on a new team.

```Python
# *When create a team
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

db.execute("""UPDATE workers SET team_id = ?, role = ?
          WHERE id = ?""", team_id, role, worker_id)

# * When delete a team
if confirm is None or confirm == "no":
  return apology("Action canceled!")

team_id = db.execute("SELECT id FROM teams WHERE name = ?", team)[0]["id"]

db.execute("UPDATE workers SET team_id = NULL WHERE team_id = ?", team_id)
db.execute("DELETE FROM teams WHERE id = ?", team_id)
```

#### c. Delegate tasks

- This feature allow ALL users to add work to other counterparts.

- The HTML form includes a \<textarea> that helps with multiple lines description from the user.

```HTML
<form action="/delegate_task" method="post" id="delegate">
    <label for="name">Worker's name:</label>
    <input type="text" name="name" id="name" placeholder="Name" class="form-control mx-auto w-auto" autocomplete="off" autofocus>
    <label for="email">Worker's email:</label>
    <input type="text" name="email" id="email" placeholder="Email" class="form-control mx-auto w-auto" autocomplete="off">
    <label for="project">Project's name:</label>
    <input type="text" name="project" id="project" placeholder="Project" class="form-control mx-auto w-auto" autocomplete="off">
    <label for="info">Deadline (Format: DD/MM/YYYY):</label>
    <input type="text" name="deadline" id="deadline" placeholder="Deadline" class="form-control mx-auto w-auto" autocomplete="off">
    <label for="partner">Partner:</label>
    <input type="text" name="partner" id="partner" placeholder="Partner" class="form-control mx-auto w-auto" autocomplete="off">
    <label for="info">Description:</label>
    <textarea form="delegate" name="info" style="margin-top: 10px;" id="info" placeholder="Description" autocomplete="off"></textarea>
    <input type="submit" id="sub-btn" value="Add task">
</form>
```

```Python
if partner_rev:
  db.execute("""INSERT INTO work(worker_id, project_id, description, deadline, reviewer_id, partner_id)
          VALUES(?, ?, ?, ?, ?, ?)""", worker_id, project_id, info, deadline, session["user_id"], partner_id)
else:
  db.execute("""INSERT INTO work(worker_id, project_id, description, deadline, reviewer_id, partner_id)
          VALUES(?, ?, ?, ?, ?, NULL)""", worker_id, project_id, info, deadline, session["user_id"])
```

#### d. Work and delete work

#### 4. In and Outside meetings

#### a. In meetings

#### b. Outside meetings
