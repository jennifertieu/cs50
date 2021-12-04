-- Keep a log of any SQL queries you execute as you solve the mystery.
-- All you know is that the theft took place on July 28, 2020 and that it took place on Chamberlin Street.

-- Tables list
/*
airports                  crime_scene_reports       people
atm_transactions          flights                   phone_calls
bank_accounts             interviews
courthouse_security_logs  passengers
*/

-- Query the crime_scene_reports table for information on the theft that took place on Chamberlin Street, July 28, 2020
-- Results: Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted today
-- with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.
SELECT description
FROM crime_scene_reports
WHERE month = 7 AND day = 28 AND year = 2020 AND street = 'Chamberlin Street';

-- Query interview table to see what the witnesses said
/* Results: name | transcript
Jose | “Ah,” said he, “I forgot that I had not seen you for some weeks. It is a little souvenir from the King of Bohemia in return for my assistance in the case of the Irene Adler papers.”
Eugene | “I suppose,” said Holmes, “that when Mr. Windibank came back from France he was very annoyed at your having gone to the ball.”
Barbara | “You had my note?” he asked with a deep harsh voice and a strongly marked German accent. “I told you that I would call.” He looked from one to the other of us, as if uncertain which to address.
Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot,
you might want to look for cars that left the parking lot in that time frame.
Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest
flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
*/
SELECT name, transcript
FROM interviews
WHERE month = 7 AND day = 28 AND year = 2020;

-- Query the courthouse_security_logstable for information on the scene of the crime, within 10 minutes of the theft to look for cars that left the parking lot in that time frame
/* Results:
exit | 5P2BI95 | 10 | 16
exit | 94KL13X | 10 | 18
exit | 6P58WS2 | 10 | 18
exit | 4328GD8 | 10 | 19
exit | G412CB7 | 10 | 20
exit | L93JTIZ | 10 | 21
exit | 322W7JE | 10 | 23
exit | 0NTHK55 | 10 | 23
*/
SELECT activity, license_plate, hour, minute
FROM courthouse_security_logs
WHERE month = 7 AND day = 28 AND year = 2020 AND hour = 10 AND 5 <= minute AND minute <= 25 AND activity = 'exit';

-- Query the atm_transactions table for the person withdrawing money on Fifer Street
/* Results:
28500762 | Fifer Street | withdraw | 48
28296815 | Fifer Street | withdraw | 20
76054385 | Fifer Street | withdraw | 60
49610011 | Fifer Street | withdraw | 50
16153065 | Fifer Street | withdraw | 80
25506511 | Fifer Street | withdraw | 20
81061156 | Fifer Street | withdraw | 30
26013199 | Fifer Street | withdraw | 35
*/
SELECT account_number, atm_location, transaction_type, amount
FROM atm_transactions
WHERE month = 7 AND day = 28 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw';

