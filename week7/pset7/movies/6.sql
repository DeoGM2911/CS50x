SELECT ROUND(AVG(rating), 2) AS avg_rating_in_2012
FROM movies JOIN ratings ON movies.id = ratings.movie_id
WHERE movies.year = 2012;