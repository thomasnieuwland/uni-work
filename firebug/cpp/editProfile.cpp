/*
 * Original Author: Liam Foster
 * Date of Creation: 10/10/2016
 * Filename: editProfile.cpp
 * Usage and implementation: this script allows for a user to edit their own profile or for an admin to edit a profile.
 *
 * POST inputs: id, email, displayname, isReporter, isReviewer, isTriager, isDeveloper, isAdmin, active.
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

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/editProfile.cgi cpp/editProfile.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
		std::string email = cgi("email");
		std::string displayname = cgi("displayname");
		std::string isReporter = cgi("isReporter");
		std::string isReviewer = cgi("isReviewer");
		std::string isTriager = cgi("isTriager");
		std::string isDeveloper = cgi("isDeveloper");
		std::string isAdmin = cgi("isAdmin");
		std::string active = cgi("active");

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
		prepStmt = conn->prepareStatement( "SELECT isAdmin FROM user WHERE id=?" );

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
			// User has been found, check for appropriate user privileges
			int userRole = res->getInt("isAdmin");
			if (userRole == 1 || userID == id) // Allow admins and the creator to edit bug
			{
				sql::PreparedStatement *prepStmt2;
				prepStmt2 = conn->prepareStatement( "UPDATE user SET email=?, displayname=? WHERE id=?" );
				prepStmt2->setString(1, email);
				prepStmt2->setString(2, displayname);
				prepStmt2->setString(3, id);
				prepStmt2->executeQuery();

				if(userRole == 1)
				{
					sql::PreparedStatement *prepStmt3;
					prepStmt3 = conn->prepareStatement( "UPDATE user SET isReporter=?, isReviewer=?, isTriager=?, isDeveloper=?, isAdmin=?, active=? WHERE id=?");
					prepStmt3->setString(1, isReporter);
					prepStmt3->setString(2, isReviewer);
					prepStmt3->setString(3, isTriager);
					prepStmt3->setString(4, isDeveloper);
					prepStmt3->setString(5, isAdmin);
					prepStmt3->setString(6, active);
					prepStmt3->setString(7, id);
					prepStmt3->executeQuery();

					delete prepStmt3;
				}

				retJ["status"] = "success";
				delete prepStmt2;
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
	catch(...) {
		std::cout << "Something went wrong with the SQL or the JSON formatting!\nDumping the data set:\n";
		std::cout << retJ.dump(4) << std::endl;
	}

	

	return 0;
}