-- Query people, atm_transaction, and bank account table to determine who used the ATM on the day of the theft and also left the courthouse
/* Results:
id | name | phone_number | passport_number | license_plate
396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X
*/
SELECT *
FROM people
WHERE id IN (
SELECT person_id
FROM bank_accounts
WHERE account_number IN (SELECT account_number
FROM atm_transactions
WHERE month = 7 AND day = 28 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw')
AND license_plate IN (SELECT license_plate
FROM courthouse_security_logs
WHERE month = 7 AND day = 28 AND year = 2020 AND hour = 10 AND 5 <= minute AND minute <= 25 AND activity = 'exit'));

-- Query phone calls table to see time of calls for a short duration because witness said they called someone who talked to them for less than a minute
/* Results:
id | caller | receiver | year | month | day | duration
224 | (499) 555-9472 | (892) 555-8872 | 2020 | 7 | 28 | 36
261 | (031) 555-6622 | (910) 555-3251 | 2020 | 7 | 28 | 38
254 | (286) 555-6063 | (676) 555-6554 | 2020 | 7 | 28 | 43
233 | (367) 555-5533 | (375) 555-8161 | 2020 | 7 | 28 | 45
255 | (770) 555-1861 | (725) 555-3243 | 2020 | 7 | 28 | 49
*/
SELECT *
FROM phone_calls
WHERE month = 7 AND day = 28 AND year = 2020 ORDER BY duration ASC LIMIT 5;

-- Query for callers
/* Results
id | name | phone_number | passport_number | license_plate
514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X
*/
SELECT *
FROM people
WHERE id IN (
SELECT person_id
FROM bank_accounts
WHERE account_number IN (SELECT account_number
FROM atm_transactions
WHERE month = 7 AND day = 28 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw')
AND license_plate IN (SELECT license_plate
FROM courthouse_security_logs
WHERE month = 7 AND day = 28 AND year = 2020 AND hour = 10 AND 5 <= minute AND minute <= 25 AND activity = 'exit'))
AND phone_number IN (SELECT caller
FROM phone_calls
WHERE month = 7 AND day = 28 AND year = 2020 ORDER BY duration ASC LIMIT 5);

-- Query for receivers
/* Results
id | name | phone_number | passport_number | license_plate
847116 | Philip | (725) 555-3243 | 3391710505 | GW362R6
864400 | Berthold | (375) 555-8161 |  | 4V16VO0
*/
SELECT *
FROM people
WHERE phone_number IN (
SELECT receiver
FROM phone_calls
WHERE month = 7 AND day = 28 AND year = 2020 AND caller IN (
SELECT phone_number
FROM people
WHERE id IN (
SELECT person_id
FROM bank_accounts
WHERE account_number IN (SELECT account_number
FROM atm_transactions
WHERE month = 7 AND day = 28 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw')
AND license_plate IN (SELECT license_plate
FROM courthouse_security_logs
WHERE month = 7 AND day = 28 AND year = 2020 AND hour = 10 AND 5 <= minute AND minute <= 25 AND activity = 'exit'))
AND phone_number IN (SELECT caller
FROM phone_calls
WHERE month = 7 AND day = 28 AND year = 2020 ORDER BY duration ASC LIMIT 5)) AND id IN (SELECT id
FROM phone_calls
WHERE month = 7 AND day = 28 AND year = 2020 ORDER BY duration ASC LIMIT 5));



-- Query flights table to find the earlier flight out of Fiftyville tomorrow
/* Results:
36 | 8 | 4 | Fiftyville Regional Airport | Fiftyville | 8 | 20
*/
SELECT F.id, F.origin_airport_id, F.destination_airport_id, A.full_name, A.city, F.hour, F.minute
FROM flights F
INNER JOIN airports A
ON F.origin_airport_id = A.id
WHERE F.month = 7 AND F.day = 29 AND F.year = 2020 AND A.city = 'Fiftyville' ORDER BY hour, minute ASC LIMIT 1;

-- Query flights table to find the desitination of the earlier flight out of Fiftyville
/*  Results:
full_name | city
Heathrow Airport | London
*/
SELECT A.full_name, A.city
FROM airports A
WHERE A.id IN (SELECT F.destination_airport_id
FROM flights F
INNER JOIN airports A
ON F.origin_airport_id = A.id
WHERE F.month = 7 AND F.day = 29 AND F.year = 2020 AND A.city = 'Fiftyville' ORDER BY hour, minute ASC LIMIT 1);

-- Query passengers table to get a list of people on the flight
/* Results
flight_id | passport_number | seat
36 | 7214083635 | 2A
36 | 1695452385 | 3B
36 | 5773159633 | 4A
36 | 1540955065 | 5C
36 | 8294398571 | 6C
36 | 1988161715 | 6D
36 | 9878712108 | 7A
36 | 8496433585 | 7B
*/
SELECT *
FROM passengers
WHERE flight_id = (SELECT F.id
FROM flights F
INNER JOIN airports A
ON F.origin_airport_id = A.id
WHERE F.month = 7 AND F.day = 29 AND F.year = 2020 AND A.city = 'Fiftyville' ORDER BY hour, minute ASC LIMIT 1);

-- Now cross-reference passport_number with the results from the bank_account and atm_transactions query results to find the theif
/* Results:
id | name | phone_number | passport_number | license_plate
686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X
*/
SELECT *
FROM people
WHERE id IN (
SELECT person_id
FROM bank_accounts
WHERE account_number IN (SELECT account_number
FROM atm_transactions
WHERE month = 7 AND day = 28 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw')
AND license_plate IN (SELECT license_plate
FROM courthouse_security_logs
WHERE month = 7 AND day = 28 AND year = 2020 AND hour = 10 AND 5 <= minute AND minute <= 25 AND activity = 'exit')
AND phone_number IN (SELECT caller
FROM phone_calls
WHERE month = 7 AND day = 28 AND year = 2020 ORDER BY duration ASC LIMIT 5) AND passport_number IN (SELECT passport_number
FROM passengers
WHERE flight_id = (SELECT F.id
FROM flights F
INNER JOIN airports A
ON F.origin_airport_id = A.id
WHERE F.month = 7 AND F.day = 29 AND F.year = 2020 AND A.city = 'Fiftyville' ORDER BY hour, minute ASC LIMIT 1)));

-- Query for accomplice
/* Results
id | name | phone_number | passport_number | license_plate
864400 | Berthold | (375) 555-8161 |  | 4V16VO0
*/
SELECT *
FROM people
WHERE phone_number IN (
SELECT receiver
FROM phone_calls
WHERE month = 7 AND day = 28 AND year = 2020 AND caller IN (
SELECT phone_number
FROM people
WHERE id IN (
SELECT person_id
FROM bank_accounts
WHERE account_number IN (SELECT account_number
FROM atm_transactions
WHERE month = 7 AND day = 28 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw')
AND license_plate IN (SELECT license_plate
FROM courthouse_security_logs
WHERE month = 7 AND day = 28 AND year = 2020 AND hour = 10 AND 5 <= minute AND minute <= 25 AND activity = 'exit')
AND phone_number IN (SELECT caller
FROM phone_calls
WHERE month = 7 AND day = 28 AND year = 2020 ORDER BY duration ASC LIMIT 5) AND passport_number IN (SELECT passport_number
FROM passengers
WHERE flight_id = (SELECT F.id
FROM flights F
INNER JOIN airports A
ON F.origin_airport_id = A.id
WHERE F.month = 7 AND F.day = 29 AND F.year = 2020 AND A.city = 'Fiftyville' ORDER BY hour, minute ASC LIMIT 1)))) AND id IN (SELECT id
FROM phone_calls
WHERE month = 7 AND day = 28 AND year = 2020 ORDER BY duration ASC LIMIT 5));