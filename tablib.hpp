#ifndef _TABLIB_
#define _TABLIB_

#include <string>
#include <vector>
#include <map>

using namespace std;

struct listnode{
	listnode();
	int locctr;
	int opcode;
	int insLength;
	string data;
	string label;
	string ins;
	string operand;
	string comment;
	string wrongMessage;
};

typedef pair<string,int> tabpair;
typedef map<string,int>::const_iterator cmapiter;
typedef map<string,int>::iterator mapiter;
typedef vector<listnode>::iterator veciter;

extern const map<string,int> optab;
extern const map<string,int> psdtab;
extern map<string,int> symtab;
extern vector<listnode> list;

#endif
