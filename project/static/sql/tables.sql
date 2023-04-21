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
