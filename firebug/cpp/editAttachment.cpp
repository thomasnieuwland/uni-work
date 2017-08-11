/*
 * Original Author: Liam Foster
 * Date of Creation: 23/10/2016
 * Filename: editAttachment.cpp
 * Usage and implementation: this script allows an admin or owner of an attachment to modify the relevant attachment
 *
 * POST inputs: attachid, bug_id, isobsolete, ispatch, desc, filename, type, size, attacher.
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

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/editAttachment.cgi cpp/editAttachment.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
		sql::PreparedStatement *prepStmt3;
		sql::ResultSet *res;
		sql::ResultSet *res2;
		sql::ResultSet *res3;

		// Get POST values
		std::string id = cgi("attachid");
		std::string isobsolete = cgi("isobsolete");
		std::string ispatch = cgi("ispatch");
		std::string filename = cgi("filename");
		std::string type = cgi("type");
		std::string size = cgi("size");

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

		prepStmt2 = conn->prepareStatement( "SELECT attacher FROM attachments WHERE attachid=?" );
		prepStmt2->setString(1, id);
		res2 = prepStmt2->executeQuery();
		
		if (!res2->next())
		{
			// User has not been found, build appropriate empty JSON object
			retJ["status"] = "failure";
			retJ["message"] = "Cannot locate attacher";
		}
		else
		{
			std::string attachmentOwner = res2->getString("attacher");

			// Selects the id of the user who created the attachment
			prepStmt3 = conn->prepareStatement( "SELECT id FROM user WHERE username=?" );
			prepStmt3->setString(1, attachmentOwner);
			res3 = prepStmt3->executeQuery();
		
			if (!res->next())	
			{
				// User has not been found, build appropriate empty JSON object
				retJ["status"] = "failure";
				retJ["message"] = "insufficient user privileges or cookie index not working?";
			}
			else if (!res3->next())	
			{
				// User has not been found, build appropriate empty JSON object
				retJ["status"] = "failure";
				retJ["message"] = "Could not locate users id in table user";
			}
			else
			{
				std::string ownerID = res3->getString("id");

				// User has been found, check for appropriate user privileges
	
				if (res->getInt("isAdmin") == 1 || userID == ownerID) // Allow admins and the creator to edit bug
				{
					sql::PreparedStatement *prepStmt4;
					prepStmt4 = conn->prepareStatement( "UPDATE attachments SET isobsolete=?, ispatch=?, filename=?, type=?, size=? WHERE attachid=?" );
					prepStmt4->setString(1, isobsolete);
					prepStmt4->setString(2, ispatch);
					prepStmt4->setString(3, filename);
					prepStmt4->setString(4, type);
					prepStmt4->setString(5, size);
					prepStmt4->setString(6, id);
					prepStmt4->executeQuery();

					retJ["status"] = "success";
					delete prepStmt4;
				}
				else
				{
					retJ["status"] = "failure";
					retJ["message"] = "insufficient user privileges";
				}
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
		std::cout << "Something went wrong with the SQL or the JSON formatting!\nDumping the data set:\n";
		std::cout << retJ.dump(4) << std::endl;
	}

	

	return 0;
}
