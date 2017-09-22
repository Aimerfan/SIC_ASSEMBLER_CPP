#include "debugger.hpp"

using namespace std;

bool intvecFind(intvec& arr, ...){
	bool result = false;
	va_list numlist;
	va_start(numlist,arr);
	while(int j = va_arg(numlist,int) && !result){
		for(intvec::iterator i = arr.begin(); i != arr.end() ; i++){
			if(*i == j){
				result = true;
				break;
			}
		}
	}
	va_end(numlist);
	return result;
}

debugger debug = debugger::getDebugger();

debugger* debugger::exist = NULL;
debugger::debugger(){
	startup = false;
	processing = NULL;
}

debugger& debugger::getDebugger(){
	if(exist == NULL){
		exist = new debugger;
	}
	return *(exist);
}
void debugger::resetDebugger(){
	startup = false;
	processing = NULL;
}

intvec* debugger::pass1Test(listnode& test){
	intvec* result = new intvec;
	if(test.ins.length()){
		processing = &test;
		if(test.ins == "START" && startup) result->push_back(19);
		if(!startup && test.ins == "START") startup = true;
		if(!startup && test.ins != "START") result->push_back(22);
		result->push_back( testLabel(test) );
		result->push_back( testPsdOperand(test) );
		processing = NULL;
	}
	return result;
}
intvec* debugger::pass2Test(listnode &test){
	intvec* result = new intvec;
	processing = &test;
	if( test.comment.length() > 0 && test.comment.at(0) == '.'){}
	else result->push_back( testInsOperand(test) );
	processing = NULL;
	return result;
}

int debugger::testLabel(listnode &test){
	int result = 0;
	
	if( test.label.length() > 0 && symtab.find(test.label) != symtab.end() ) result = 26;
	if( (test.ins == "START") && (test.label.length() == 0) ) result = 3;
	setError(result);
	
	return result;
}
int debugger::testInsOperand(listnode &test){
	int result = 0;
	
	if( psdtab.find(test.ins) != psdtab.end() || !test.ins.length()){}
	else if( optab.find(test.ins) == optab.end() ) result = 17;
	else if( test.ins == "RSUB" ){
		if( test.operand.length() > 0 ) result = 23;
	}
	else if( test.operand.length() > 0 ){
		int found = test.operand.find_first_of(',');
		string str = test.operand;
		if( found != string::npos && test.operand[found+1] == 'X') str = str.substr(0,found);
		if( symtab.find(str) == symtab.end() ) result = 2; 
	}
	else result = 2;
	
	setError(result);
	return result;
}
int debugger::testPsdOperand(listnode &test){
	int result = 0;
	
	if(psdtab.find(test.ins) != psdtab.end()){
		if(test.ins == "BYTE"){
			if(test.operand.length() == 0) result = 11;
			else if(strIsDigit(test.operand)) result = false;
			else if(test.operand[0] == 'C'){
				if(test.operand[1] == '\'' && test.operand[test.operand.length()-1] == '\'') result = 0;
				else result = 8;
			}
			else if(test.operand[0] == 'X'){
				if(test.operand[1] == '\'' && test.operand[test.operand.length()-1] == '\''){
					string hex = test.operand.substr(2,test.operand.length()-3);
					if(strIsHex(hex)) result = false;
					else result = 9;
				}
				else result = 8;
			}
			else result = 7;
		}
		else if(test.ins == "WORD"){
			if(test.operand.length() == 0) result = 12;
			else if(strIsDigit(test.operand)) result = 0;
			else if(symtab.find(test.operand) != symtab.end()) result = 0;
			else result = 2;
		}
		else if(test.ins == "RESB"){
			if(test.operand.length() == 0) result = 13;
			else if(strIsDigit(test.operand)) result = false;
			else if(symtab.find(test.operand) != symtab.end()) result = 14;
			else result = 2;
		}
		else if(test.ins == "RESW"){
			if(test.operand.length() == 0) result = 15;
			else if(strIsDigit(test.operand)) result = 0;
			else if(symtab.find(test.operand) != symtab.end()) result = 16;
			else result = 2;
		}
		else if(test.ins == "START"){
			if(test.operand.length() == 0) result = 4;
			else if(!strIsHex(test.operand)) result = 5;
		}
		else if(test.ins == "END"){
			if(symtab.find(test.operand) == symtab.end()) result = 6;
		}
	}
	
	setError(result);
	return result;
}

bool debugger::setError(int errorCode){
	if(errorCode == 0) return false;
	erriter err = errtab.find(errorCode);
	string errmessage = "       **** " + err->second + "! ****\n";
	(*processing).wrongMessage = (*processing).wrongMessage.append(errmessage);
	return true;
}

const map<int,string> debugger::errtab{
	{1,"Illegal format in label field"},
	{2,"Illegal format in operation field"},
	{3,"Missing label in START statement"},
	{4,"Missing operand in START statement"},
	{5,"Illegal hex string in START statement"},
	
	{6,"Undefined label after END statement"},
	{7,"Missing right quote in BYTE statement"},
	{8,"Illegal operand in BYTE statement"},
	{9,"Illegal hex string in BYTE statement"},
	{10,"Odd length hex string in BYTE statement"},
	
	{11,"Missing operand in BYTE statement"},
	{12,"Missing operand in WORD statement"},
	{13,"Missing operand in RESB statement"},
	{14,"Operand must be absolute attribute in RESB statement"},
	{15,"Missing operand in RESW statement"},
	
	{16,"Operand must be absolute attribute in RESW statement"},
	{17,"Unrecognized operation code"},
	{18,"Multiply EXTDEF statement"},
	//useless in this Version
	{19,"Multiply START statement"},
	{20,"Undefined symbol in WORD statement"},
	
	{21,"Unrecognized register name"},
	//what is this mean?????
	{22,"Misplaced START statement"},
	{23,"Operand should not follow RSUB statement"},
	{24,"Missing Operand in Format 2 instruction"},
	//what is this mean?????
	
	{26,"Multiply label definition"},
};
