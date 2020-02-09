
#include <iostream>
#include "Parser.h"
#include <string>


int main(int args, char* argv[]) {
	using namespace std;

	string source("[]");
	int index = 0;
	Node res = addr(&source, &index);
	cout << printNode(res) << endl;
	cout << index << endl;
	return 0;
}
