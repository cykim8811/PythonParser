
#include <iostream>
#include "Parser.h"
#include <string>


int main(int args, char* argv[]) {
	using namespace std;
	string source("\nimport math\n\na = [1, 2, 3]\nd = a, (3, 2)\nif a[2] == 3:\n  a.append(4)\n  a.append(5)\nelse:\n  a.append(6)\nprint(a)\ndef p(q, r):\n  print(s)\n  return k");
	source = "if a:\n  b\nelif c:\n  d\nelse:\n  e\nf";

	int index = 0;
	Node res = parse_source(&source, &index);
	cout << "====================" << endl;
	cout << printNode(res) << endl;
	cout << "====================" << endl;
	cout << reformat(res) << endl;
	cout << "====================" << endl;
	cout << index << endl;
	cout << source.size() << endl;
	cout << "====================" << endl;

	return 0;
}
