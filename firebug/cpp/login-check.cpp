/*
 * Original Author: Ben Jervis
 * Date of Creation: ~4/10/2016
 * Filename: login-check.cpp
 * Usage and implementation: this script searches the database for a user with a matching username and password and returns a JSON object specifying wether or not the login was successful.
 */

/* JSON FORMAT:

{
	"status": "VALID",
}

or...

{
	"status": "INVALID"
}

or...

{
	"status": "ERROR"
}

*/

/* HOW TO COMPILE FROM THE firebug DIRECTORY (this assumes you have installed json.hpp to the /usr/include/json directory) (this also assumes that you are using g++ version 4.9 or higher):

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/login-check.cgi cpp/login-check.cpp -L/usr/lib -lmysqlcppconn -lcgicc
*/

/* Standard C++ includes */
#include <iostream>
#include <string>
#include <exception>

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

int main(void)
{
	cgicc::Cgicc cgi;
	nlohmann::json retJ;

	// Output content headers to the web page
	std::cout << cgicc::HTTPHTMLHeader() << std::endl;

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

		//res->next();

		// If the count is 1, return the VALID identifier
		if(!res->next()) {
			retJ["status"] = "INVALID";
		}
		else {
			retJ["status"] = "VALID";
		}

		// Return the status to the page
		std::cout << retJ << std::endl;
	}
	catch(std::exception& e) {
		retJ["status"] = "error";
		retJ["message"] = e.what();
		std::cout << retJ << std::endl;
	}

	return 0;
}
