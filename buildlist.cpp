#include "buildlist.hpp"

using namespace std;

void buildlist(string filename){
	ifstream srcfile(filename);
	if(!srcfile){
		if(filename.compare("SRCFILE") != 0){
			cout << '\"' << filename << '\"' << " 檔案可能不存在或毀損，嘗試使用預設檔名開啟原始碼檔。" << endl;
			srcfile.open("SRCFILE");
		}
		if(!srcfile){
			cout << "\"SRCFILE\" 不存在或毀損，請檢察原始碼是否已經正確儲存。" << endl;
			exit(1);
		}
	}
	
	string line;
	listnode* tmp = NULL;
	
	while(getline(srcfile,line)){
		//if space line
		if(line.length() == 0) continue;
		
		tmp = lineCutter(line);
		if(!tmp) continue;
		
		list.push_back(*tmp);
		delete tmp;
		tmp = NULL;
	}
	
	return;
}

listnode* lineCutter(string line){
	int i = 0;
	string sub;
	listnode* tmp = new listnode;
	
	//check comment line or space line
	if(line.at(i) == ' ' || line.at(i) == '\t') i = nextString(line,i);
	if(i == line.length()){
		delete tmp;
		return NULL;
	}
	else if(line.at(i) == '.'){
		tmp->comment = line.substr(i,line.length() - i);
		return tmp;
	}
	
	sub = line.substr(i,substrLength(line,i));
	if( (optab.find(sub) == optab.end()) && (psdtab.find(sub) == psdtab.end()) ){
		tmp->label = sub;
		i = nextString(line,i);
		sub = line.substr(i,substrLength(line,i));
	}
	tmp->ins = strToUpper(sub);
	i = nextString(line,i);
	sub = line.substr(i,substrLength(line,i));
	if(tmp->ins == "BYTE" && sub.length() > 0 ){
		for(int j = i, space = 0 ; j < line.length() ; j++){
			if( line[j] == '\'' ) space++;
			if( space == 2 ){
				sub = line.substr(i,j-i+1);
				i = j;
				break;
			} 
		}
		if(!sub.length()){}
		else if(sub.at(0) == 'c') sub.at(0) = 'C';
		else if(sub.at(0) == 'x') sub.at(0) = 'X';
		
		tmp->operand = sub;
		i = nextString(line,i);
		sub = line.substr(i,substrLength(line,i));
	}
	else if(tmp->ins != "RSUB"){
		tmp->operand = sub;
		i = nextString(line,i);
		sub = line.substr(i,substrLength(line,i));
	}
	sub = line.substr(i,line.length() - i);
	tmp->comment = sub;
	
	return tmp;
}
