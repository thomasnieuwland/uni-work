/*
 * Original Author: Matthew Brayne
 * Date of Creation: 22/10/2016
 * Filename: getAttachments.cpp
 * Usage and implementation: Searches through the database for attachments associated with the given bug report, returns an array of attachment objects formatted as JSON.
 */

/* JSON FORMAT: (NOTE: These attachments are sorted by the date they were created, so the first attachment should be on top, and the last on the bottom)

	POST INPUT: bug_id

{
	"status": "found",
	"attachments": [
		{
			"bug_id":	...,
			"attachid":	...,
			"attacher":	...,
			"date":		...,
			"filename":	...,
			"isobsolete":	...,
			"type":		...,
			"desc":		...
		},
		{
			"bug_id":	...,
			"attachid":	...,
			"attacher":	...,
			"date":		...,
			"filename":	...,
			"isobsolete":	...,
			"type":		...,
			"desc":		...
		}
	]
}

or...

{
	"status": "not found"
}

*/

/* HOW TO COMPILE FROM THE firebug DIRECTORY (this assumes you have installed json.hpp to the /usr/include/json directory) (this also assumes that you are using g++ version 4.9 or higher):

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/getAttachments.cgi cpp/getAttachments.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
		std::string bug = cgi("bug_id");

		// Create the connection
		driver = get_driver_instance();
		conn = driver->connect("localhost", "web", "VerySecure");

		// Connect to the database required
		conn->setSchema("FIREBUG");

		// Use a prepared statement to prevent SQL injection
		// Selects the bug data which matches the id supplied by POST
		prepStmt = conn->prepareStatement( "SELECT * FROM attachments WHERE bug_id=? ORDER BY date DESC" );

		// Apply the variables from POST and execute
		prepStmt->setString(1, bug);

		// Execute the query
		res = prepStmt->executeQuery();
		
		// Convert results into JSON object
		if (!res->next())
		{
			// Comment has not been found, build appropriate empty JSON object
			retJ["status"] = "not found";
		}
		else
		{
			// Comment has been found, build appropriate JSON object
			res->beforeFirst(); // Make sure we are at the start of the results list

			retJ["status"] = "found";
			
			while (res->next()) //create an array of comments objects
			{
				retJ["attachments"].push_back( { {"bug_id", res->getInt("bug_id")}, {"attacher", res->getString("attacher")}, {"attachid", res->getInt("attachid")}, {"date", res->getString("date")}, {"filename", res->getString("filename")}, {"desc", res->getString("desc")}, {"isobsolete", res->getString("isobsolete")}, {"type", res->getString("type")} } );
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
