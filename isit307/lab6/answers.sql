--Standard Answers
--1
SELECT * FROM shoes WHERE size = '5' AND stock > 0;

--2
SELECT * FROM shoes WHERE colour = 'Red' AND size = '10' AND stock > 0;

--3
INSERT INTO shoes(brand,colour,size,stock) VALUES ('Tom\'s','Green','13',10);

--4
UPDATE shoes SET stock = 15 WHERE ID = 36;

--5
SELECT colour,COUNT(colour) FROM shoes WHERE stock >0 GROUP BY colour;
