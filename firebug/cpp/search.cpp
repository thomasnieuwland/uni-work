/*
 * Original Author: Matthew Brayne
 * Date of Creation: 23/10/2016
 * Filename: search.cpp
 * Usage and implementation: This is an advanced search that returns bugs based on relevance
 *
 * POST INPUT: search_term, n
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
			"assigned_to":		...,
			"relevance":		...
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
			"assigned_to":		...,
			"relevance":		...
		}
	]
}

or...

{
	"status": "not found"
}

*/

/* HOW TO COMPILE FROM THE firebug DIRECTORY (this assumes you have installed json.hpp to the /usr/include/json directory) (this also assumes that you are using g++ version 4.9 or higher):

g++ -std=c++11 -Wall -I/usr/include/cppconn -I/usr/include/json -o web/cgi-bin/search.cgi cpp/search.cpp cpp/Bug.cpp -L/usr/lib -lmysqlcppconn -lcgicc
*/

/* Standard C++ includes */
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "Bug.h"

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

	// Words to exclude from search term
	std::string stopwords[] = {"a", "about", "above", "across", "after", "afterwards", "again", "against", "all", "almost", "alone", "along", "already", "also", "although", "always","am","among", "amongst", "amoungst", "amount", "an", "and", "another", "any", "anyhow", "anyone", "anything", "anyway", "anywhere", "are", "around", "as", "at", "back", "be", "became", "because", "become", "becomes", "becoming", "been", "before", "beforehand", "behind", "being", "below", "beside", "besides", "between", "beyond", "bill", "both", "bottom","but", "by", "call", "co", "con", "could", "couldnt", "cry", "de", "describe", "detail", "do", "done", "down", "due", "during", "each", "eg", "eight", "either", "eleven","else", "elsewhere", "enough", "etc", "even", "ever", "every", "everyone", "everything", "everywhere", "except", "few", "fifteen", "fify", "fill", "find", "fire", "first", "five", "for", "former", "formerly", "forty", "found", "four", "from", "front", "full", "further", "get", "give", "go", "had", "has", "hasnt", "have", "he", "hence", "her", "here", "hereafter", "hereby", "herein", "hereupon", "hers", "herself", "him", "himself", "his", "how", "however", "hundred", "ie", "if", "in", "inc", "indeed", "interest", "into", "is", "it", "its", "itself", "keep", "last", "latter", "latterly", "least", "less", "ltd", "made", "many", "may", "me", "meanwhile", "might", "mill", "mine", "more", "moreover", "most", "mostly", "move", "much", "must", "my", "myself", "name", "namely", "neither", "never", "nevertheless", "next", "nine", "no", "nobody", "none", "noone", "nor", "not", "nothing", "now", "nowhere", "of", "off", "often", "on", "once", "one", "only", "onto", "or", "other", "others", "otherwise", "our", "ours", "ourselves", "out", "over", "own","part", "per", "perhaps", "please", "put", "rather", "re", "see", "seem", "seemed", "seeming", "seems", "serious", "several", "she", "should", "show", "side", "since", "sincere", "six", "sixty", "so", "some", "somehow", "someone", "something", "sometime", "sometimes", "somewhere", "still", "such", "system", "take", "ten", "than", "that", "the", "their", "them", "themselves", "then", "thence", "there", "thereafter", "thereby", "therefore", "therein", "thereupon", "these", "they", "thick", "thin", "third", "this", "those", "though", "three", "through", "throughout", "thru", "thus", "to", "together", "too", "top", "toward", "towards", "twelve", "twenty", "two", "un", "under", "until", "up", "upon", "us", "very", "via", "was", "we", "well", "were", "what", "whatever", "when", "whence", "whenever", "where", "whereafter", "whereas", "whereby", "wherein", "whereupon", "wherever", "whether", "which", "while", "whither", "who", "whoever", "whole", "whom", "whose", "why", "will", "with", "within", "without", "would", "yet", "you", "your", "yours", "yourself", "yourselves", "the"};	

	std::cout << cgicc::HTTPHTMLHeader() << std::endl;

	try {
		sql::Driver *driver;
		sql::Connection *conn;

		// Get POST values
		std::string search_term = cgi("search_term");
		std::string n = cgi("n");

		// Create the connection
		driver = get_driver_instance();
		conn = driver->connect("localhost", "web", "VerySecure");

		// Connect to the database required
		conn->setSchema("FIREBUG");
/*
		std::string query = "SELECT DISTINCT * FROM bug t1 JOIN comments t2 ON t2.bug_id = t1.bug_id JOIN attachments t3 ON t3.bug_id = t1.bug_id WHERE short_desc LIKE '%" + search_term + "%' OR classification LIKE '%" + search_term + "%' OR product LIKE '%" + search_term + "%' OR component LIKE '%" + search_term + "%' OR op_sys LIKE '%" + search_term + "%' OR reporter LIKE '%" + search_term + "%' OR assigned_to LIKE '%" + search_term + "%' OR t2.thetext LIKE '%" + search_term + "%' OR t2.who LIKE '%" + search_term + "%' OR t3.desc LIKE '%" + search_term + "%' OR t3.attacher LIKE '%" + search_term + "%' OR t3.filename LIKE '%" + search_term + "%' LIMIT ?";
*/

		/***********************************************************************************
						SEARCH FUNCTION
		***********************************************************************************/

		// First break down the search term into individual words (tokens)
		std::vector<std::string> tokens;
    		std::istringstream iss(search_term);
		std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(tokens));

		// Exclude dictionary terms
		for (unsigned int i = 0; i < tokens.size(); i++)
		{
			for (unsigned int j = 0; j < sizeof(stopwords)/sizeof(stopwords[0]); j++)
			{
				if (tokens[i] == stopwords[j])
				{
  					tokens.erase(tokens.begin()+i);
				}
			}
		}

		// Check if search term is still valid. If not, end the program before needlessly searching the database
		if (tokens.empty())
		{
			retJ["status"] = "not found";
			retJ["message"] = "no results found";
			std::cout << retJ << std::endl;
			return 1;
		}

		// For each token, check the database and build bug objects
		std::vector<Bug> bugList;
		for (unsigned int i = 0; i < tokens.size(); i++)
		{
			sql::PreparedStatement *prepStmt;
			sql::ResultSet *res;

			prepStmt = conn->prepareStatement( "SELECT DISTINCT * FROM bug WHERE bug_id LIKE '%" + tokens[i] + "%'" );
			res = prepStmt->executeQuery();

			while (res->next())
			{
				// Check if the bug is already in the list
				bool found = false;
				int marker;
				for (unsigned int j = 0; j < bugList.size(); j++)
				{
					if (bugList[j].getBug_id() == res->getInt("bug_id"))
					{
						found = true;
						marker = j;
					}
				}
				if (found) // If the bug is already in the list, just increase the relevance
				{
					bugList[marker].addRelevance();
					bugList[marker].addRelevance();
					bugList[marker].addRelevance();
					bugList[marker].addRelevance();
					bugList[marker].addRelevance();
					bugList[marker].addRelevance();
					bugList[marker].addRelevance();
					bugList[marker].addRelevance();
					bugList[marker].addRelevance();
					bugList[marker].addRelevance(); // Did this 10 times because the relevance of an explicit bug_id is massive
				}
				else // If the bug is not in the list already, add it to the list
				{
					Bug buggyMcBugFace(res->getInt("bug_id"), res->getString("creation_ts"), res->getString("short_desc"), res->getString("classification"), res->getString("product"), res->getString("component"), res->getString("version"), res->getString("op_sys"), res->getString("bug_status"), res->getString("resolution"), res->getString("bug_severity"), res->getInt("votes"), res->getString("reporter"), res->getString("assigned_to"));
					buggyMcBugFace.addRelevance();
					buggyMcBugFace.addRelevance();
					buggyMcBugFace.addRelevance();
					buggyMcBugFace.addRelevance();
					buggyMcBugFace.addRelevance();
					buggyMcBugFace.addRelevance();
					buggyMcBugFace.addRelevance();
					buggyMcBugFace.addRelevance();
					buggyMcBugFace.addRelevance();
					buggyMcBugFace.addRelevance(); // Did this 10 times because the relevance of an explicit bug_id is massive

					bugList.push_back(buggyMcBugFace);
				}
			}

			prepStmt = conn->prepareStatement( "SELECT DISTINCT * FROM bug WHERE LOWER(short_desc) LIKE LOWER('%" + tokens[i] + "%')" );
			res = prepStmt->executeQuery();

			while (res->next())
			{
				// Check if the bug is already in the list
				bool found = false;
				int marker;
				for (unsigned int j = 0; j < bugList.size(); j++)
				{
					if (bugList[j].getBug_id() == res->getInt("bug_id"))
					{
						found = true;
						marker = j;
					}
				}
				if (found) // If the bug is already in the list, just increase the relevance
				{
					bugList[marker].addRelevance();
				}
				else // If the bug is not in the list already, add it to the list
				{
					Bug buggyMcBugFace(res->getInt("bug_id"), res->getString("creation_ts"), res->getString("short_desc"), res->getString("classification"), res->getString("product"), res->getString("component"), res->getString("version"), res->getString("op_sys"), res->getString("bug_status"), res->getString("resolution"), res->getString("bug_severity"), res->getInt("votes"), res->getString("reporter"), res->getString("assigned_to"));
					buggyMcBugFace.addRelevance();

					bugList.push_back(buggyMcBugFace);
				}
			}

			prepStmt = conn->prepareStatement( "SELECT DISTINCT * FROM bug WHERE LOWER(classification) LIKE LOWER('%" + tokens[i] + "%')" );
			res = prepStmt->executeQuery();

			while (res->next())
			{
				// Check if the bug is already in the list
				bool found = false;
				int marker;
				for (unsigned int j = 0; j < bugList.size(); j++)
				{
					if (bugList[j].getBug_id() == res->getInt("bug_id"))
					{
						found = true;
						marker = j;
					}
				}
				if (found) // If the bug is already in the list, just increase the relevance
				{
					bugList[marker].addRelevance();
				}
				else // If the bug is not in the list already, add it to the list
				{
					Bug buggyMcBugFace(res->getInt("bug_id"), res->getString("creation_ts"), res->getString("short_desc"), res->getString("classification"), res->getString("product"), res->getString("component"), res->getString("version"), res->getString("op_sys"), res->getString("bug_status"), res->getString("resolution"), res->getString("bug_severity"), res->getInt("votes"), res->getString("reporter"), res->getString("assigned_to"));
					buggyMcBugFace.addRelevance();

					bugList.push_back(buggyMcBugFace);
				}
			}

			prepStmt = conn->prepareStatement( "SELECT DISTINCT * FROM bug WHERE LOWER(product) LIKE LOWER('%" + tokens[i] + "%')" );
			res = prepStmt->executeQuery();

			while (res->next())
			{
				// Check if the bug is already in the list
				bool found = false;
				int marker;
				for (unsigned int j = 0; j < bugList.size(); j++)
				{
					if (bugList[j].getBug_id() == res->getInt("bug_id"))
					{
						found = true;
						marker = j;
					}
				}
				if (found) // If the bug is already in the list, just increase the relevance
				{
					bugList[marker].addRelevance();
				}
				else // If the bug is not in the list already, add it to the list
				{
					Bug buggyMcBugFace(res->getInt("bug_id"), res->getString("creation_ts"), res->getString("short_desc"), res->getString("classification"), res->getString("product"), res->getString("component"), res->getString("version"), res->getString("op_sys"), res->getString("bug_status"), res->getString("resolution"), res->getString("bug_severity"), res->getInt("votes"), res->getString("reporter"), res->getString("assigned_to"));
					buggyMcBugFace.addRelevance();

					bugList.push_back(buggyMcBugFace);
				}
			}

			prepStmt = conn->prepareStatement( "SELECT DISTINCT * FROM bug WHERE LOWER(component) LIKE LOWER('%" + tokens[i] + "%')" );
			res = prepStmt->executeQuery();

			while (res->next())
			{
				// Check if the bug is already in the list
				bool found = false;
				int marker;
				for (unsigned int j = 0; j < bugList.size(); j++)
				{
					if (bugList[j].getBug_id() == res->getInt("bug_id"))
					{
						found = true;
						marker = j;
					}
				}
				if (found) // If the bug is already in the list, just increase the relevance
				{
					bugList[marker].addRelevance();
				}
				else // If the bug is not in the list already, add it to the list
				{
					Bug buggyMcBugFace(res->getInt("bug_id"), res->getString("creation_ts"), res->getString("short_desc"), res->getString("classification"), res->getString("product"), res->getString("component"), res->getString("version"), res->getString("op_sys"), res->getString("bug_status"), res->getString("resolution"), res->getString("bug_severity"), res->getInt("votes"), res->getString("reporter"), res->getString("assigned_to"));
					buggyMcBugFace.addRelevance();

					bugList.push_back(buggyMcBugFace);
				}
			}

			prepStmt = conn->prepareStatement( "SELECT DISTINCT * FROM bug WHERE LOWER(version) LIKE LOWER('%" + tokens[i] + "%')" );
			res = prepStmt->executeQuery();

			while (res->next())
			{
				// Check if the bug is already in the list
				bool found = false;
				int marker;
				for (unsigned int j = 0; j < bugList.size(); j++)
				{
					if (bugList[j].getBug_id() == res->getInt("bug_id"))
					{
						found = true;
						marker = j;
					}
				}
				if (found) // If the bug is already in the list, just increase the relevance
				{
					bugList[marker].addRelevance();
				}
				else // If the bug is not in the list already, add it to the list
				{
					Bug buggyMcBugFace(res->getInt("bug_id"), res->getString("creation_ts"), res->getString("short_desc"), res->getString("classification"), res->getString("product"), res->getString("component"), res->getString("version"), res->getString("op_sys"), res->getString("bug_status"), res->getString("resolution"), res->getString("bug_severity"), res->getInt("votes"), res->getString("reporter"), res->getString("assigned_to"));
					buggyMcBugFace.addRelevance();

					bugList.push_back(buggyMcBugFace);
				}
			}

			prepStmt = conn->prepareStatement( "SELECT DISTINCT * FROM bug WHERE LOWER(op_sys) LIKE LOWER('%" + tokens[i] + "%')" );
			res = prepStmt->executeQuery();

			while (res->next())
			{
				// Check if the bug is already in the list
				bool found = false;
				int marker;
				for (unsigned int j = 0; j < bugList.size(); j++)
				{
					if (bugList[j].getBug_id() == res->getInt("bug_id"))
					{
						found = true;
						marker = j;
					}
				}
				if (found) // If the bug is already in the list, just increase the relevance
				{
					bugList[marker].addRelevance();
				}
				else // If the bug is not in the list already, add it to the list
				{
					Bug buggyMcBugFace(res->getInt("bug_id"), res->getString("creation_ts"), res->getString("short_desc"), res->getString("classification"), res->getString("product"), res->getString("component"), res->getString("version"), res->getString("op_sys"), res->getString("bug_status"), res->getString("resolution"), res->getString("bug_severity"), res->getInt("votes"), res->getString("reporter"), res->getString("assigned_to"));
					buggyMcBugFace.addRelevance();

					bugList.push_back(buggyMcBugFace);
				}
			}

			prepStmt = conn->prepareStatement( "SELECT DISTINCT * FROM bug WHERE LOWER(reporter) LIKE LOWER('%" + tokens[i] + "%')" );
			res = prepStmt->executeQuery();

			while (res->next())
			{
				// Check if the bug is already in the list
				bool found = false;
				int marker;
				for (unsigned int j = 0; j < bugList.size(); j++)
				{
					if (bugList[j].getBug_id() == res->getInt("bug_id"))
					{
						found = true;
						marker = j;
					}
				}
				if (found) // If the bug is already in the list, just increase the relevance
				{
					bugList[marker].addRelevance();
				}
				else // If the bug is not in the list already, add it to the list
				{
					Bug buggyMcBugFace(res->getInt("bug_id"), res->getString("creation_ts"), res->getString("short_desc"), res->getString("classification"), res->getString("product"), res->getString("component"), res->getString("version"), res->getString("op_sys"), res->getString("bug_status"), res->getString("resolution"), res->getString("bug_severity"), res->getInt("votes"), res->getString("reporter"), res->getString("assigned_to"));
					buggyMcBugFace.addRelevance();

					bugList.push_back(buggyMcBugFace);
				}
			}

			prepStmt = conn->prepareStatement( "SELECT DISTINCT * FROM bug WHERE LOWER(assigned_to) LIKE LOWER('%" + tokens[i] + "%')" );
			res = prepStmt->executeQuery();

			while (res->next())
			{
				// Check if the bug is already in the list
				bool found = false;
				int marker;
				for (unsigned int j = 0; j < bugList.size(); j++)
				{
					if (bugList[j].getBug_id() == res->getInt("bug_id"))
					{
						found = true;
						marker = j;
					}
				}
				if (found) // If the bug is already in the list, just increase the relevance
				{
					bugList[marker].addRelevance();
				}
				else // If the bug is not in the list already, add it to the list
				{
					Bug buggyMcBugFace(res->getInt("bug_id"), res->getString("creation_ts"), res->getString("short_desc"), res->getString("classification"), res->getString("product"), res->getString("component"), res->getString("version"), res->getString("op_sys"), res->getString("bug_status"), res->getString("resolution"), res->getString("bug_severity"), res->getInt("votes"), res->getString("reporter"), res->getString("assigned_to"));
					buggyMcBugFace.addRelevance();

					bugList.push_back(buggyMcBugFace);
				}
			}

			delete res;
			delete prepStmt;
		}

		if (bugList.empty())
		{
			retJ["status"] = "not found";
			retJ["message"] = "no results found";
			std::cout << retJ << std::endl;
			return 1;
		}
		
		// Now we have a list (bugList) of relevant bugs. Sort by relevance.
		std::sort (bugList.begin(), bugList.end());

		// Convert the cgi("n") POST value to integer
		unsigned int n_int = atoi(n.c_str());

		// Add the most relevant n bugs to the JSON object for outputting
		if (n_int > bugList.size())
		{
			n_int = bugList.size();
		}
		for (unsigned int i = 0; i < n_int; i++)
		{
			retJ["bugs"].push_back( { {"bug_id", bugList[i].getBug_id()}, {"reporter", bugList[i].getReporter()}, {"creation_ts", bugList[i].getCreation_ts()}, {"short_desc", bugList[i].getShort_desc()}, {"classification", bugList[i].getClassification()}, {"product", bugList[i].getProduct()}, {"component", bugList[i].getComponent()}, {"version", bugList[i].getVersion()}, {"op_sys", bugList[i].getOp_sys()}, {"bug_status", bugList[i].getBug_status()}, {"resolution", bugList[i].getResolution()}, {"bug_severity", bugList[i].getBug_severity()}, {"votes", bugList[i].getVotes()}, {"assigned_to", bugList[i].getAssigned_to()}, {"relevance", bugList[i].getRelevance()} } );
		}
		retJ["status"] = "found";

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
