#include "pass2.hpp"

using namespace std;

extern debugger debug;

void pass2(){
	int opcode = 0;
	intvec* error = NULL;
	mapiter operand;
	cmapiter ins;
	
	for(veciter i = list.begin();i != list.end();i++){
		opcode = 0;
		error = debug.pass2Test(*i);
		
		//test comment line
		if(i->ins.empty()) continue;
		
		//combination opcode if instruction is exist
		if( intvecFind(*error,17) ){}
		else if( ( ins = optab.find(i->ins) ) != optab.end()){
			opcode = ins->second;
			//if operation field is wrong, not to try combination opcode
			if( intvecFind(*error,2,23) ){}
			else if( (operand = symtab.find(i->operand)) != symtab.end() ){
				opcode += operand->second;
			}
			//npos to indicate no matches
			else if(i->operand.find(",X") != string::npos){
				int j = 0;
				for(j = 0;(i->operand)[j] != ',';j++){}
				string Xusedoperand((i->operand).substr(0,j));
				opcode += symtab.find(Xusedoperand)->second;
				opcode += 0x8000;
			}
			i->opcode = opcode;
			
			char* data = new char[9];
			sprintf(data,"%06X",opcode);
			i->data.assign(data);
			delete data;
		}
		else if( ( ins = psdtab.find(i->ins) ) != psdtab.end() ){
			if(ins->second == 2){
				//if operation field is wrong, not to try combination opcode
				if( i->wrongMessage.find("BYTE") != string::npos ) continue;
				
				else if((i->operand)[0] == 'C' && (i->operand)[1] == '\'' && (i->operand)[(i->operand).length()-1] == '\''){
					int chtohex = 0;
					char* strhex = new char[3];
					for(int j = 2;(i->operand)[j] != '\'';j++){
						chtohex = (i->operand)[j];
						sprintf(strhex,"%2X",chtohex);
						i->data = i->data.append(strhex);
					}
					delete strhex;
				}
				else if((i->operand)[0] == 'X' && (i->operand)[1] == '\'' && (i->operand)[(i->operand).length()-1] == '\''){
					for(int j = 2;(i->operand)[j] != '\'';j++){
						//less the check(hex char)
						i->data.push_back((i->operand)[j]);
					}
				}
				else if(strIsDigit((i->operand))){
					i->opcode = strToInt(i->operand);
					if(i->opcode > 255){
						i->opcode = i->opcode & 255;
						cout << i->opcode;
					}
					
					char* data = new char[9];
					sprintf(data,"%02X",i->opcode);
					i->data.assign(data);
					delete data;
				}
			}
			else if((ins->second == 3)){
				//if operation field is wrong, not to try combination opcode
				if( i->wrongMessage.find("WORD") != string::npos ) continue;
				
				if( strIsDigit(i->operand) ) i->opcode = stoi(i->operand,NULL);
				else i->opcode = symtab.find(i->operand)->second;
				
				char* data = new char[9];
				sprintf(data,"%06X",(i->opcode));
				i->data.assign(data);
				delete data;
			} 
		}
		delete error;
		error = NULL;
	}
	
	return;
}
