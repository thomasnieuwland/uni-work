/* How to compile from the firebug directory:
	g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/register.cgi cpp/register.cpp -L/usr/lib -lmysqlcppconn -lcgicc
*/

/* Standard C++ includes */
#include <iostream>
#include <string>
#include <exception>
#include <functional>

/* Connector/C++ includes */
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

/* CGI Includes */
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

/* JSON Include */
#include "json.hpp"

void statusOutput(std::string status) {
	nlohmann::json retJ;
	retJ["status"] = status;
	std::cout << retJ << std::endl;
}

int main(void) {
	cgicc::Cgicc cgi;
	
	std::cout << cgicc::HTTPHTMLHeader() << std::endl;
	
	try {
		sql::Driver *driver;
		sql::Connection *conn;
		sql::PreparedStatement *prepStmt;
		sql::ResultSet *res;
		
		// Get POST values
		std::string displayname = cgi("displayname");
		std::string username = cgi("username");
		std::string email = cgi("email");
		std::string password = cgi("password");
		std::string passwordConfirm = cgi("password-confirm");
		std::string hashedPassword;
		
		// Check if the passwords are the same
		if(password != passwordConfirm) {
			statusOutput("password_mismatch");
			return 0;
		}
		
		// Check if the username or email already exists
		driver = get_driver_instance();
		conn = driver->connect("localhost", "web", "VerySecure");
		conn->setSchema("FIREBUG");
		
		prepStmt = conn->prepareStatement("SELECT COUNT(*) FROM user WHERE username=?");
		prepStmt->setString(1, username);
		res = prepStmt->executeQuery();
		res->next();
		if(res->getInt("COUNT(*)")) {
			statusOutput("user_exists");
			return 0;
		}
		
		prepStmt = conn->prepareStatement("SELECT COUNT(*) FROM user WHERE email=?");
		prepStmt->setString(1, email);
		res = prepStmt->executeQuery();
		
		res->next();
		if(res->getInt("COUNT(*)")) {
			statusOutput("email_registered");
			return 0;
		}

		hashedPassword = std::to_string(std::hash<std::string>{}(password));
		
		// Create the account
		prepStmt = conn->prepareStatement("INSERT INTO user (username, pass, email, displayname, isreporter) VALUES (?, ?, ?, ?, 1)");
		prepStmt->setString(1, username);
		prepStmt->setString(2, hashedPassword);
		prepStmt->setString(3, email);
		prepStmt->setString(4, displayname);
		prepStmt->executeQuery();
		
		statusOutput("success");
		
	} catch(sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
  		std::cout << " (MySQL error code: " << e.getErrorCode();
  		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	
	return 0;
}

/* POSSIBLE RETURN STATUS
 *
 * password_mistmatch	the password confirmation did not match
 * user_exists			the username is already taken
 * email_registered		the email is already registered to an account
 * error				something fucked up
 */
