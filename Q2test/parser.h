#pragma once

#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <regex>

using namespace std;


string parseline(string& line);
void str_replace(string &s, string &search, string &replace);
bool search_headers_style(string& stringToReturn, smatch& match);


#endif /*_PARSER_H_ */