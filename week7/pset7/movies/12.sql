-- Write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.
-- Your query should output a table with a single column for the title of each movie.
-- You may assume that there is only one person in the database with the name Johnny Depp.
-- You may assume that there is only one person in the database with the name Helena Bonham Carter.
SELECT M.title FROM (SELECT M.title, M.id FROM movies M INNER JOIN stars S ON M.id = S.movie_id INNER JOIN people P ON S.person_id = P.id WHERE P.name = 'Johnny Depp') M INNER JOIN stars S ON M.id = S.movie_id INNER JOIN people P ON P.id = S.person_id WHERE P.name = 'Helena Bonham Carter';
