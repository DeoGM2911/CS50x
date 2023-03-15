SELECT people.name FROM
people JOIN (SELECT DISTINCT(people.id) AS p_id FROM
            movies JOIN directors JOIN people JOIN ratings ON
            movies.id = directors.movie_id AND people.id = directors.person_id AND movies.id = ratings.movie_id
            WHERE ratings.rating >= 9.0) ON p_id = people.id;
