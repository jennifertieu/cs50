-- Write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.
-- Your query should output a table with a single column for the name of each person.
SELECT P.name FROM people P INNER JOIN directors D ON P.id = D.person_id INNER JOIN movies M ON M.id = D.movie_id INNER JOIN ratings R ON R.movie_id = M.id WHERE R.rating >= 9;