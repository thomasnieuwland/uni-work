/*
 * Original Author: Matthew Brayne
 * Date of Creation: 25/10/2016
 * Filename: getReported.cpp
 * Usage and implementation:  returns all bugs that the current user has created
 */

//	POST input: n

/* JSON FORMAT:

{
	"status": "found",
	"bugs": [
		{
			"bug_id":		...,
			"reporter":		...,
			"creation_ts":		...,
			"short_desc":		...,
			"classification":	...,
			"product":		...,
			"component":		...,
			"version":		...,
			"op_sys":		...,
			"bug_status":		...,
			"resolution":		...,
			"bug_severity":		...,
			"votes":		...,
			"assigned_to":		...
		},
		{
			"bug_id":		...,
			"reporter":		...,
			"creation_ts":		...,
			"short_desc":		...,
			"classification":	...,
			"product":		...,
			"component":		...,
			"version":		...,
			"op_sys":		...,
			"bug_status":		...,
			"resolution":		...,
			"bug_severity":		...,
			"votes":		...,
			"assigned_to":		...
		}
	]
}

or...

{
	"status": "not found",
	"message": "error message goes here"
}

*/

/* HOW TO COMPILE FROM THE firebug DIRECTORY (this assumes you have installed json.hpp to the /usr/include/json directory) (this also assumes that you are using g++ version 4.9 or higher):

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/getReported.cgi cpp/getReported.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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

		std::string n = cgi("n");

		// Retrieve cookies ***
		cgicc::const_cookie_iterator cci;
   		const cgicc::CgiEnvironment& env = cgi.getEnvironment(); // Get environment variables
		cci = env.getCookieList().begin(); // Currently Firebug only tracks one cookie (user id)
		std::string id = cci->getValue(); // Now we need to use the ID to check the user's permissions

		// Create the connection
		driver = get_driver_instance();
		conn = driver->connect("localhost", "web", "VerySecure");

		// Connect to the database required
		conn->setSchema("FIREBUG");

		// Use a prepared statement to prevent SQL injection
		// Selects the bug data which matches the id supplied by POST
		prepStmt = conn->prepareStatement( "SELECT * FROM user WHERE id=?" );

		// Apply the variables from POST and execute
		prepStmt->setString(1, id);

		// Execute the query
		res = prepStmt->executeQuery();

		// Convert results into JSON object
		if (!res->next())
		{
			// User has not been found, build appropriate empty JSON object
			retJ["status"] = "not found";
			retJ["message"] = "Cookie not verified. Are you logged in?";
		}
		else
		{
			std::string username = res->getString("username");
			// User has been found, build appropriate JSON object
			sql::PreparedStatement *prepStmt2;
			prepStmt2 = conn->prepareStatement( "SELECT * FROM bug WHERE reporter = ? LIMIT ?" );
			prepStmt2->setString(1, username);
			prepStmt2->setString(2, n);
			sql::ResultSet *res2 = prepStmt2->executeQuery();

			if (!res2->next())
			{
				retJ["status"] = "not found";
				retJ["message"] = "You haven't created any reports";
				std::cout << retJ << std::endl;
				delete res2;
				delete prepStmt2;
				return 1;
			}

			retJ["status"] = "found";
			res2->beforeFirst();

			while (res2->next()) //create an array of bug objects
			{
				retJ["bugs"].push_back( { {"bug_id", res2->getInt("bug_id")}, {"reporter", res2->getString("reporter")}, {"creation_ts", res2->getString("creation_ts")}, {"short_desc", res2->getString("short_desc")}, {"classification", res2->getString("classification")}, {"product", res2->getString("product")}, {"component", res2->getString("component")}, {"version", res2->getString("version")}, {"op_sys", res2->getString("op_sys")}, {"bug_status", res2->getString("bug_status")}, {"resolution", res2->getString("resolution")}, {"bug_severity", res2->getString("bug_severity")}, {"votes", res2->getInt("votes")} } );
			}
			delete res2;
			delete prepStmt2;
		}

		// Return the object to the page
		std::cout << retJ << std::endl;
		delete res;
		delete prepStmt;
		delete conn;
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
