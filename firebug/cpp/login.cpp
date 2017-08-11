/*
 * Original Author: Matthew Brayne
 * Date of Creation: 9/10/2016
 * Filename: login.cpp
 * Usage and implementation: this script searches the database for a user with a matching username and password. If the login is successful, the script redirects the user to the dashboard, also sends a cookie to the web page containing the user id, which can later be used to check user permissions
 */

/* HOW TO COMPILE FROM THE firebug DIRECTORY (this assumes you have installed json.hpp to the /usr/include/json directory) (this also assumes that you are using g++ version 4.9 or higher):

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/login.cgi cpp/login.cpp -L/usr/lib -lmysqlcppconn -lcgicc
*/

/* Standard C++ includes */
#include <iostream>
#include <string>
#include <ctime>
#include <sstream>

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
#include "cgicc/HTTPRedirectHeader.h"

/* JSON Include */
#include "json.hpp"

int main(void)
{
	cgicc::Cgicc cgi;
	nlohmann::json retJ;

	try {
		sql::Driver *driver;
		sql::Connection *conn;
		sql::PreparedStatement *prepStmt;
		sql::ResultSet *res;

		// Get POST values
		std::string username = cgi("username");
		std::string password = cgi("password");
		std::string hashedPassword = std::to_string(std::hash<std::string>{}(password));

		// Create the connection
		driver = get_driver_instance();
		conn = driver->connect("localhost", "web", "VerySecure");

		// Connect to the database required
		conn->setSchema("FIREBUG");

		// Use a prepared statement to prevent SQL injection
		// Counts the number of matching records. 0 = not found, 1 = authenticated
		prepStmt = conn->prepareStatement("SELECT id FROM user WHERE username=? AND pass=?");

		// Apply the variables from POST and execute
		prepStmt->setString(1, username);
		prepStmt->setString(2, hashedPassword);
		res = prepStmt->executeQuery();	

		// If there is something in the dataset, the login is successful
		if(res->next()) {
			
		// If the user's reputation is of a certain level, they become a reviewer
		prepStmt = conn->prepareStatement("UPDATE user SET isReviewer = 1 WHERE username=? AND reputation > 100");
		prepStmt->setString(1, username);
		prepStmt->executeQuery();

		//If a user's rep is too low, they lose reviewer status
		prepStmt = conn->prepareStatement("UPDATE user SET isReviewer = 0 WHERE username=? AND reputation < 100");
		prepStmt->setString(1, username);
		prepStmt->executeQuery();	
		
		// Redirect user to dashboard page with cookie (login has succeeded)
		std::cout << cgicc::HTTPRedirectHeader("/dashboard").setCookie(cgicc::HTTPCookie("id", res->getString("id"), "This cookie is used for checking user privileges", "", 31540000, "/", false)); //create a cookie that contains the user id, 365 day expiry time, and is not encrypted

		}
		else {
			// Redirect user to login page without cookie (login has failed)
			std::cout << cgicc::HTTPRedirectHeader("../login") << std::endl;
		}
	}
	catch(sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
  		std::cout << " (MySQL error code: " << e.getErrorCode();
  		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}

	return 0;
}
