/*
 * Original Author: Matthew Brayne
 * Date of Creation: 11/10/2016
 * Filename: createComment.cpp
 * Usage and implementation: this script takes a bug id as input and allows a user with sufficient privileges to comment on a bug report. The script checks user permission by reading a user id from the cookie. If the cookie is empty, or the id does not exist in the database (ie: deleted user), then the script will not let the user create a comment. The script returns a success or failure message formatted as JSON.
 *
 * POST inputs: id (bug), comment
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

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/createComment.cgi cpp/createComment.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
		std::string id = cgi("bug_id");
		std::string comment = cgi("thetext");

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

		// Check if the bug exists
		sql::PreparedStatement *prepStmt69;
		sql::ResultSet *res69;
		prepStmt69 = conn->prepareStatement( "SELECT * FROM bug WHERE bug_id=?" );
		prepStmt69->setString(1, id);
		res69 = prepStmt69->executeQuery();
		
		if (!res->next())
		{
			// User has not been found, build appropriate empty JSON object
			retJ["status"] = "failure";
			retJ["message"] = "insufficient user privileges";
		}
		else if (!res69->next())
		{
			// Bug has not been found, build appropriate empty JSON object
			retJ["status"] = "failure";
			retJ["message"] = "bug not found";
		}
		else
		{
			// User has been found, check for appropriate user privileges
			if (res->getInt("isAdmin") == 1 || res->getInt("isReporter") == 1 || res->getInt("isTriager") == 1 || res->getInt("isReviewer") == 1 || res->getInt("isDeveloper") == 1) // Allow everyone with correct user permissions to create comment
			{
				sql::PreparedStatement *prepStmt3;
				// Add the new comment to the database
				prepStmt3 = conn->prepareStatement( "INSERT INTO comments (who, bug_when, bug_id, thetext) VALUES (?, NOW(), ?, ?)" );
				prepStmt3->setString(1, res->getString("displayname"));
				prepStmt3->setString(2, id);
				prepStmt3->setString(3, comment);
				prepStmt3->executeQuery();

				// Check if the user is already following this bug
                sql::PreparedStatement *prepStmtFollowCheck;
                sql::ResultSet *resFollowCheck;
                prepStmtFollowCheck = conn->prepareStatement( "SELECT COUNT(*) AS numRecs FROM following WHERE bug_id=? AND username=?" );
                prepStmtFollowCheck->setString(1, id);
                prepStmtFollowCheck->setString(2, res->getString("username"));
                resFollowCheck = prepStmtFollowCheck->executeQuery();
				resFollowCheck->next();

                if (resFollowCheck->getInt("numRecs") == 0)
                {
                    // Update the 'following' table, users automatically follow any bug reports that they comment on
                    sql::PreparedStatement *prepStmt6;
                    prepStmt6 = conn->prepareStatement( "INSERT INTO following (username, bug_id) VALUES (?, ?)" );
                    prepStmt6->setString(1, res->getString("username"));
                    prepStmt6->setString(2, id);

                    prepStmt6->executeQuery();

                    delete prepStmt6;
                }

                delete resFollowCheck;
                delete prepStmtFollowCheck;

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
		delete res;
		delete prepStmt69;
		delete res69;
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
