#include "Bug.h"
#include <string>

Bug::Bug()
{
	bug_id = 0;
	creation_ts = "";
	short_desc = "";
	classification = "";
	product = "";
	component = "";
	version = "";
	op_sys = "";
	bug_status = "";
	resolution = "";
	bug_severity = "";
	votes = 0;
	reporter = "";
	assigned_to = "";
	relevance = 0;
	
}

Bug::Bug(int a, std::string b, std::string c, std::string d, std::string e, std::string f, std::string g, std::string h, std::string i, std::string j, std::string k, int l, std::string m, std::string n)
{
	bug_id = a;
	creation_ts = b;
	short_desc = c;
	classification = d;
	product = e;
	component = f;
	version = g;
	op_sys = h;
	bug_status = i;
	resolution = j;
	bug_severity = k;
	votes = l;
	reporter = m;
	assigned_to = n;
	relevance = 0;
}

int Bug::getBug_id()
{
	return bug_id;
}

std::string Bug::getCreation_ts()
{
	return creation_ts;
}

std::string Bug::getShort_desc()
{
	return short_desc;
}

std::string Bug::getClassification()
{
	return classification;
}

std::string Bug::getProduct()
{
	return product;
}

std::string Bug::getComponent()
{
	return component;
}

std::string Bug::getVersion()
{
	return version;
}

std::string Bug::getOp_sys()
{
	return op_sys;
}

std::string Bug::getBug_status()
{
	return bug_status;
}

std::string Bug::getResolution()
{
	return resolution;
}

std::string Bug::getBug_severity()
{
	return bug_severity;
}

int Bug::getVotes()
{
	return votes;
}

std::string Bug::getReporter()
{
	return reporter;
}

std::string Bug::getAssigned_to()
{
	return assigned_to;
}

void Bug::addRelevance()
{
	relevance++;
}

int Bug::getRelevance()
{
	return relevance;
}

