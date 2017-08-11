/*
 * Original Author: Matthew Brayne
 * Date of Creation: 25/10/2016
 * Filename: alterBugRep.cpp
 * Usage and implementation: this script allows a user with sufficient privileges to give +1 or -1 reputation to a bug.
 *
 * POST inputs: bug_id, vote
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

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/alterBugRep.cgi cpp/alterBugRep.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
		std::string bug_id = cgi("bug_id");
		std::string vote = cgi("vote");

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
			prepStmt2 = conn->prepareStatement( "SELECT votes FROM bug WHERE bug_id=?" );
			prepStmt2->setString(1, bug_id);
			sql::ResultSet *res2 = prepStmt2->executeQuery();
			if (!res2->next()) // Target bug has not been found. Abort with error message
			{
				retJ["status"] = "failure";
				retJ["message"] = "Target bug not found!";
				std::cout << retJ.dump(4) << std::endl;
				return 1;
			}
			
			double reputation = res2->getDouble("votes");
			if (vote == "+")
			{
				reputation += REP_INCREMENT;
			}
			else if (vote == "-")
			{
				reputation -= REP_INCREMENT;
			}
			else 
			{
				retJ["status"] = "failure";
				retJ["message"] = "insufficient input. 'votes' POST input must be either '+' or '-'";
				std::cout << retJ.dump(4) << std::endl;
				return 1;
			}

			if (res->getInt("isAdmin") == 1 || res->getInt("isReviewer") == 1 || res->getInt("isTriager") == 1) // Allow everyone with correct user permissions to add/minus reputation
			{	
				std::ostringstream strs; // Convert reputation double to string
				strs << reputation;
				std::string rep = strs.str();

				// Update the database with the new reputation value
				sql::PreparedStatement *prepStmt7;
				if (!bug_id.empty())
				{
					prepStmt7 = conn->prepareStatement( "UPDATE bug SET votes=? WHERE bug_id=?" );
					prepStmt7->setString(2, bug_id);
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
				retJ["message"] = "insufficient user privileges (you need to be an admin or reviewer or triager to use this function)";
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
