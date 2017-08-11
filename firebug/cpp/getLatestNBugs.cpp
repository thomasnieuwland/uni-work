/*
 * Original Author: Matthew Brayne
 * Date of Creation: 8/10/2016
 * Filename: getLatestNBugs.cpp
 * Usage and implementation: this script searches the database for the latest nth bugs, sorted by creation date , then returns an array of n bugs formatted as a JSON string. This script takes a POST variable "n" representing the number of newest bugs to display.
 *
 * POST INPUT: n
 */

/* JSON OUTPUT:

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
	"status": "not found"
}

*/

/* HOW TO COMPILE FROM THE firebug DIRECTORY (this assumes you have installed json.hpp to the /usr/include/json directory) (this also assumes that you are using g++ version 4.9 or higher):

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/getLatestNBugs.cgi cpp/getLatestNBugs.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
		std::string n = cgi("n");

		// Create the connection
		driver = get_driver_instance();
		conn = driver->connect("localhost", "web", "VerySecure");

		// Connect to the database required
		conn->setSchema("FIREBUG");

		// Use a prepared statement to prevent SQL injection
		// Searches the database for the newest N amount of bugs
		prepStmt = conn->prepareStatement( "SELECT * FROM bug ORDER BY creation_ts DESC LIMIT ?" );

		// Apply the variables from POST and execute
		prepStmt->setString(1, n);

		// Execute the query
		res = prepStmt->executeQuery();
		
		// Convert results into JSON object
		if (!res->next())
		{
			// No bugs found, build appropriate empty JSON object
			retJ["status"] = "not found";
		}
		else
		{
			// Bugs have been found, build appropriate JSON object
			res->beforeFirst(); // Make sure we are at the start of the results list

			retJ["status"] = "found";
			
			while (res->next()) //create an array of comments objects
			{
				retJ["bugs"].push_back( { {"bug_id", res->getInt("bug_id")}, {"reporter", res->getString("reporter")}, {"creation_ts", res->getString("creation_ts")}, {"short_desc", res->getString("short_desc")}, {"classification", res->getString("classification")}, {"product", res->getString("product")}, {"component", res->getString("component")}, {"version", res->getString("version")}, {"op_sys", res->getString("op_sys")}, {"bug_status", res->getString("bug_status")}, {"resolution", res->getString("resolution")}, {"bug_severity", res->getString("bug_severity")}, {"votes", res->getInt("votes")}, {"assigned_to", res->getString("assigned_to")} } );
			}
		}

		// Return the object to the page
		std::cout << retJ << std::endl;
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
