/*
 * Original Author: Matthew Brayne
 * Date of Creation: 8/10/2016
 * Filename: getBug.cpp
 * Usage and implementation: this script searches the database for a bug with a specific ID, then returns the bug formatted as a JSON string.
 */

/* JSON FORMAT:

{
	"status": "found",
	"bug": {
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
		"assigned_to"		...
	}
}

or...

{
	"status": "not found"
}

*/

/* HOW TO COMPILE FROM THE firebug DIRECTORY (this assumes you have installed json.hpp to the /usr/include/json directory) (this also assumes that you are using g++ version 4.9 or higher):

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/getBug.cgi cpp/getBug.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
		std::string bug_id = cgi("bug_id");

		// Create the connection
		driver = get_driver_instance();
		conn = driver->connect("localhost", "web", "VerySecure");

		// Connect to the database required
		conn->setSchema("FIREBUG");

		// Use a prepared statement to prevent SQL injection
		// Selects the bug data which matches the id supplied by POST
		prepStmt = conn->prepareStatement( "SELECT * FROM bug WHERE bug_id=?" );

		// Apply the variables from POST and execute
		prepStmt->setString(1, bug_id);

		// Execute the query
		res = prepStmt->executeQuery();
		
		// Convert results into JSON object
		if (!res->next())
		{
			// Bug has not been found, build appropriate empty JSON object
			retJ["status"] = "not found";
		}
		else
		{
			// Bug has been found, build appropriate JSON object
			res->beforeFirst(); // Make sure we are at the start of the results list
			retJ["status"] = "found";

			while (res->next())
			{
				retJ["bug"] = {
					{"bug_id", res->getInt("bug_id")},
					{"reporter", res->getString("reporter")},
					{"creation_ts", res->getString("creation_ts")},
					{"short_desc", res->getString("short_desc")},
					{"classification", res->getString("classification")},
					{"product", res->getString("product")},
					{"component", res->getString("component")},
					{"version", res->getString("version")},
					{"op_sys", res->getString("op_sys")},
					{"bug_status", res->getString("bug_status")},
					{"resolution", res->getString("resolution")},
					{"bug_severity", res->getString("bug_severity")},
					{"votes", res->getInt("votes")},
					{"assigned_to", res->getString("assigned_to")}
				};
			
			}
		}

		// Return the object to the page
		std::cout << retJ << std::endl;
	}
	catch(...) {
		std::cout << "Something went wrong with the SQL or the JSON formatting!\nDumping the data set:\n";
		std::cout << retJ.dump(4) << std::endl;
	}

	return 0;
}
