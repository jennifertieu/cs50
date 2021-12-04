-- Write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
-- Your query should output a table with a single column for the name of each person.
-- There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
-- Kevin Bacon himself should not be included in the resulting list
SELECT P.name FROM people P INNER JOIN stars S ON P.id = S.person_id INNER JOIN (SELECT M.id FROM movies M INNER JOIN stars S ON M.id = S.movie_id INNER JOIN people P ON S.person_id = P.id WHERE P.name = 'Kevin Bacon' AND P.birth = 1958) M ON M.id = S.movie_id WHERE P.name <> 'Kevin Bacon';