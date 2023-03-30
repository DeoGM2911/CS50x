SELECT companies.symbol, stocks.shares
FROM stocks JOIN companies 
ON companies.id = stocks.company_id
WHERE stocks.user_id = ?