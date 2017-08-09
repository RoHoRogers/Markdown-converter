#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include "parser.h"

using namespace std;


//Italics 
regex italic1_regex("\\*(.*)\\*");	// * italic
regex italic2_regex("\\_(.*)\\_");	// _ italic

//Bolds 
regex bold1_regex("\\*\\*(.*)\\*\\*"); // ** bold  
regex bold2_regex("\\_\\_(.*)\\_\\_"); // __ bold

//Unordered lists
regex uList1_regex("^\\*(.*)"); // * list
regex uList2_regex("^\\-(.*)"); // - list
regex uList3_regex("^\\+(.*)"); // + list

//Ordered list 
regex oList_regex("^[0-9]*\\."); // numbered list

//Symbols
regex amp_regex("\\'(.*)&(.*)\\'"); // &amp when within quotes ''
regex lt_regex("\\'(.*)\\<(.*)\\'"); // &lt when within quotes ''


//H1-6 ATX style headers regexs
regex h1_regex("# (.*)");
regex h2_regex("## (.*)");
regex h3_regex("### (.*)");
regex h4_regex("#### (.*)");
regex h5_regex("##### (.*)");
regex h6_regex("###### (.*)");


extern bool need_paragraph;
extern bool need_oList;
extern bool need_uList;
extern bool current_paragraph;
extern bool current_list;
extern bool need_amp;
extern bool need_lt;

void str_replace(string &s, string &search, string &replace) {
	for (size_t pos = 0; ; pos += replace.length())
	{
		pos = s.find(search, pos);
		if (pos == string::npos)
			break;

		s.erase(pos, search.length());
		s.insert(pos, replace);
	}
}

bool search_headers_style(string& stringToReturn, smatch& match);


