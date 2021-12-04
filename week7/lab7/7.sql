-- Write a SQL query that returns the average energy of songs that are by Drake.
-- Your query should output a table with a single column and a single row containing the average energy.
-- You should not make any assumptions about what Drake’s artist_id is.
SELECT AVG(S.energy) FROM songs S INNER JOIN artists A ON S.artist_id = A.id WHERE A.name = 'Drake';