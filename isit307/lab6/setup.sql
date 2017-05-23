DROP DATABASE IF EXISTS lab6;
CREATE DATABASE lab6;
USE lab6;

CREATE TABLE shoes(
	id int NOT NULL AUTO_INCREMENT,
	brand varchar(255) NOT NULL,
	colour varchar(255) NOT NULL,
	size varchar(255) NOT NULL,
	stock int NOT NULL,
	PRIMARY KEY (ID)
);
