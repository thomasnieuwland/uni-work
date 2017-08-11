/*
 * Original Author: Liam Foster
 * Date of Creation: 8/10/2016
 * Filename: getComments.cpp
 * Usage and implementation: Searches through the database for comments associated with the given bug report, returns an array of comment objects formatted as JSON.
 */

/* JSON FORMAT: (NOTE: These comments are sorted by the date they were created, so the first comment should be on top, and the last on the bottom)

{
	"status": "found",
	"comments": [
		{
			"bug_id":	...,
			"commentid":	...,
			"attachid":	...,
			"who":		...,
			"bug_when":	...,
			"thetext":	...
		},
		{
			"bug_id":	...,
			"commentid":	...,
			"attachid":	...,
			"who":		...,
			"bug_when":	...,
			"thetext":	...
		}
	]
}

or...

{
	"status": "not found"
}

*/

/* HOW TO COMPILE FROM THE firebug DIRECTORY (this assumes you have installed json.hpp to the /usr/include/json directory) (this also assumes that you are using g++ version 4.9 or higher):

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/getComments.cgi cpp/getComments.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
		std::string searchId = cgi("id");
		std::string source = cgi("src");

		// Create the connection
		driver = get_driver_instance();
		conn = driver->connect("localhost", "web", "VerySecure");

		// Connect to the database required
		conn->setSchema("FIREBUG");

		// Use a prepared statement to prevent SQL injection
		// Selects the bug data which matches the id supplied by POST
		if(source == "bug")
        {
            prepStmt = conn->prepareStatement( "SELECT * FROM comments WHERE bug_id=? ORDER BY bug_when DESC" );
        }
        else if(source == "user")
        {
            sql::PreparedStatement *usernameLookup;
            sql::ResultSet *usernameResults;

            usernameLookup = conn->prepareStatement("SELECT username FROM user WHERE id=?");
            usernameLookup->setString(1, searchId);

            usernameResults = usernameLookup->executeQuery();
            if(!usernameResults->next())
            {
                retJ["status"] = "not found";
            }
            else
            {
                searchId = usernameResults->getString("username");
            }

            prepStmt = conn->prepareStatement( "SELECT * FROM comments WHERE who=? ORDER BY bug_when DESC" );
        }

		// Apply the variables from POST and execute
		prepStmt->setString(1, searchId);

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
				retJ["comments"].push_back( { {"bug_id", res->getInt("bug_id")}, {"who", res->getString("who")}, {"commentid", res->getInt("commentid")}, {"bug_when", res->getString("bug_when")}, {"attachid", res->getInt("attachid")}, {"thetext", res->getString("thetext")} } );
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
