#include <iostream>
#include <cstring>
#include "setting.hpp"
#include "debugger.hpp"
#include "buildlist.hpp"
#include "pass1.hpp"
#include "pass2.hpp"
#include "fileoutput.hpp"

void argumentSetting(int,char**);

int main(int argc, char** argv){
	argumentSetting(argc,argv);
	buildlist(setting::srcfile);
	//cout << "buildlist pass" << endl;
	pass1();
	//cout << "pass1 pass" << endl;
	pass2();
	//cout << "pass2 pass" << endl;
	lisFileOutput(setting::lisfile);
	//cout << "lis pass" << endl;
	objFileOutput(setting::objfile);
	//cout << "obj pass" << endl;
	
	//printLisFile(setting::lisfile);
	
	return 0;
}

void argumentSetting(int argc, char** argv){
	for(int i=1;i<argc;i++){
		if(argv[i][0] == '/' && strlen(argv[i]) == 2){
			switch(argv[i][1]){
				case '?':
					cout << "[/i]  指定原始碼檔名" <<endl;
					cout << "[/l]  指定列表檔檔名" <<endl;
					cout << "[/o]  指定目的檔檔名" <<endl;
					cout << "default值:使用SRCFILE為原始碼檔名,LISFILE1與OBJFILE1為輸出檔名" <<endl;
					break;
				case 'i':
					setting::srcfile = argv[++i];
					break;
				case 'l':
					setting::lisfile = argv[++i];;
					break;
				case 'o':
					setting::objfile = argv[++i];;
					break;
				default:
					cout << '\"' << argv[i] << "\" 不是可以執行的內部或外部命令、可執行的程式或批次檔。" << endl;
					break;
			}
		}
		else{
			cout << '\"' << argv[i] << "\" 不是可以執行的內部或外部命令、可執行的程式或批次檔。" << endl;
		}
	}
}
