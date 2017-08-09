#include "markdown.h"
#include "parser.h"

using namespace std;

bool need_paragraph = true;
bool need_oList = false;
bool need_uList;
bool current_paragraph = false;
bool current_list = false;
bool need_amp = false;
bool need_lt = false;

int main(int argc, char* argv[]) {

	char inFile[100] = "markdown.txt";
	char outFile[100] = "markdown.html";

	if (argc > 1)
		strcpy(inFile, argv[1]);
	if (argc > 2)
		strcpy(outFile, argv[2]);
/******Had trouble getting the files from the command line so I made a prompt input instead******

    Md_file* test = new Md_file(inFile);
	test->markdown_to_html(inFile, outFile);
	cout << "done";
*/
		string iFile_name;
		string oFile_name;
		string line;
		string parsed_line;

		cout << "*********************************************" << endl
			<< "Welcome to Bungie Markdown to HTML" << endl
			<< "*********************************************" << endl << endl;
		cout << "Please enter an input file name" << endl;

		cin >> iFile_name;

		Markdown* md_file = new Markdown(inFile);

		cout << endl << "Please enter an output file name" << endl;

		cin >> oFile_name;

		ofstream outfile;
		outfile.open(oFile_name, ios::out);

		if (!md_file->openFile()) {
			cout << "File not found..." << endl;
			return 1;
		}

		cout << "File opened" << endl;
		cout << endl;

		//Go through text line by line
		while (getline(md_file->getFile(), line)) {
			outfile << parseline(line) << endl;
		}

		md_file->closeFile();
		outfile.close();
		cout << endl;
		delete md_file;
}
