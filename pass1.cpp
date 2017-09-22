#include "pass1.hpp"

using namespace std;

extern debugger debug;

void pass1(){
	int locctr = 0;
	intvec* error = NULL;
	cmapiter optest;
	
	for(veciter tmp = list.begin(); tmp!=list.end() ; tmp++){
		//if space line
		if(tmp->ins.length() == 0) continue;
		
		//error test!!!!
		tmp->locctr = locctr;
		error = debug.pass1Test(*tmp);
		if( (tmp->label).length() > 0 && !intvecFind(*error,26)){
			symtab.insert(tabpair(tmp->label,locctr));
		}
		
		//set location counter
		if( optab.find(tmp->ins) != optab.end()){
			tmp->insLength = 3;
			locctr += 3;
		}
		else if( ( optest = psdtab.find(tmp->ins) ) != psdtab.end()){
			if( intvecFind(*error,2) ) {}
			else if( (optest->second == 2) && !intvecFind(*error,7,8,9,10,11) ){
				int j = 0;
				if(tmp->operand[0] == 'C' || tmp->operand[0] == 'X'){
					for(j = 2 ; tmp->operand[j+2] != '\'' ; j++){}
					if(tmp->operand[0] == 'X'){
						if( (j&1) == 1 ) j += 2;
						j >>= 1;
					}
				}
				else{
					j = 1;
				}
				tmp->insLength = j;
				locctr += j;
			}
			else if(optest->second == 3 && !intvecFind(*error,12,20) ){
				tmp->insLength = 3;
				locctr += 3;
			}
			else if(optest->second == 4 && !intvecFind(*error,13,14) ){
				tmp->insLength = 0;
				locctr += (strToInt(tmp->operand));
			}
			else if(optest->second == 5 && !intvecFind(*error,15,16) ){
				tmp->insLength = 0;
				locctr += (strToInt(tmp->operand)*3);
			}
			else if(optest->second == 0 && !intvecFind(*error,4,5,19) ){
				tmp->insLength = 0;
				locctr = strToHex(tmp->operand);
				tmp->locctr = locctr;
				
				if( symtab.find(tmp->label) != symtab.end() ){
					symtab.erase( symtab.find(tmp->label) );
					symtab.insert( tabpair(tmp->label,locctr) );
				}
			}
			else if(optest->second == 1 ){
				tmp->insLength = 0;
			}
		}
		
		delete error;
		error = NULL;
	}
	
	return;
}
