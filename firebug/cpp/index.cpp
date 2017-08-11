/*
 * Original Author: Matthew Brayne
 * Date of Creation: 15/10/2016
 * Filename: index.cpp
 * Usage and implementation: this script checks if the user is logged in already. If so, the user is redirected to the dashboard. If not, the user is redirected to the login page
 *
 *
 */

/* HOW TO COMPILE FROM THE firebug DIRECTORY (this assumes you have installed json.hpp to the /usr/include/json directory) (this also assumes that you are using g++ version 4.9 or higher):

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/index.cgi cpp/index.cpp -L/usr/lib -lmysqlcppconn -lcgicc
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
#include "cgicc/HTTPRedirectHeader.h"

/* JSON Include */
#include "json.hpp"

int main(void)
{
	cgicc::Cgicc cgi;

	try {
		// Retrieve cookies ***
		cgicc::const_cookie_iterator cci;
   		const cgicc::CgiEnvironment& env = cgi.getEnvironment(); // Get environment variables
		cci = env.getCookieList().begin(); // Currently Firebug only tracks one cookie (user id)
		std::string userID = cci->getValue(); // Now we need to use the ID to check the user's permissions

		if (userID.empty())
		{
			// Cookie has not been found, display login page
			std::cout << cgicc::HTTPHTMLHeader() << std::endl;
		}
		else
		{
			// Cookie has been found, redirect to dashboard
			std::cout << cgicc::HTTPRedirectHeader("../dashboard") << std::endl;
		}

	}
	catch(...) {
		std::cout << "Something went wrong with the cookies!\n";
	}

	

	return 0;
}
