/*
    THESE COLUMN NAMES ARE FINAL, AS THESE ARE WHAT IS REQUIRED TO IMPORT THE
    DATA FOUND IN THE XML FILE GIVEN IN THE SPEC

    PLEASE NOTE THIS SCRIPT CAN TAKE MULTIPLE MINUTES TO EXECUTE DEPENDING ON XML
    FILE SIZE
*/

/* BUG TABLE CREATION */

DROP TABLE IF EXISTS `bug`;
CREATE TABLE `bug` (
`bug_id` INT NOT NULL AUTO_INCREMENT,
`creation_ts` TIMESTAMP NULL,
`short_desc` LONGTEXT NULL,
`classification` VARCHAR(100) NULL,
`product` VARCHAR(100) NULL,
`component` VARCHAR(100) NULL,
`version` VARCHAR(100) NULL,
`op_sys` VARCHAR(100) NULL,
`bug_status` VARCHAR(100) NULL,
`resolution` VARCHAR(100) NULL,
`bug_severity` VARCHAR(100) NULL,
`votes` INT NULL,
`reporter` VARCHAR(100) NULL,
`assigned_to` VARCHAR(100) DEFAULT 'nobody',
PRIMARY KEY (`bug_id`),
UNIQUE INDEX `bug_id_UNIQUE` (`bug_id` ASC));


/*COMMENTS TABLE CREATION*/
DROP TABLE IF EXISTS `comments`;
CREATE TABLE `comments` (
`bug_id` INT NOT NULL,
`commentid` INT NOT NULL AUTO_INCREMENT,
`attachid` INT NULL,
`who` VARCHAR(100) NULL,
`bug_when` TIMESTAMP NULL,
`thetext` LONGTEXT NULL,
PRIMARY KEY (`commentid`),
UNIQUE INDEX `commentid_UNIQUE` (`commentid` ASC));


/* ATTACHMENTS TABLE CREATION */
DROP TABLE IF EXISTS `attachments`;
CREATE TABLE `attachments` (
`bug_id` INT NOT NULL,
`attachid` INT NOT NULL AUTO_INCREMENT,
`date` TIMESTAMP NULL,
`isobsolete` VARCHAR(45) NULL,
`ispatch` VARCHAR(45) NULL,
`desc` LONGTEXT NULL,
`filename` VARCHAR(100) NULL,
`type` VARCHAR(100) NULL,
`size` INT NULL,
`attacher` VARCHAR(100) NULL,
PRIMARY KEY (`attachid`),
UNIQUE INDEX `attachid_UNIQUE` (`attachid` ASC));


/* VOTES TABLE CREATION */
DROP TABLE IF EXISTS `votes`;
CREATE TABLE `votes` (
`bug_id` INT NOT NULL,
`id` VARCHAR(45) NOT NULL,
`setter` VARCHAR(100) NULL,
`status` VARCHAR(45) NOT NULL,
UNIQUE INDEX `id_UNIQUE` (`id` ASC),
PRIMARY KEY (`id`));

/* LOADING XML */
LOAD XML LOCAL INFILE 'BugReports.xml'
INTO TABLE bug
ROWS IDENTIFIED BY '<bug>';

LOAD XML LOCAL INFILE 'BugReports.xml'
INTO TABLE comments
ROWS IDENTIFIED BY '<long_desc>';

LOAD XML LOCAL INFILE 'BugReports.xml'
INTO TABLE attachments
ROWS IDENTIFIED BY '<attachment>';

LOAD XML LOCAL INFILE 'BugReports.xml'
INTO TABLE votes
ROWS IDENTIFIED BY '<flag>';
/*
    Below is the part of the script which is used to import which user is
    following what bug. Because this is done using 2 different xml elements,
    a temporary table must be used
*/

DROP TABLE IF EXISTS `following`;
CREATE TABLE `following` (
`bug_id` INT NOT NULL,
`username` VARCHAR(100) NOT NULL,
	PRIMARY KEY (`bug_id`, `username`)
);

DROP TABLE IF EXISTS `followTemp`;
CREATE TABLE `followTemp` (
`bug_id` INT NOT NULL,
`cc` VARCHAR(45) NULL);

INSERT INTO following(bug_id,username)
SELECT bug_id,cc FROM followTemp;

DROP TABLE `followTemp`;

INSERT INTO following(bug_id,username)
SELECT bug_id,assigned_to FROM bug;

/*
    Below is the section of the script for importing users. Since there are
    users stored under multiple xml definitions, multiple tables have to be
    created to import each defintion. Expect warnings when using this part of
    the script
*/

/* MAIN TABLE CREATED */
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
`id` INT NOT NULL AUTO_INCREMENT,
`username` VARCHAR(255) NOT NULL,
`displayname` VARCHAR(255) NULL,
`email` VARCHAR(255) NOT NULL DEFAULT 'unspecified',
`pass` VARCHAR(255) NOT NULL DEFAULT '6072375419398818283', /* 'password' in c++ hash */
`isReporter` INT NOT NULL DEFAULT 0,
`isReviewer` INT NOT NULL DEFAULT 0,
`isTriager` INT NOT NULL DEFAULT 0,
`isDeveloper` INT NOT NULL DEFAULT 0,
`isAdmin` INT NOT NULL DEFAULT 0,
`reputation` INT NULL DEFAULT 0,
`active` INT NULL DEFAULT 1,
`sessionID` VARCHAR(255) NULL,
PRIMARY KEY (`id`),
UNIQUE INDEX `username_UNIQUE` (`username` ASC),
UNIQUE INDEX `displayname_UNIQUE` (`displayname` ASC));

/* WHO TEMP TABLE CREATED */

DROP TABLE IF EXISTS `tempUsers`;
CREATE TABLE `tempUsers` (
`who` VARCHAR(255) NOT NULL,
`name` VARCHAR(255) NULL,
PRIMARY KEY (`who`),
UNIQUE INDEX `who_UNIQUE` (`who` ASC),
UNIQUE INDEX `name_UNIQUE` (`name` ASC));

LOAD XML LOCAL INFILE 'BugReports.xml'
INTO TABLE tempUsers
ROWS IDENTIFIED BY '<who>';

INSERT INTO user(username,displayname)
SELECT who,name FROM tempUsers;

DROP TABLE tempUsers;

/* All users given displaynames */
UPDATE user SET displayname = username WHERE displayname IS NULL;

/* Roles are discovered and assigned */
SET SQL_SAFE_UPDATES = 0;
UPDATE user AS u
INNER JOIN votes AS v ON u.username = v.setter
SET u.isReviewer = 1
WHERE v.bug_id > 0;

UPDATE user AS u
INNER JOIN bug AS b ON u.username = b.reporter
SET u.isReporter = 1
WHERE b.bug_id > 0;

UPDATE user AS u
INNER JOIN attachments AS a ON u.username = a.attacher
SET u.isDeveloper = 1
WHERE a.ispatch = 1;
