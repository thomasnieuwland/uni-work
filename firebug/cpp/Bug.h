#ifndef BUG_H_INCLUDED
#define BUG_H_INCLUDED

#include <string>

class Bug
{
	private:
		int bug_id;
		std::string creation_ts;
		std::string short_desc;
		std::string classification;
		std::string product;
		std::string component;
		std::string version;
		std::string op_sys;
		std::string bug_status;
		std::string resolution;
		std::string bug_severity;
		int votes;
		std::string reporter;
		std::string assigned_to;
		int relevance;
	public:
		Bug();
		Bug(int a, std::string b, std::string c, std::string d, std::string e, std::string f, std::string g, std::string h, std::string i, std::string j, std::string k, int l, std::string m, std::string n);
		int getBug_id();
		std::string getCreation_ts();
		std::string getShort_desc();
		std::string getClassification();
		std::string getProduct();
		std::string getComponent();
		std::string getVersion();
		std::string getOp_sys();
		std::string getBug_status();
		std::string getResolution();
		std::string getBug_severity();
		int getVotes();
		std::string getReporter();
		std::string getAssigned_to();
		int getRelevance();
		void addRelevance();

		friend bool operator< (Bug& l, Bug& r)
		{
			return l.relevance >= r.relevance;
		}
};

#endif
