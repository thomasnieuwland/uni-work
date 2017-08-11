/*
 * Original Author: Matthew Brayne
 * Date of Creation: 8/10/2016
 * Filename: giveReputation.cpp
 * Usage and implementation: this script allows a user with sufficient privileges to give +1 reputation to another user. It reads in the id of the user to add reputation to. This could act like a "+1" or "like" button on a user's profile, or perhaps next to their name no a bug report/comment/attachment.
 *
 * POST inputs: id, username, displayname
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

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/giveReputation.cgi cpp/giveReputation.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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

	const double REP_INCREMENT = 1;

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
		std::string userID = cci->getValue(); // Grab user ID from cookie

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
			retJ["message"] = "insufficient user privileges";
		}
		else
		{
			// User has been found, check for appropriate user privileges
			
			// Get target user reputation
			sql::PreparedStatement *prepStmt2;
			prepStmt2 = conn->prepareStatement( "SELECT reputation FROM user WHERE id=? OR username=? OR displayname=?" );
			prepStmt2->setString(1, id);
			prepStmt2->setString(2, id);
			prepStmt2->setString(3, id);
			sql::ResultSet *res2 = prepStmt2->executeQuery();
			if (!res2->next()) // Target user has not been found. Abort with error message
			{
				retJ["status"] = "failure";
				retJ["message"] = "Target user not found!";
				std::cout << retJ.dump(4) << std::endl;
				return 1;
			}
			
			double reputation = res2->getDouble("reputation");
			reputation += REP_INCREMENT;
			//std::cout << reputation << std::endl;
			if (res->getInt("isAdmin") == 1 || res->getInt("isReviewer") == 1) // Allow everyone with correct user permissions to add reputation
			{	
				std::ostringstream strs; // Convert reputation double to string
				strs << reputation;
				std::string rep = strs.str();

				// Update the database with the new reputation value
				sql::PreparedStatement *prepStmt7;
				if (!id.empty())
				{
					prepStmt7 = conn->prepareStatement( "UPDATE user SET reputation=? WHERE id=? OR username=? OR displayname=?" );
					prepStmt7->setString(2, id);
					prepStmt7->setString(3, id);
					prepStmt7->setString(4, id);
					prepStmt7->setString(1, rep);
					prepStmt7->executeQuery();
				}
				else
				{
					retJ["status"] = "failure";
					retJ["message"] = "insufficient input";
					std::cout << retJ.dump(4) << std::endl;
					return 1;
				}
				delete prepStmt7;
		
				retJ["status"] = "success";
			}
			else
			{
				retJ["status"] = "failure";
				retJ["message"] = "insufficient user privileges";
			}
			
			delete prepStmt2;
			delete res2;
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
