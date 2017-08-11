/*
 * Original Author: Liam Foster
 * Date of Creation: 16/10/2016
 * Filename: deleteReport.cpp
 * Usage and implementation: this script allows the creater of an report or an admin to delete the report and all attachments.
 * POST inputs: bug_id
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

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/deleteReport.cgi cpp/deleteReport.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
		std::string id = cgi("bug_id");

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

		sql::PreparedStatement *prepStmt2;
		sql::ResultSet *res2;

		prepStmt2 = conn->prepareStatement( "SELECT reporter FROM bug WHERE bug_id=?" );

		prepStmt2->setString(1, id);

		res2 = prepStmt2->executeQuery();

		sql::PreparedStatement *prepStmt3;
		sql::ResultSet *res3;

		prepStmt3 = conn->prepareStatement( "SELECT * FROM user WHERE id=?" );

		prepStmt3->setString(1, userID);

		res3 = prepStmt3->executeQuery();
		

		if (!res->next())
		{
			// User has not been found, build appropriate empty JSON object
			retJ["status"] = "failure";
			retJ["message"] = "insufficient user privileges or cookie index not working?";
		}
		else
		{
			// User has been found, check for appropriate user privileges
			if (!res2->next())
			{
				// Bug doesn't exist in system. Build appropriate JSON error message
				retJ["status"] = "failure";
				retJ["message"] = "Bug doesn't exist in the database";
				std::cout << retJ << std::endl;
				return 1;
			}

			std::string attachmentOwner = res2->getString("reporter");
			std::string userDisplayName = res->getString("displayname");
			std::string userUserName = res->getString("username");
			if (res->getInt("isAdmin") == 1 || attachmentOwner == userDisplayName || attachmentOwner == userUserName) // Allow admins and the creator to edit bug
			{
				sql::PreparedStatement *prepStmt4;
				prepStmt4 = conn->prepareStatement( "DELETE FROM attachments WHERE bug_id=?" );
				prepStmt4->setString(1, id);
				prepStmt4->executeQuery();

				sql::PreparedStatement *prepStmt5;
				prepStmt5 = conn->prepareStatement( "DELETE FROM comments WHERE bug_id=?" );
				prepStmt5->setString(1, id);
				prepStmt5->executeQuery();

				sql::PreparedStatement *prepStmt6;
				prepStmt6 = conn->prepareStatement( "DELETE FROM bug WHERE bug_id=?" );
				prepStmt6->setString(1, id);
				prepStmt6->executeQuery();
	
				sql::PreparedStatement *prepStmt7;
				prepStmt7 = conn->prepareStatement( "DELETE FROM following WHERE bug_id=?" );
				prepStmt7->setString(1, id);
				prepStmt7->executeQuery();

				retJ["status"] = "success";
				delete prepStmt4;
				delete prepStmt5;
				delete prepStmt6;
				delete prepStmt7;
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
		delete prepStmt3;
		delete res;
		delete res2;
		delete res3;
	}
	catch(sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
  		std::cout << " (MySQL error code: " << e.getErrorCode();
  		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	catch(...) {
		std::cout << "Something went wrong with the SQL or the JSON formatting!\nDumping the dataset:\n";
		std::cout << retJ.dump(4) << std::endl;
	}

	

	return 0;
}
