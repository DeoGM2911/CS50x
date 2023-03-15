SELECT movies.title FROM
movies JOIN people JOIN ratings JOIN stars
ON movies.id = ratings.movie_id AND movies.id = stars.movie_id
AND people.id = stars.person_id
WHERE people.name = 'Chadwick Boseman' ORDER BY ratings.rating DESC LIMIT 5;