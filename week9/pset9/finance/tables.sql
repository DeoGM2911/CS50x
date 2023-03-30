-- This sheet is for creating a new table recording users' purchases, companies, and shares that each user has
CREATE TABLE purchases (
    id INTEGER,
    date NUMERIC,
    type TEXT NOT NULL,
    user_id INTEGER,
    company_id INTEGER,
    shares INTEGER,
    price INTEGER NOT NULL,
    PRIMARY KEY(id),
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(company_id) REFERENCES companies(id)
);

CREATE TABLE companies (
    id INTEGER,
    name TEXT,
    symbol TEXT UNIQUE,
    PRIMARY KEY(id)
);

CREATE TABLE stocks (
    user_id INTEGER,
    shares INTEGER NOT NULL,
    company_id INTEGER,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(company_id) REFERENCES companies(id)
);