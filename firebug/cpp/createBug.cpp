/*
 * Original Author: Matthew Brayne
 * Date of Creation: 8/10/2016
 * Filename: createBug.cpp
 * Usage and implementation: this script allows a user with sufficient privileges to create a bug report. The script checks user permission by reading a user id from the cookie. If the cookie is empty, or the id does not exist in the database (ie: deleted user), then the script will not let the user create a bug. The script returns a success or failure message formatted as JSON.
 *
 * POST inputs: short_desc, classification, product, component, version, op_sys, bug_severity.
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

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/createBug.cgi cpp/createBug.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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

	const double REP_INCREMENT = 1;

	std::cout << cgicc::HTTPHTMLHeader() << std::endl;

	try {
		sql::Driver *driver;
		sql::Connection *conn;
		sql::PreparedStatement *prepStmt;
		sql::ResultSet *res;

		// Get POST values
		std::string short_desc = cgi("short_desc");
		std::string classification = cgi("classification");
		std::string product = cgi("product");
		std::string component = cgi("component");
		std::string version = cgi("version");
		std::string op_sys = cgi("op_sys");
		std::string bug_severity = cgi("bug_severity");

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
		
		if (!res->next())
		{
			// User has not been found, build appropriate empty JSON object
			retJ["status"] = "failure";
			retJ["message"] = "insufficient user privileges";
		}
		else
		{
			// User has been found, check for appropriate user privileges
			double reputation = res->getDouble("reputation");

			if (res->getInt("isAdmin") == 1 || res->getInt("isReporter") == 1 || res->getInt("isTriager") == 1 || res->getInt("isReviewer") == 1 || res->getInt("isDeveloper") == 1) // Allow everyone with correct user permissions to edit bug
			{
				sql::PreparedStatement *prepStmt3;
				
				// Add the new bug to the database
				prepStmt3 = conn->prepareStatement( "INSERT INTO bug (creation_ts, reporter, short_desc, classification, product, component, version, op_sys, bug_severity) VALUES (NOW(), ?, ?, ?, ?, ?, ?, ?, ?)" );
				prepStmt3->setString(1, res->getString("displayname"));
				prepStmt3->setString(2, short_desc);
				prepStmt3->setString(3, classification);
				prepStmt3->setString(4, product);
				prepStmt3->setString(5, component);
				prepStmt3->setString(6, version);
				prepStmt3->setString(7, op_sys);
				prepStmt3->setString(8, bug_severity);

				prepStmt3->executeQuery();

				// Update the 'following' table, users automatically follow any bug reports that they create
				sql::PreparedStatement *prepStmt6;
				prepStmt6 = conn->prepareStatement( "INSERT INTO following (username, bug_id) VALUES (?, (SELECT bug_id FROM bug WHERE reporter=? ORDER BY creation_ts DESC LIMIT 1))" );
				prepStmt6->setString(1, res->getString("username"));
				prepStmt6->setString(2, res->getString("displayname"));

				prepStmt6->executeQuery();
				
				// Update user reputation based on amount of fields filled out, and severity of bug
				if (!short_desc.empty()) {reputation += REP_INCREMENT;}
				if (!classification.empty()) {reputation += REP_INCREMENT;}
				if (!product.empty()) {reputation += REP_INCREMENT;}
				if (!component.empty()) {reputation += REP_INCREMENT;}
				if (!version.empty()) {reputation += REP_INCREMENT;}
				if (!op_sys.empty()) {reputation += REP_INCREMENT;}
				if (!bug_severity.empty()) {reputation += REP_INCREMENT;}
				if (bug_severity == "minor") {reputation += REP_INCREMENT;}
				if (bug_severity == "enhancement") {reputation += REP_INCREMENT;}
				if (bug_severity == "normal") {reputation += REP_INCREMENT * 3;}
				if (bug_severity == "major") {reputation += REP_INCREMENT * 6;}
				if (bug_severity == "critical") {reputation += REP_INCREMENT * 10;}

				// Update the database with the new reputation value
				sql::PreparedStatement *prepStmt7;
				prepStmt7 = conn->prepareStatement( "UPDATE user SET reputation=? WHERE id=?" );
				std::ostringstream strs;
				strs << reputation;
				std::string rep = strs.str();
				prepStmt7->setString(1, rep);
				prepStmt7->setString(2, userID);
				prepStmt7->executeQuery();
				
				retJ["status"] = "success";
				delete prepStmt3;
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
		delete res;
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
