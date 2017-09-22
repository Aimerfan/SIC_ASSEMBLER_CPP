#ifndef _DEBUGGER_
#define _DEBUGGER_

#include <cstdarg>
#include <string>
#include <vector>
#include <map>
#include "tablib.hpp"
#include "tools.hpp"

typedef map<int,string>::const_iterator erriter;
typedef vector<int> intvec;
bool intvecFind(intvec&, ...);

class debugger{
	public:
		static debugger& getDebugger();
		void resetDebugger();
		intvec* pass1Test(listnode&);
		intvec* pass2Test(listnode&);
	private:
		debugger();
		int testLabel(listnode&);
		int testInsOperand(listnode&);
		int testPsdOperand(listnode&);
		bool setError(int);
	private:
		bool startup;
		listnode* processing;
		static debugger* exist;
		static const map<int,string> errtab; 
};
extern debugger debug;

#endif
