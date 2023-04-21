# **Adminine** (Admin Online)

## 1. *Video Demo*:  <url_here>

### 2. **Description**

#### 2.1 **Overview**

- Adminine (or Admin Online) is a web-based application aim to porvide some flexibility to work with some major features such as adding and viewing pending work, meetings in or outside the company, etc.. For each role in the model, there is specific duties or abilities that they have.
- This project is based on a simple model of a company with CEOs and Directors in the Executives team, Technical Leaders and Workers (Tech Lead and Worker) in Technical team (Tech), and Human Resource (HR) Manager and Worker (Hr Manager and Worker) in Human Resource team (Hr).
- This project uses Python, the Flask framwork along with SQLite database and the UI coded with HTML and CSS. In addition, I **also use the apology function and the login_required wrapper in funcs.py from PSET 9 - Finance, so I want to have a special thanks and shout out to the CS50 team members who worte the code**
*Remarks*: This project mostly aim for back-end skills; therefore, the UI isn't given much attention and there is no JS files! I probably should also learn more about front-end and JS afterwards.

#### 2.2 **Details about the application**

##### 1. Log in, log out, time keep, and user's profile

###### a. Log_in (route: /login)

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

###### b. Log out (route: /logout)

- Log out the current user simply by clear the session (both user_id and role) and redirect them to the log in page.

``` Python
@app.route("/logout", methods=["GET", "POST"])
def logout():
    session.clear()
    return redirect("/")
```

###### c. Time keeping (route: /time_keep)

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

###### d. User's profile (route: /)

- The user profile contains basic information about the user like his/her name, date of birth, role, team, email, and a profile image of him/her. The user can change his/her profile image by uploading his/her own image; however, it's noticed that the image should be like a image on an ID or the like (avoiding overflow or too big picture).

- index.html file is extended from layout.html, containing a navigation bar that help the user to access other features that are listed in the following sections.  
