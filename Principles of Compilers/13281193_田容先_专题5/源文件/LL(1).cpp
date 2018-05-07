// test2-ll(1)down.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "string.h"
#include<cstdlib>
#include "LL(1).h"

char current;
int NextSym = 0;
char input[30];


bool equal(char id[]){
	int i = 0;
	int length = strlen(id);
	char token[10];
	while (i < length&&i<10){
		token[i] = input[NextSym++];
		i++;
	}
	token[i] = '\0';
	if (strcmp(token, id)){
		printf("token:%s 与id:%s不等\n", token, id);
		rollback(id);
		return false;
	}
	printf("token:%s 与id:%s相等\n", token, id);
	rollback(id);
	return true;
}

void rollback(char* id){
	NextSym -= strlen(id);
}

void Next(char* id){
	NextSym += strlen(id);
}

bool E(){
	if (!(equal("i") || equal("("))){ return false; }
	if (!T()){ return false; }
	if (!E_()){ return false; }
	return true;
}

bool E_(){
	while (1){
		if (equal("+") || equal("-")){
			if (!A()){ return false; }
			if (!T()){ return false; }
		}
		else{
			return true;
		}
	}
}

bool T(){
	if (!(equal("i") || equal("("))){ return false; }
	if (!F()){ return false; }
	if (!T_()){ return false; }
	return true;
}

bool T_(){
	while (1){
		if (equal("*") || equal("/")){
			if (!M()){ return false; }
			if (!F()){ return false; }
		}
		else{
			return true;
		}
	}
}

bool F(){
	if (equal("(")){
		Next("(");
		if (!E()){ return false; }
		if (!equal(")")){ return false; }
		Next(")");
		return true;
	}
	else{
		if (equal("i")){ Next("i");  return true; }
	}
}

bool M(){
	if (equal("*")){ Next("*"); return true; }
	else{
		if (equal("/")){ return true; }
		return false;
	}
}

bool A(){
	if (equal("+")){ Next("+"); return true; }
	else{
		if (equal("-")){ return true; }
		return false;
	}
}

void read(){
	FILE* fp2;
	char num[2], defined[20];
	char *reserved;
	int i, c;
	if ((fp2 = fopen("out.txt", "r")) == NULL)//打开操作不成功
	{
		printf("open error");
		exit(1);//结束程序的执行
	}
	fseek(fp2, 1, SEEK_CUR);
	while (1){
		if (!fgets(num, 3, fp2))break;
		if (iden(num) == NULL){
			fseek(fp2, 1, SEEK_CUR);//空格
			for (i = 0;; i++){
				c = fgetc(fp2);
				if (c == 32){ fseek(fp2, -1, SEEK_CUR); break; }/*space ascii:32*/
				defined[i] = c;
			}
			defined[i] = '\0';
			strcat(input, defined);
		}
		else{
			reserved = iden(num);
			strcat(input, reserved);
		}
		fseek(fp2, 1, SEEK_CUR);
	}
}

char* iden(char* num){
	if (!strcmp(num, "01"))return "void";
	else if (!strcmp(num, "02")) return "int";
	else if (!strcmp(num, "03")) return "float";
	else if (!strcmp(num, "04")) return "double";
	else if (!strcmp(num, "05")) return "if";
	else if (!strcmp(num, "06")) return "else";
	else if (!strcmp(num, "07")) return "for";
	else if (!strcmp(num, "08")) return "do";
	else if (!strcmp(num, "09")) return "while";
	else if (!strcmp(num, "10")) return NULL;
	else if (!strcmp(num, "11")) return NULL;
	else if (!strcmp(num, "12")) return "+";
	else if (!strcmp(num, "13")) return "-";
	else if (!strcmp(num, "14")) return "*";
	else if (!strcmp(num, "15")) return ";";
	else if (!strcmp(num, "16")) return ",";
	else if (!strcmp(num, "17")) return "(";
	else if (!strcmp(num, "18")) return ")";
	else if (!strcmp(num, "19"))return "{";
	else if (!strcmp(num, "20")) return "}";
	else if (!strcmp(num, "21")) return "<";
	else if (!strcmp(num, "22"))return "=";
	else if (!strcmp(num, "23"))return ">";
	else if (!strcmp(num, "24"))return "/";
	else if (!strcmp(num, "25"))return "!";
	else if (!strcmp(num, "26"))return ">=";
	else if (!strcmp(num, "27"))return "<=";
	else if (!strcmp(num, "28"))return "!=";
	else if (!strcmp(num, "29"))return "==";
	else if (!strcmp(num, "30"))return NULL;
	else if (!strcmp(num, "31"))return "++";
	else if (!strcmp(num, "32"))return "--";
	else if (!strcmp(num, "33"))return "<<";
	else if (!strcmp(num, "34"))return ">>";
	else if (!strcmp(num, "35"))return "+=";
	else if (!strcmp(num, "36"))return "-=";
	else if (!strcmp(num, "37"))return "*=";
	else if (!strcmp(num, "38"))return "/=";
	else if (!strcmp(num, "39"))return "\"";
	else if (!strcmp(num, "40"))return "%d";
	else if (!strcmp(num, "41"))return "%c";
	else if (!strcmp(num, "42"))return "%s";
	else if (!strcmp(num, "43"))return "%lf";
	else if (!strcmp(num, "44"))return "%";
	else if (!strcmp(num, "45"))return "\\";
	else return "";
}