string parseline(string& line) {

	string stringToReturn = line;
	string toReplace;
	string search;
	smatch match;

	if ((stringToReturn.empty()) && (!current_paragraph) && (current_list) &&(need_uList)) {	//if currently in Unordered list, end list if end of line
		current_list = false;
		need_uList = false;
		toReplace += "</ul>";
		return toReplace;
	}
	if ((stringToReturn.empty()) && (!current_paragraph) && (current_list) && (need_oList)) {	//if currently in irdered list, end list if end of line
		current_list = false;
		need_oList = false;
		toReplace += "</ol>";
		return toReplace;
	}


	if ((!stringToReturn.empty()) && !(regex_search(stringToReturn, match, bold1_regex) || regex_search(stringToReturn, match, italic1_regex)
		|| regex_search(stringToReturn, match, italic2_regex) || regex_search(stringToReturn, match, uList3_regex)
		|| regex_search(stringToReturn, match, uList2_regex) || search_headers_style(stringToReturn, match)
		|| regex_search(stringToReturn, match, bold2_regex) || regex_search(stringToReturn, match, uList3_regex)
		|| regex_search(stringToReturn, match, uList1_regex) || regex_search(stringToReturn, match, oList_regex))) {

		if (need_paragraph) {
			toReplace += "<p>";
			toReplace += stringToReturn;
			stringToReturn = toReplace;
			need_paragraph = false;
			current_paragraph = true;
		}
	}

	if (stringToReturn.empty() && current_paragraph) {
		stringToReturn += "</p>";
		need_paragraph = true;
		current_paragraph = false;
		return stringToReturn;
	}

	if (line.size() == 0)
		return stringToReturn = "";

	if (regex_search(stringToReturn, match, amp_regex))
		need_amp = true;

	if (regex_search(stringToReturn, match, lt_regex))
		need_lt = true;

	while (regex_search(stringToReturn, match, bold1_regex) || regex_search(stringToReturn, match, italic1_regex) || regex_search(stringToReturn, match, italic2_regex)
		|| regex_search(stringToReturn, match, uList2_regex) || search_headers_style(stringToReturn, match)
		|| regex_search(stringToReturn, match, bold2_regex) || regex_search(stringToReturn, match, uList3_regex)
		|| regex_search(stringToReturn, match, uList1_regex) || regex_search(stringToReturn, match, amp_regex)
		|| regex_search(stringToReturn, match, lt_regex) || regex_search(stringToReturn, match, oList_regex)) {

		if (regex_search(stringToReturn, match, bold1_regex)) { // Bold ** search
			if (match[1] == "")
				break;

			if (need_paragraph) {
				toReplace += "<p>";
				need_paragraph = false;
				current_paragraph = true;
			}

			toReplace += "<strong>";
			toReplace += match[1];
			toReplace += "</strong>";

			search += "**";
			search += match[1];

			search += "**";
			str_replace(stringToReturn, search, toReplace);
		}

		toReplace = "";
		search = "";

		if (regex_search(stringToReturn, match, bold2_regex)) { // Bold __ search
			if (match[1] == "")
				break;

			toReplace += "<strong>";
			toReplace += match[1];
			toReplace += "</strong>";

			search += "__";
			search += match[1];
			search += "__";

			str_replace(stringToReturn, search, toReplace);

		}

		toReplace = "";
		search = "";

		if (regex_search(stringToReturn, match, italic1_regex)) { // Italic * search

			if (current_list) {
				current_list = false;
				toReplace += "</ul>";
			}

			if (need_paragraph) {
				toReplace += "<p>";
				need_paragraph = false;
				current_paragraph = true;
			}

			toReplace += "<em>";
			toReplace += match[1];
			toReplace += "</em>";
			search += "*";
			search += match[1];
			search += "*";

			str_replace(stringToReturn, search, toReplace);
		}

		toReplace = "";
		search = "";

		if (regex_search(stringToReturn, match, italic2_regex)) { // Italic - search


			if (need_paragraph) {
				toReplace += "<p>";
				need_paragraph = false;
				current_paragraph = true;
			}

			toReplace += "<em>";
			toReplace += match[1];
			toReplace += "</em>";
			search += "_";
			search += match[1];
			search += "_";

			str_replace(stringToReturn, search, toReplace);
		}

		toReplace = "";
		search = "";

		if (regex_search(stringToReturn, match, uList1_regex)) { // List * search
			if (current_paragraph)
				break;
			if (!current_list) {
				toReplace += "<ul>";
				current_list = true;
				need_uList = true;
			}

			toReplace += "<li>";
			toReplace += match[1];
			toReplace += "</li>";
			
			search += "*";
			search += match[1];

			str_replace(stringToReturn, search, toReplace);
		}

		toReplace = "";
		search = "";

		if (regex_search(stringToReturn, match, uList2_regex)) { // List - search
			if (current_paragraph)
				break;
			if (!current_list) {
				toReplace += "<ul>";
				current_list = true;
				need_uList = true;
			}

			toReplace += "<li>";
			toReplace += match[1];
			toReplace += "</li>";
			search += "-";
			search += match[1];

			str_replace(stringToReturn, search, toReplace);
		}

		toReplace = "";
		search = "";

		if (regex_search(stringToReturn, match, uList3_regex)) { // List + search
			if (current_paragraph)
				break;
			if (!current_list) {
				toReplace += "<ul>";
				current_list = true;
				need_uList = true;
			}

			toReplace += "<li>";
			toReplace += match[1];
			toReplace += "</li>";
			search += "+";
			search += match[1];

			str_replace(stringToReturn, search, toReplace);
		}

		toReplace = "";
		search = "";

		if (regex_search(stringToReturn, match, oList_regex)) { // Ordered list 0-9 search
			if (current_paragraph)
				break;
			if (!current_list) {
				toReplace += "<ol>";
				current_list = true;
				need_oList = true;
			}
			string temp = match[0];

			toReplace += "<li>";
			toReplace += match[1];
			toReplace += "</li>";
			search += match[0];
			search += match[1];

			str_replace(stringToReturn, search, toReplace);
		}

		toReplace = "";
		search = "";

		if (regex_search(stringToReturn, match, amp_regex)) { // &amp search
			if (!need_amp)
				break;

			toReplace += "&amp";
			toReplace += match[2];
			search += "&";
			search += match[2];

			need_amp = false;

			str_replace(stringToReturn, search, toReplace);
		}

		toReplace = "";
		search = "";

		if (regex_search(stringToReturn, match, lt_regex)) { // &lt search
			if (!need_lt)
				break;

			toReplace += "&lt";
			toReplace += match[2];
			search += "<";
			search += match[2];

			need_amp = false;

			str_replace(stringToReturn, search, toReplace);
		}

		toReplace = "";
		search = "";

		if (search_headers_style(stringToReturn, match)) { // Headers 1-6 search
			if (regex_search(stringToReturn, match, h6_regex)) {
				toReplace += "<h6>";
				toReplace += match[1];
				toReplace += "</h6>";
				search += "###### ";
				search += match[1];

				str_replace(stringToReturn, search, toReplace);
			}

			if (regex_search(stringToReturn, match, h5_regex)) {
				toReplace += "<h5>";
				toReplace += match[1];
				toReplace += "</h5>";
				search += "##### ";
				search += match[1];

				str_replace(stringToReturn, search, toReplace);
			}

			if (regex_search(stringToReturn, match, h4_regex)) {
				toReplace += "<h4>";
				toReplace += match[1];
				toReplace += "</h4>";
				search += "#### ";
				search += match[1];

				str_replace(stringToReturn, search, toReplace);
			}

			if (regex_search(stringToReturn, match, h3_regex)) {
				toReplace += "<h3>";
				toReplace += match[1];
				toReplace += "</h3>";
				search += "### ";
				search += match[1];

				str_replace(stringToReturn, search, toReplace);
			}

			if (regex_search(stringToReturn, match, h2_regex)) {
				toReplace += "<h2>";
				toReplace += match[1];
				toReplace += "</h2>";
				search += "## ";
				search += match[1];

				str_replace(stringToReturn, search, toReplace);
			}

			if (regex_search(stringToReturn, match, h1_regex)) {
				toReplace += "<h1>";
				toReplace += match[1];
				toReplace += "</h1>";
				search += "# ";
				search += match[1];

				str_replace(stringToReturn, search, toReplace);
			}
		}
	}

	return stringToReturn;



}



bool search_headers_style(string& stringToReturn, smatch& match) {
	if (regex_search(stringToReturn, match, h1_regex) || regex_search(stringToReturn, match, h2_regex)
		|| regex_search(stringToReturn, match, h3_regex) || regex_search(stringToReturn, match, h4_regex)
		|| regex_search(stringToReturn, match, h5_regex) || regex_search(stringToReturn, match, h6_regex))

		return true;

	return false;



}