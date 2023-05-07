SELECT movies.title FROM
movies JOIN stars JOIN (SELECT movies.id AS mv FROM
movies JOIN people JOIN stars
ON movies.id = stars.movie_id AND people.id = stars.person_id
WHERE people.name = 'Johnny Depp') 
ON movies.id = stars.movie_id AND movies.id = mv
WHERE stars.person_id = (SELECT people.id FROM people WHERE people.name = 'Helena Bonham Carter');