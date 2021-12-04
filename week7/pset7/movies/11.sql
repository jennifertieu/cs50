-- Write a SQL query to list the titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated.
-- Your query should output a table with a single column for the title of each movie.
-- You may assume that there is only one person in the database with the name Chadwick Boseman.
SELECT M.title FROM movies M INNER JOIN stars S ON M.id = S.movie_id INNER JOIN people P ON P.id = S.person_id INNER JOIN ratings R ON R.movie_id = M.id WHERE P.name = 'Chadwick Boseman' ORDER BY R.rating DESC LIMIT 5;