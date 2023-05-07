SELECT DISTINCT(people.name) FROM 
people JOIN stars JOIN (SELECT movie_id AS mv_id FROM stars
WHERE person_id = (SELECT id FROM people WHERE people.name = 'Kevin Bacon' and people.birth = 1958))
ON people.id = stars.person_id AND stars.movie_id = mv_id
WHERE people.id != (SELECT id FROM people WHERE people.name = 'Kevin Bacon' and people.birth = 1958);
