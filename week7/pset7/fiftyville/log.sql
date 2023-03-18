 -- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find the details about the report
SELECT description FROM crime_scene_reports
WHERE day = 28 AND month = 7 AND year = 2021
AND street = 'Humphrey Street';

--10:15am
--the Humphrey Street bakery
--Interviews with three witnesses who were present at the time
-- each of their interview transcripts mentions the bakery.

-- Rretrieve the transcript of the interviews
SELECT transcript FROM interviews
WHERE day = 28 AND month = 7 AND year = 2021 
AND transcript LIKE '%bakery%'

--Witness 1: Sometime within ten minutes of the theft,
-- I saw the thief get into a car in the bakery parking lot 
--and drive away. If you have security footage 
--from the bakery parking lot, you might want 
--to look for cars that left the parking lot in that time frame.

-- Retrieve suspecting license plates, call this table lp 
SELECT license_plate AS lp FROM bakery_security_logs
WHERE day = 28 AND month = 7 AND year = 2021
AND hour = 10 AND (minute <= 25 AND minute >= 15);

-- Retrieve data about people who have the above license plates
SELECT id FROM people JOIN lp ON lp = people.license_plate;


--Witness 2: I don't know the thief's name,
-- but it was someone I recognized.
-- Earlier this morning,
--before I arrived at Emma's bakery, 
--I was walking by the ATM on Leggett Street 
-- and saw the thief there withdrawing some money.

-- Retrieve data about account numbers withdrawing on Leggett street
SELECT account_number AS an FROM atm_transactions 
WHERE day = 28 AND month = 7 AND year = 2021
AND atm_location = 'Leggett Street' 
AND transaction_type = 'withdraw';

SELECT person_id FROM bank_accounts JOIN an
ON an = bank_accounts.account_number;

--Witness 3: As the thief was leaving the bakery, 
--they called someone who talked to them for less than a minute. 
--In the call, 
--I heard the thief say that they were plannin
-- to take the earliest flight out of Fiftyville tomorrow (29th July).
-- The thief then asked the person on the other end of
-- the phone to purchse the flight ticket.

-- Retrieve callers' and receivers' numbers with calls less than 1 min.
SELECT caller AS cal FROM phone_calls 
WHERE day = 28 AND month = 7 AND year = 2021
AND duration < 60;
SELECT receiver AS rec FROM phone_calls 
WHERE day = 28 AND month = 7 AND year = 2021
AND duration < 60;

-- Query to find callers' and receivers' id:
SELECT people.id FROM people JOIN cal ON cal = people.phone_number;
SELECT people.id FROM people JOIN rec ON rec = people.phone_number;


-- Find the id of Fiftyville Airport:
SELECT id FROM airports WHERE city = 'Fiftyville';

-- The airport's id is 8.

-- Find all flight in 29th July 2021
SELECT id, hour, minute FROM flights
WHERE day = 29 AND month = 7 AND year = 2021
AND origin_airport_id = 8;
-- Note: We can run 2 other queries to find the earliest flight via the hour and minute
--id, hour, minute
--18	16	0
--23	12	15
--36	8	20 => Earliest flight
--43	9	30
--53	15	20

-- Retrieve the passport numbers of the passengers in flight 36
SELECT passport_number AS pn FROM passengers WHERE flight_id = 36;

-- Retrieve data about the passengers' id
SELECT people.id FROM people JOIN pn ON pn = people.passport_number;

-- Match the caller with the receiver to find the accomplance
SELECT phone_number FROM people WHERE id = 686048;

-- Find the destiantion
SELECT destination_airport_id FROM flights WHERE id = 36; -- This gives 4
SELECT city FROM airports WHERE id = 4;
-- The destination is New York City

-- ! 686048 -> The theif
-- The name of the theif is Bruce
SELECT name FROM people WHERE id = 686048;

-- Find the phone call between the accomplice and the theif
SELECT phone_calls.receiver as rec FROM
people JOIN phone_calls ON caller = people.phone_number 
WHERE people.id = 686048 AND 
phone_calls.day = 28 AND phone_calls.month = 7 AND phone_calls.year = 2021;

SELECT people.name FROM people JOIN phone_calls JOIN 
(SELECT phone_calls.receiver AS rec FROM people JOIN phone_calls ON caller = people.phone_number 
WHERE people.id = 686048 AND phone_calls.day = 28 AND phone_calls.month = 7 AND phone_calls.year = 2021)
ON rec = people.phone_number AND rec = phone_calls.receiver WHERE phone_calls.duration < 60;

-- Fin the accomplice
SELECT name FROM people WHERE id = 686048;SELECT name FROM people WHERE id = 864400;
-- ! 864400
-- The name of the accomplice is Robin