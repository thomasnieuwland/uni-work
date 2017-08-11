/*
 * Original Author: Liam Foster
 * Date of Creation: 16/10/2016
 * Filename: deleteAttachment.cpp
 * Usage and implementation: this script allows the creater of an attachment or an admin to delete an attachment for a bug report.
 * POST inputs: attachid
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

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/deleteAttachment.cgi cpp/deleteAttachment.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
		std::string id = cgi("attachid");

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

		sql::PreparedStatement *prepStmt2;
		sql::ResultSet *res2;

		prepStmt2 = conn->prepareStatement( "SELECT attacher FROM attachments WHERE attachid=?" );

		prepStmt2->setString(1, id);

		res2 = prepStmt2->executeQuery();

		sql::PreparedStatement *prepStmt3;
		sql::ResultSet *res3;

		prepStmt3 = conn->prepareStatement( "SELECT displayname FROM user WHERE id=?" );

		prepStmt3->setString(1, userID);

		res3 = prepStmt3->executeQuery();


		if (!res->next() || !res2->next() || !res3->next())
		{
			// User has not been found, build appropriate empty JSON object
			retJ["status"] = "failure";
			retJ["message"] = "insufficient user privileges or cookie index not working?";
		}
		else
		{
			// User has been found, check for appropriate user privileges
			std::string userRole = res->getString("isAdmin");
			std::string attachmentOwner = res2->getString("attacher");
			std::string userDisplayName = res3->getString("displayname");
			if (userRole == "1" || attachmentOwner == userDisplayName) // Allow admins and the creator to edit bug
			{
				sql::PreparedStatement *prepStmt4;
				prepStmt4 = conn->prepareStatement( "DELETE FROM attachments WHERE attachid=?" );
				prepStmt4->setString(1, id);
				prepStmt4->executeQuery();

				sql::PreparedStatement *prepStmt5;
				prepStmt5 = conn->prepareStatement( "DELETE FROM comments WHERE attachid=?" );
				prepStmt5->setString(1, id);
				prepStmt5->executeQuery();

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
