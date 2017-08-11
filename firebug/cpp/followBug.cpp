/*
 * Original Author: Matthew Brayne
 * Date of Creation: 23/10/2016
 * Filename: followBug.cpp
 * Usage and implementation: this script allows a user with sufficient privileges to follow a bug report.
 *
 * POST inputs: bug_id
 */

/* JSON OUTPUT:

{
	"status": "success"
	"message": "following bug"
}

or...

{
	"status": "failure",
	"message": "insufficient user permissions ... other error messages go here"
}

*/

/* HOW TO COMPILE FROM THE firebug DIRECTORY (this assumes you have installed json.hpp to the /usr/include/json directory) (this also assumes that you are using g++ version 4.9 or higher):

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/followBug.cgi cpp/followBug.cpp -L/usr/lib -lmysqlcppconn -lcgicc
*/

/* Standard C++ includes */
#include <iostream>
#include <string>
#include <cstdlib>
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

/* JSON Include */
#include "json.hpp"

int main(void)
{
	cgicc::Cgicc cgi;
	nlohmann::json retJ;

	std::cout << cgicc::HTTPHTMLHeader() << std::endl;

	try {
		sql::Driver *driver;
		sql::Connection *conn;
		sql::PreparedStatement *prepStmt;
		sql::ResultSet *res;

		// Get POST values
		std::string bug_id = cgi("bug_id");

		// Retrieve cookies ***
		cgicc::const_cookie_iterator cci;
   		const cgicc::CgiEnvironment& env = cgi.getEnvironment(); // Get environment variables
		cci = env.getCookieList().begin(); // Currently Firebug only tracks one cookie (user id)
		std::string userID = cci->getValue(); // Grab user ID from cookie

		// Create the connection
		driver = get_driver_instance();
		conn = driver->connect("localhost", "web", "VerySecure");

		// Connect to the database required
		conn->setSchema("FIREBUG");

		// Use a prepared statement to prevent SQL injection
		// Selects the user with the id that matches the cookie
		prepStmt = conn->prepareStatement( "SELECT * FROM user WHERE id=?" );
		prepStmt->setString(1, userID);

		// Execute the query
		res = prepStmt->executeQuery();

		// Check if the bug exists
		sql::PreparedStatement *prepStmt69;
		sql::ResultSet *res2;
		prepStmt69 = conn->prepareStatement( "SELECT * FROM bug WHERE bug_id=?" );
		prepStmt69->setString(1, bug_id);
		res2 = prepStmt69->executeQuery();
		
		if (!res->next())
		{
			// User has not been found, build appropriate empty JSON object
			retJ["status"] = "failure";
			retJ["message"] = "insufficient user privileges";
		}
		else if (!res2->next())
		{
			// Bug has not been found, build appropriate empty JSON object
			retJ["status"] = "failure";
			retJ["message"] = "Bug_id not found";
		}
		else
		{
			// check if user is following the bug already
			sql::PreparedStatement *prepStmt666;
			sql::ResultSet *res666;
			prepStmt666 = conn->prepareStatement( "SELECT * FROM following WHERE username=? AND bug_id=?" );
			prepStmt666->setString(1, res->getString("username"));
			prepStmt666->setString(2, bug_id);

			res666 = prepStmt666->executeQuery();

			if (res666->next()) //user is already following the bug
			{
				retJ["status"] = "failure";
				retJ["message"] = "user is already following bug #" + bug_id;
				std::cout << retJ << std::endl;
				delete prepStmt666;
				delete res666;
				delete conn;
				delete prepStmt;
				delete res;
				delete res2;
				delete prepStmt69;
				return 1;
			}
			
			// Update the 'following' table, users automatically follow any bug reports that they create
			sql::PreparedStatement *prepStmt6;
			prepStmt6 = conn->prepareStatement( "INSERT INTO following (username, bug_id) VALUES (?, ?)" );
			prepStmt6->setString(1, res->getString("username"));
			prepStmt6->setString(2, bug_id);

			prepStmt6->executeQuery();
			
			delete prepStmt6;
			retJ["status"] = "success";
			retJ["message"] = "following bug";
		}

		// Return the object to the page
		std::cout << retJ << std::endl;
		delete conn;
		delete prepStmt;
		delete res;
		delete res2;
		delete prepStmt69;
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
