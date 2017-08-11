/*
 * Original Author: Matthew Brayne
 * Date of Creation: 13/10/2016
 * Filename: banUser.cpp
 * Usage and implementation: this script allows an admin to ban a user. It takes the id of the user to be banned as a POST input.
 *
 * POST inputs: id (user to be banned)
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

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/banUser.cgi cpp/banUser.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
		sql::ResultSet *res;

		// Get POST values
		std::string id = cgi("id");

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

		if (!res->next())
		{
			// User has not been found, build appropriate empty JSON object
			retJ["status"] = "failure";
			retJ["message"] = "insufficient user privileges or cookie index not working?";
		}
		else
		{
			// Cookie user has been found, check for appropriate user privileges
			if (res->getInt("isAdmin") == 1 && (userID != id && res->getString("username") != id && res->getString("displayname") != id)) // Allow admins to ban the user. Also make sure the admin can't ban themself on accident
			{
				// Make sure the user exists in the database:
				sql::ResultSet *res2;
				sql::PreparedStatement * prepStmt3;
				prepStmt3 = conn->prepareStatement( "SELECT id FROM user WHERE id=? OR username=? OR displayname=?" );
				prepStmt3->setString(1, id);	
				prepStmt3->setString(2, id);	
				prepStmt3->setString(3, id);	
				res2 = prepStmt3->executeQuery();

				
				if (!res2->next()) // User has not been found in the database
				{
					retJ["status"] = "failure";
					retJ["message"] = "User " + id + " is not found in the database.";
				}
				else
				{
					int uid = res2->getInt("id");
					sql::PreparedStatement *prepStmt2;
					prepStmt2 = conn->prepareStatement( "UPDATE user SET active = 0 WHERE id=?" );
					prepStmt2->setInt(1, uid);
					prepStmt2->executeQuery();

					retJ["status"] = "success";
					delete prepStmt2;
				}
				
				delete prepStmt3;
				delete res2;
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
		delete res;
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
