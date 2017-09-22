#include "fileoutput.hpp"

using namespace std;

void lisFileOutput(string lisFileName){
	ofstream listfile(lisFileName);
	if(!listfile){
		if(lisFileName.compare("LISFILE1") != 0){
			cout << '\"' << lisFileName << '\"' << " 檔案無法正常開啟，嘗試使用預設檔名輸出列表檔。" << endl;
			listfile.open("LISFILE1");
		}
		if(!listfile){
			cout << "\"LISFILE1\" 無法正確開啟，請檢察磁碟機狀態。" << endl;
			exit(1);
		}
	}
	
	listfile << hex << setiosflags(ios::left | ios::uppercase);
	listfile << "SIC Assembler V2.51 source_version" << endl;
	listfile << "by RUEI_ZIH CHEIN 2017/04" << endl << endl;
	listfile << "loc  opcode label    ins     operand            comment" << endl;
	listfile << "---- ------ -------- ------- ------------------ ----------" << endl;
	for(veciter i = list.begin();i != list.end();i++){
		if((i->ins).length() == 0){
			listfile << "            ";
			listfile << i->comment << endl;
			continue;
		}
		// 4 + ' ' for location counter
		listfile << setiosflags(ios::right) << setfill('0');
		listfile << setw(4) << i->locctr << ' ';
		listfile << resetiosflags(ios::right) << setfill(' ');
		
		// 6 + ' ' for opcode
		bool overDataFlag = false;
		listfile << setfill('0');
		if(psdtab.find(i->ins)->second == 2){
			if( (i->data).length() > 6){
				for(int j = 0 ; j < 6 ; j++) listfile << (i->data)[j];
				listfile << ' ';
				overDataFlag = true;
			}
			else{
				listfile << setfill(' ');
				listfile << setw(6) << i->data << ' ';
				listfile << setfill('0');
			}
		}
		else if(i->opcode < 0) listfile << "       ";
		else{
			listfile << setiosflags(ios::right);
			listfile << setw(6) << i->data << ' ';
			listfile << resetiosflags(ios::right);
		}
		
		// 8 + ' ' for label
		listfile << setfill(' ');
		listfile << setw(8) << i->label  << ' ';
		
		// 7 + ' ' for instruction
		listfile << setw(7) << i->ins    << ' ';
		
		// 18 + ' ' for operand
		listfile << setw(18)<< i->operand<< ' ';
		
		// unlimit length for comment
		listfile << i->comment << endl;
		
		if(overDataFlag){
			for(int j = 6 ; j < (i->data).length() ; j += 6){
				listfile << "     ";
				for(int k = j ; k < j+6 && k < (i->data).length() ; k++){
					listfile << (i->data)[k];
				}
				listfile << endl;
			}
		}
		
		if(i->wrongMessage.length()){
			listfile << i->wrongMessage;
		}
	}
	listfile.close();
}
void objFileOutput(string objFileName){
	ofstream objfile(objFileName);
	if(!objfile){
		if(objFileName.compare("OBJFILE1") != 0){
			cout << '\"' << objFileName << '\"' << " 檔案無法正常開啟，嘗試使用預設檔名輸出目的檔。" << endl;
			objfile.open("OBJFILE1");
		}
		if(!objfile){
			cout << "\"OBJFILE1\" 無法正確開啟，請檢察磁碟機狀態。" << endl;
			exit(1);
		}
	}
	
	objfile << hex << setiosflags(ios::uppercase) << setfill('0');
	int cardStart = (list.begin()->locctr);
	int programStart = list.begin()->locctr;
	if( symtab.find((list.rbegin()->operand)) != symtab.end() ) programStart = (symtab.find((list.rbegin()->operand)))->second;
	int programLength = (list.rbegin()->locctr) - (list.begin()->locctr);
	veciter i = list.begin();
	if(psdtab.find(i->ins) != psdtab.end() && psdtab.find(i->ins)->second == 0){
		//write 'H' card 
		objfile << setiosflags(ios::left) << setfill(' ');
		objfile << 'H' << setw(6) << i->label;
		objfile << resetiosflags(ios::left) << setfill('0');
		objfile << setw(6) << i->locctr << setw(6) << programLength << endl;
	}
	//write 'T' cards
	for(veciter j = ++i; i != list.end() && psdtab.find(i->ins)->second != 1 ; i = j){
		int cardLength = 0;
		for(;(cardLength + j->insLength) <= 30;j++){
			if(j == list.end() || psdtab.find(j->ins)->second == 1) break;
			else if(psdtab.find(j->ins)->second == 4 || psdtab.find(j->ins)->second == 5) break;
			cardLength += j->insLength;
		}
		if(cardLength > 0){
			objfile << 'T' << setw(6) << cardStart << setw(2) << cardLength;
			for(veciter k = i; k != j ; k++){
				if((k->insLength) >= 3) objfile << setw(6) << k->data;
				else if((k->insLength) > 0) objfile << k->data;
			}
			objfile << endl;
		}
		for(;j != list.end();j++){
			if(!j->ins.length()) continue;
			else if(psdtab.find(j->ins)->second == 4) continue;
			else if(psdtab.find(j->ins)->second == 5) continue;
			else break;
		}
		cardStart = j->locctr;
	}
	//write 'E' card
	objfile << 'E' << setw(6) << programStart << endl;
	objfile.close();
}

void listprint(){
	cout << hex << setiosflags(ios::left | ios::uppercase);
	cout << "loc  opcode label    ins     operand            comment" << endl;
	cout << "---- ------ -------- ------- ------------------ ----------" << endl;
	for(veciter i = list.begin();i != list.end();i++){
		if((i->ins).length() == 0){
			cout << "   comment: ";
			cout << i->comment << endl;
			continue;
		}
		// 4 + ' ' for location counter
		cout << setiosflags(ios::right) << setfill('0');
		cout << setw(4) << i->locctr << ' ';
		cout << resetiosflags(ios::right) << setfill(' ');
		
		// 6 + ' ' for opcode
		bool overDataFlag = false;
		cout << setfill('0');
		if(psdtab.find(i->ins)->second == 2){
			if( (i->data).length() > 6){
				for(int j = 0 ; j < 6 ; j++) cout << (i->data)[j];
				cout << ' ';
				overDataFlag = true;
			}
			else{
				cout << setfill(' ');
				cout << setw(6) << i->data << ' ';
				cout << setfill('0');
			}
		}
		else if(i->opcode < 0) cout << "       ";
		else{
			cout << setiosflags(ios::right);
			cout << setw(6) << i->data << ' ';
			cout << resetiosflags(ios::right);
		}
		
		// 8 + ' ' for label
		cout << setfill(' ');
		cout << setw(8) << i->label  << ' ';
		
		// 7 + ' ' for instruction
		cout << setw(7) << i->ins    << ' ';
		
		// 18 + ' ' for operand
		cout << setw(18)<< i->operand<< ' ';
		
		// unlimit length for comment
		cout << i->comment << endl;
		
		if(overDataFlag){
			for(int j = 6 ; j < (i->data).length() ; j += 6){
				cout << "     ";
				for(int k = j ; k < j+6 && k < (i->data).length() ; k++){
					cout << (i->data)[k];
				}
				cout << endl;
			}
		}
		
		if(i->wrongMessage.length()){
			cout << i->wrongMessage;
		}
	}
}

void printLisFile(string lisFileName){
	ifstream lisfile(lisFileName);
	string printlis;
	while(getline(lisfile,printlis)){
		cout << printlis << endl;
	}
	return;
}
