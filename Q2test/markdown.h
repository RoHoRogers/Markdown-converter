#pragma once
#ifndef _MARKDOWN_H_ 
#define _MARKDOWN_H_

#include <fstream>
#include <iostream>

using namespace std;

class Markdown {

private:
	string file_name;
	ifstream file;

public:
	Markdown(string file_name);
	~Markdown(void);
	ifstream& getFile(void);
	bool openFile(void);
	void closeFile(void);
//	void markdown_to_html(const char *iFile, const char *oFile);
};

#endif /* _MARKDOWN_H_ */