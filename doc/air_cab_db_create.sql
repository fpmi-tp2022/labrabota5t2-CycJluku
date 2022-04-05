-- Created by Vertabelo (http://vertabelo.com)
-- Last modification date: 2022-04-05 19:17:30.063

-- tables
-- Table: Flights
CREATE TABLE Flights (
    id integer NOT NULL CONSTRAINT Flights_pk PRIMARY KEY,
    helicopter_id integer NOT NULL,
    date date NOT NULL,
    mass_cargo integer NOT NULL,
    people_amount integer NOT NULL,
    duration integer NOT NULL,
    price integer NOT NULL,
    type_id integer NOT NULL,
    CONSTRAINT Flights_Helicopters FOREIGN KEY (helicopter_id)
    REFERENCES Helicopters (id),
    CONSTRAINT Flights_Types FOREIGN KEY (type_id)
    REFERENCES Types (id)
);

-- Table: Helicopters
CREATE TABLE Helicopters (
    id integer NOT NULL CONSTRAINT Helicopters_pk PRIMARY KEY,
    brand text NOT NULL,
    production_date date NOT NULL,
    load_capacity integer NOT NULL,
    last_overhaul_date date NOT NULL,
    flights_resource integer NOT NULL
);

-- Table: Pilots
CREATE TABLE Pilots (
    id integer NOT NULL CONSTRAINT Pilots_pk PRIMARY KEY,
    surname text NOT NULL,
    position_id integer NOT NULL,
    experience integer NOT NULL,
    address text NOT NULL,
    birth_year integer NOT NULL,
    helicopter_id integer NOT NULL,
    login text NOT NULL,
    password text NOT NULL,
    CONSTRAINT Pilots_Helicopter FOREIGN KEY (helicopter_id)
    REFERENCES Helicopters (id),
    CONSTRAINT Pilots_Positions FOREIGN KEY (position_id)
    REFERENCES Positions (id)
);

-- Table: Positions
CREATE TABLE Positions (
    id integer NOT NULL CONSTRAINT Positions_pk PRIMARY KEY,
    name text NOT NULL
);

-- Table: Sums
CREATE TABLE Sums (
    id integer NOT NULL CONSTRAINT Sums_pk PRIMARY KEY,
    period_strart date NOT NULL,
    period_end date NOT NULL,
    sums integer NOT NULL
);

-- Table: Types
CREATE TABLE Types (
    id integer NOT NULL CONSTRAINT Types_pk PRIMARY KEY,
    name text NOT NULL,
    salary_ratio real NOT NULL
);

-- End of file.

