#include "utils.h"

string biteString(string& str, char c)
{
	size_t pos = str.find(c);

	string result;

	if (pos == string::npos)
	{
		result = str;
		str = "";
	}
	else
	{
		result = str.substr(0, pos);
		str = str.substr(pos + 1);
	}
	return result;
}

// optionally you could return a pointer and just loop through
// and count how many instances of the char c there are and set an
// array to be that size, although you end up looping though the string
// twice so you have a trade off more efficant with memory but much less
// efficiant with performance
vector<string> parseString(string str, char c)
{
	vector<string> results;

	while (str != "")
		results.push_back(biteString(str, c));

	return results;
}