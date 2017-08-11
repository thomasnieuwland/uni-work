/*
 * Original Author: Matthew Brayne
 * Date of Creation: 8/10/2016
 * Filename: editBug.cpp
 * Usage and implementation: this script searches the database for a bug with a specific ID, then updates the database information for that bug based on the POST inputs. This script also checks the user's cookie. So if you have already logged in and generated a cookie, you will be checked! If your user ID is the same as the owner of the bug, you will be able to edit the bug successfully! (also if you'e an admin). If not, the script will return an error message formatted as JSON.
 *
 * POST inputs: bug_id, short_desc, classification, product, component, version, op_sys, bug_severity.
 */

/* JSON OUTPUT:

{
	"status": "success"
}

or...

{
	"status": "failure",
	"message": "insufficient user permissions ... other error messages go here"
}

*/

/* HOW TO COMPILE FROM THE firebug DIRECTORY (this assumes you have installed json.hpp to the /usr/include/json directory) (this also assumes that you are using g++ version 4.9 or higher):

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/editBug.cgi cpp/editBug.cpp -L/usr/lib -lmysqlcppconn -lcgicc
*/

/* Standard C++ includes */
#include <iostream>
#include <string>

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
		sql::PreparedStatement *prepStmt2;
		sql::ResultSet *res;
		sql::ResultSet *res2;

		// Get POST values
		std::string id = cgi("bug_id");
		std::string short_desc = cgi("short_desc");
		std::string classification = cgi("classification");
		std::string product = cgi("product");
		std::string component = cgi("component");
		std::string version = cgi("version");
		std::string op_sys = cgi("op_sys");
		std::string severity = cgi("bug_severity");

		// Retrieve cookies ***
		cgicc::const_cookie_iterator cci;
   		const cgicc::CgiEnvironment& env = cgi.getEnvironment(); // Get environment variables
		cci = env.getCookieList().begin(); // Currently Firebug only tracks one cookie (user id)
		std::string userID = cci->getValue(); // Now we need to use the ID to check the user's permissions

		// Create the connection
		driver = get_driver_instance();
		conn = driver->connect("localhost", "web", "VerySecure");

		// Connect to the database required
		conn->setSchema("FIREBUG");

		// Use a prepared statement to prevent SQL injection
		// Selects the user with the id that matches the cookie
		prepStmt = conn->prepareStatement( "SELECT * FROM user WHERE id=?" );

		// Apply the variables from POST and execute
		prepStmt->setString(1, userID);

		// Execute the query
		res = prepStmt->executeQuery();

		// Selects the id of the user who created the bug
		prepStmt2 = conn->prepareStatement( "SELECT reporter FROM bug WHERE bug_id=?" );
		prepStmt2->setString(1, id);
		res2 = prepStmt2->executeQuery();
		res2->next();
		std::string ownerID = res2->getString("reporter"); // Owner ID is needed for checking because the creator of the bug report should be able to edit it
		
		if (!res->next())
		{
			// User has not been found, build appropriate empty JSON object
			retJ["status"] = "failure";
			retJ["message"] = "insufficient user privileges or cookie index not working?";
		}
		else
		{
			// User has been found, check for appropriate user privileges

			if (res->getInt("isAdmin") == 1 || userID == ownerID) // Allow admins and the creator to edit bug
			{
				sql::PreparedStatement *prepStmt3;
				prepStmt3 = conn->prepareStatement( "UPDATE bug SET short_desc=?, classification=?, product=?, component=?, version=?, op_sys=?, bug_severity=? WHERE bug_id=?" );
				prepStmt3->setString(1, short_desc);
				prepStmt3->setString(2, classification);
				prepStmt3->setString(3, product);
				prepStmt3->setString(4, component);
				prepStmt3->setString(5, version);
				prepStmt3->setString(6, op_sys);
				prepStmt3->setString(7, severity);
				prepStmt3->setString(8, id);
				prepStmt3->executeQuery();

				retJ["status"] = "success";
				delete prepStmt3;
			}
			else
			{
				retJ["status"] = "failure";
				retJ["message"] = "insufficient user privileges";
			}
		}

		// Return the object to the page
		std::cout << retJ << std::endl;
		delete conn;
		delete prepStmt;
		delete prepStmt2;
		delete res;
		delete res2;
	}
	catch(...) {
		std::cout << "Something went wrong with the SQL or the JSON formatting!\nDumping the data set:\n";
		std::cout << retJ.dump(4) << std::endl;
	}

	

	return 0;
}
