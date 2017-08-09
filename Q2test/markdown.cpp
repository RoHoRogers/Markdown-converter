#include <fstream>
#include <iostream>
#include <string>

#include "markdown.h"
#include "parser.h"

using namespace std;

Markdown::Markdown(string file_name) {
	this->file_name = file_name;
}

Markdown::~Markdown() {
	cout << "Md_file destructor" << endl;
}

ifstream& Markdown::getFile(void) {
	return file;
}

bool Markdown::openFile(void) {
	const char* c_file_name = (this->file_name).c_str();
	(this->file).open(c_file_name, ios::in);
	return ((this->file).is_open());
}

void Markdown::closeFile() {
	(this->file).close();
}

/*					Had trouble getting this to work from the command line so I used console input instead

void Markdown::markdown_to_html(const char * iFile, const char * oFile) {
	string line;
	string parsed_line;
	
	Markdown* md_file = new Markdown(iFile);
	ofstream outfile;

	outfile.open(oFile, ios::out);



	if (!md_file->openFile()) {
		cout << "File not found..." << endl;
		abort();
	}

	cout << "File opened" << endl;

	cout << endl;

	while (getline(md_file->getFile(), line)) {
		outfile << parseline(line) << endl;
	}


	md_file->closeFile();

	outfile.close();

	cout << endl;

	delete md_file;
}*/
