#include "tools.hpp"

using namespace std;

bool strIsDigit(string target){
	bool result = true;
	for(int i = 0 ; i < target.length() ; i++){
		if(target[i] < '0' || '9' < target[i]){
			result = false;
			break;
		}
	}
	return result;
}

int strToInt(string src){
	int result = 0;
	if(src.length() > 0){
		for(int i=0;'0' <= src[i] && src[i] <= '9';i++) result = result*10 + src[i]-'0';
	}
	return result;
}

bool strIsHex(string target){
	bool result = true;
	for(int i = 0 ; i < target.length() ; i++){
		if('0' <= target[i] && target[i] <= '9'){}
		else if('A' <= target[i] && target[i] <= 'F'){}
		else if('a' <= target[i] && target[i] <= 'f'){}
		else{
			result = false;
			break;
		}
	}
	return result;
}

int strToHex(string src){
	int result = 0;
	if(src.length() > 0){
		for(int i=0 ; i < src.length() ; i++){
			if('0' <= src[i] && src[i] <= '9') result = (result<<4) + src[i]-'0';
			else if('A' <= src[i] && src[i] <= 'F') result = (result<<4) + src[i]-'7';
			else if('a' <= src[i] && src[i] <= 'f') result = (result<<4) + src[i]-'W';
			else break;
		}
	}
	return result;
}

int nextString(string object, int start){
	int next = start;
	unsigned char flag;
	if(start >= object.length()) return start;
	if(object[start] == ' ' || object[start] == '\t') flag = 0;
	else flag = 1;
	while(1){
		next++;
		if(next >= object.length()) return next;
		if(flag == 1){
			if(object[next] != ' ' && object[next] != '\t') continue;
			else flag = 0;
		}
		else{
			if(object[next] != ' ' && object[next] != '\t') break;
			else continue;
		}
	}
	return next;
}

int substrLength(string object, int start){
	int i = 0;
	for(;start < object.length() && object[start] != ' ' && object[start] != '\t';start++,i++){}
	return i;
}

string strToUpper(string source){
	string upper(source);
	for(int i = 0 ; i < upper.length() ; i++){
		if('a' <= upper[i] && upper[i] <= 'z') upper[i] -= 32;
	}
	return upper;
}
