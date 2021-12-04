-- Write a SQL query that lists the names of songs that are by Post Malone.
-- Your query should output a table with a single column for the name of each song.
-- You should not make any assumptions about what Post Malone’s artist_id is.
SELECT S.name FROM songs S INNER JOIN artists A ON S.artist_id = A.id WHERE A.name = 'Post Malone';