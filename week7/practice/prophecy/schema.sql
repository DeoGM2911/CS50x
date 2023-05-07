CREATE TABLE students (
    id INTEGER,
    student_name TEXT NOT NULL,
    PRIMARY KEY(id)
);

CREATE TABLE houses (
    id INTEGER,
    house TEXT NOT NULL UNIQUE,
    head TEXT NOT NULL UNIQUE,
    PRIMARY KEY(id)
);

CREATE TABLE link (
    student_id INTEGER UNIQUE NOT NULL,
    house_id INTEGER UNIQUE NOT NULL,
)