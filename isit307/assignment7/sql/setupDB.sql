DROP DATABASE IF EXISTS assignment7;
CREATE DATABASE assignment7;
USE assignment7;

CREATE TABLE shoes(
	id int NOT NULL AUTO_INCREMENT,
	brand varchar(255) NOT NULL,
	colour varchar(255) NOT NULL,
	size int NOT NULL,
	price decimal NOT NULL,
	stock int NOT NULL,
	PRIMARY KEY (ID)
);

CREATE TABLE searched(
	id int NOT NULL,
	brand varchar(255) NOT NULL,
	colour varchar(255) NOT NULL,
	size int NOT NULL,
	price decimal NOT NULL,
	stock int NOT NULL,
	keep int DEFAULT 0,
	PRIMARY KEY (ID)
);
