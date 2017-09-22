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
					cout << "[/i]  ���w��l�X�ɦW" <<endl;
					cout << "[/l]  ���w�C�����ɦW" <<endl;
					cout << "[/o]  ���w�ت����ɦW" <<endl;
					cout << "default��:�ϥ�SRCFILE����l�X�ɦW,LISFILE1�POBJFILE1����X�ɦW" <<endl;
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
					cout << '\"' << argv[i] << "\" ���O�i�H���檺�����Υ~���R�O�B�i���檺�{���Χ妸�ɡC" << endl;
					break;
			}
		}
		else{
			cout << '\"' << argv[i] << "\" ���O�i�H���檺�����Υ~���R�O�B�i���檺�{���Χ妸�ɡC" << endl;
		}
	}
}
