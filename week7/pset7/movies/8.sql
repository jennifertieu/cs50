-- Write a SQL query to list the names of all people who starred in Toy Story.
-- Your query should output a table with a single column for the name of each person.
-- You may assume that there is only one movie in the database with the title Toy Story.
SELECT name FROM people P INNER JOIN stars S ON S.person_id = P.id INNER JOIN movies M ON S.movie_id = M.id WHERE M.title = 'Toy Story';