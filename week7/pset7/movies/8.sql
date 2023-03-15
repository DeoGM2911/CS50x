SELECT people.name
FROM movies JOIN people JOIN stars
ON movies.id = stars.movie_id AND people.id = stars.person_id
WHERE movies.title = 'Toy Story';