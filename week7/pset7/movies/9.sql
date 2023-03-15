SELECT people.name
FROM people JOIN
(SELECT DISTINCT(people.id) AS p_id
FROM movies JOIN people JOIN stars
ON movies.id = stars.movie_id AND people.id = stars.person_id
WHERE movies.year = 2004)
ON people.id = p_id ORDER BY people.birth;