import csv
from cs50 import SQL

# Connect to the database and create tables
db = SQL("sqlite:///roster.db")
db.execute("""DROP TABLE students""")

db.execute("""CREATE TABLE students (
                id INTEGER,
                student_name TEXT NOT NULL,
                PRIMARY KEY(id)
            )""")
db.execute("""CREATE TABLE houses (
                id INTEGER,
                house TEXT NOT NULL,
                head TEXT NOT NULL,
                PRIMARY KEY(id)
            )""")
db.execute("""CREATE TABLE link (
                student_id INTEGER UNIQUE NOT NULL,
                house_id INTEGER UNIQUE NOT NULL
            )""")

# Read the csv file and write data into tables
with open("prophecy\students.csv", "r") as file:
    reader = csv.DictReader(file)
    for row in reader:
        db.execute("INSERT INTO students(student_name) VALUES(?)", row["student_name"])

        student_id = db.execute("SELECT id FROM sutdents WHERE student_name = ?", row["student_name"])[0]["id"]
        db.execute("INSERT INTO link(student_id) VALUES(?)", student_id)

        db.execute("INSERT INTO houses(house, head) VALUES(?, ?)", row["house"], row["head"])

        house_id = db.execute("SELECT id FROM houses WHERE house = ?", row["house"])[0]["id"]
        db.execute("INSERT INTO link(house_id) VALUES(?)", house_id)
