#pragma once
#include "Format.h"

typedef struct Node {
	string name;
	string data;
	vector<Node> subnode;
};
string printNode(Node target);

class S {
public:
	S(string _name, int op, vector<S> _subnode);
	S(string _name, Format _data);
	S(string _name, string _data);
	S(string _name);
	S(string _name, Node (*_ftn)(string*, int*));
	int op;
	string name;
	Node fit(string* source, int *index);
	//STR
	Format data;
	//AND, OR, REP
	vector<S> subnode;
	Node(*ftn)(string*, int*);
};