#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;

// This function takes in a string and a character to search for
// it then peals everything up until that character off of the string
// and returns it as a new string. the original string also no longer
// has the elements up to the character
string biteString(string& str, char c);

// This function takes in a string and a character to search for
// then, every instance of that character that it finds, it peals off
// everything from the last time it pealed off characters and stores
// that as a new string in the vector.
vector<string> parseString(string str, char c);