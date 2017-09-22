#include "tablib.hpp"

using namespace std;

listnode::listnode(){
	this->locctr = 0;
	this->opcode = -1;
	this->insLength = 0;
}

const map<string,int> optab{
	{"ADD" ,0x180000},
	{"AND" ,0x400000},
	{"COMP",0x280000},
	{"DIV" ,0x240000},
	{"J"   ,0x3C0000},
	{"JEQ" ,0x300000},
	{"JGT" ,0x340000},
	{"JLT" ,0x380000},
	{"JSUB",0x480000},
	{"LDA" ,0x000000},
	{"LDCH",0x500000},
	{"LDL" ,0x080000},
	{"LDX" ,0x040000},
	{"MUL" ,0x200000},
	{"OR"  ,0x440000},
	{"RD"  ,0xD80000},
	{"RSUB",0x4C0000},
	{"STA" ,0x0C0000},
	{"STCH",0x540000},
	{"STL" ,0x140000},
	//{"STSW",0xE80000},
	{"STX" ,0x100000},
	{"SUB" ,0x1C0000},
	{"TD"  ,0xE00000},
	{"TIX" ,0x2C0000},
	{"WD"  ,0xDC0000},
};

const map<string,int> psdtab{
	{"START",0},
	{"END"  ,1},
	{"BYTE" ,2},
	{"WORD" ,3},
	{"RESB" ,4},
	{"RESW" ,5},
};

map<string,int> symtab;
vector<listnode> list;
