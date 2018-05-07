// test3-ll(1).cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"
#include <string.h>
#define OVERFLOW -2
#define OK 1;
#define ERROR 0
#define MAXQSIZE 20
typedef int Status;
#define NONE 1//ε
#define SenLength 12

#define  STACK_INIT_SIZE  100
#define  STACKINCREMENT   10

char input[30];//输入文法
char first[30][30];//first集
char follow[30][30];//follow集
char lowercase[30];//终结符号
char uppercase[30];//非终结符号

typedef struct{//文法，->左边是head，右边是tail
	char head;
	char tail[10];
	bool flag;
}grammar;

char tm1[15], tm2[15];//tm1是table的横坐标，tm2是纵坐标
grammar M[10][10];//the parse table
grammar sentence[12] = { { 'E', "TD" }, { 'D', "ATD" }, { 'D', NONE }, { 'T', "FR" }, { 'R', "MFR" }, { 'R', NONE }, { 'F', "(E)" }, { 'F', "i" },
{ 'A', "+" }, { 'A', "-" }, { 'M', "*" }, { 'M', "/" } };//用D代替E' R代替T'


/////////////栈///////////////////
#define SElemType char

typedef struct {//栈结构
	SElemType  *base;
	SElemType  *top;
	int  stacksize;
} SqStack;


Status InitStack(SqStack &S)
{// 构造一个空栈S
	S.base = (SElemType*)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if (!S.base) exit(OVERFLOW); //存储分配失败
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return OK;
}

Status GetTop(SqStack &S, SElemType e){
	if (S.top == S.base) return ERROR;
	e = *(S.top - 1);
	return OK;
}//GetTop

Status Push(SqStack &S, SElemType e) {
	if (S.top - S.base >= S.stacksize) {//栈满，追加存储空间
		S.base = (SElemType *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemType));
		if (!S.base) exit(OVERFLOW); //存储分配失败
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return OK;
}

Status Pop(SqStack &S, SElemType &e) {
	// 若栈不空，则删除S的栈顶元素，
	// 用e返回其值，并返回OK；
	// 否则返回ERROR
	if (S.top == S.base) return ERROR;
	e = *--S.top;
	return OK;
}

Status StackEmpty(SqStack &S){
	if (S.base == S.top) { return OK; }
	else return ERROR;
}
////////////////////栈/////////////////////

void findlowercase();//提取这个文法中的终结元素
void finduppercase();//提取这个文法中的非终结元素
void FIRST();//计算first集
void FOLLOW();//计算follow集
void read();//读入专题一中的输入字符串
void initsen();//初始化文法中的每个句子
int mapfirst(char head);//给出a,求head在first(a)中的位置
int mapfollow(char head);//给出a,求head在follow(a)中的位置
int maptable(char head,char tm[]);//求head在tm[]中的位置
bool Add(char dest[], char source[]);//将source中的元素插入到dest中，重复元素不增加
bool isuppercase(char c);//判断c是否为非终结符号（大写字母）
bool islowercase(char c);//判断c是否为终结符号（小写字母）
bool hasNone(char c);//first(c)中是否有ε
bool issymbol(char c);//判断c是否为终结符号（运算符）
bool isterminal(char c);//判断c是否为终结符号（全部）
bool exist(char[], char c);//判断c是否存在于char[]中
void inittable();//初始化分析表
void maketable();//生成分析表
bool control(SqStack &S);//总控程序

int _tmain(int argc, _TCHAR* argv[])
{
	initsen();
	SqStack S;
	InitStack(S);
	read();
	maketable();
	if (control(S)){
		printf("正确\n");
	}
	else{
		printf("错误\n");
	}
	return 0;
}

bool control(SqStack &S){
	int i = 0;//the pointer to point the word being input
	char a = input[0];
	char x;
	bool flag = true;
	Push(S, '#');
	Push(S, 'E');

	while (flag == true){
		Pop(S, x);
 		if (isterminal(x)){//lower case
			if (x == a){
				a = input[++i];//NextSym
			}
			else{
				return ERROR;
			}
		}
		else if (x == '#'){
			if (x == a){
				i++;//NextSym
				flag = false;
			}
			else return ERROR;
		}
		else if (M[maptable(x,tm2)][maptable(a,tm1)].flag == true){//倒序入栈
				grammar t = M[maptable(x, tm2)][maptable(a, tm1)];
				if (t.tail[0] != 1){//说明不推ε
					int i;
					for (i = 0; t.tail[i] != '\0'; i++){}
					i--;
					for (; i >= 0; i--){
						Push(S, t.tail[i]);
					}
				}
		}
		else return ERROR;
	}
	return true;
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
	else exit(0);
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
			fseek(fp2, 1, SEEK_CUR);
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
	strcat(input, "#");
}

void FIRST(){//first集0号位是该符号本身，1号位开始是它的集合
	findlowercase();
	finduppercase();
	int upperp = 0;
	int firstp = 0;//当前first集的行指针

	bool flag = true;//判断文法是否还在变化
	while (flag){
		flag = false;

		/****************************************************************************/
		for (firstp = 0; lowercase[firstp] != '\0'; firstp++){//为终结符号时
			first[firstp][0] = first[firstp][1] = lowercase[firstp];
			//first[firstp][2] = '\0';
		}
		/****************************************************************************/
		for (upperp = 0; uppercase[upperp] != '\0'; upperp++, firstp++){//为非终结符号时
			int i = 0;
			int firstq = 1;//当前行的指针
			first[firstp][0] = uppercase[upperp];
			while (i<SenLength){//在文法中搜索一轮，找到当前所求的first集
				if (sentence[i].head == uppercase[upperp] && (isterminal(sentence[i].tail[0]) || sentence[i].tail[0] == 1)){
					if (first[firstp][firstq] != sentence[i].tail[0]){ flag = true; }
					first[firstp][firstq] = sentence[i].tail[0];
					firstq++;
				}
				i++;
			}
		}
		first[firstp][0] = '\0';
		/****************************************************************************/
		int i;
		for (i = 0; i<SenLength; i++){//每个文法都要进行第三步i，目前判断的是sentence[i]
			//i
			if (isuppercase(sentence[i].tail[0])){
				flag = flag | Add(first[mapfirst(sentence[i].head)], first[mapfirst(sentence[i].tail[0])]);
			}
		}
		for (i = 0; i < SenLength; i++){//每个文法都要进行第三步ii，目前判断的是sentence[i]
			//ii
			int I = 1;//课件中的i
			int j = I - 1;//课件中的j
			bool fl = false;
			while (isuppercase(sentence[i].tail[I]) && sentence[i].tail[I] != '\0'){
				if (!hasNone(sentence[i].tail[j])){
					break;
				}
				fl = true;
				I++;
				j = I - 1;
			}
			I--;			//此时j所指元素为第一个first集无ε的，I指向'\0'，所以应该往前移一位，之后I指向为第一个first集无ε的
			if (fl){ flag = flag | Add(first[mapfirst(sentence[i].head)], first[mapfirst(sentence[I].head)]); }

			//iii
			if (isuppercase(sentence[i].tail[I]) && sentence[i].tail[I + 1] == '\0' && I != 0){//说明I是最后一个，所有符号都是非终结符号

				char ch[1];
				ch[0] = NONE;
				flag = flag | Add(first[mapfirst(sentence[i].head)], ch);

			}
		}

	}
}

void FOLLOW(){

	int p = 0, q = 0;
	for (p = 0; first[p][0] != '\0'; p++){
		if (isuppercase(first[p][0])) { follow[q][0] = first[p][0]; q++; }
	}

	//i
	follow[0][1] = '#';
	bool flag = true;
	while (flag){
		flag = false;
		//ii
		int i = 0;
		int B = 0;		//A→ αBβ指向B的指针
		while (sentence[i].head != '\0'){
			for (B = 0; sentence[i].tail[B + 1] != '\0'; B++){
				if (isuppercase(sentence[i].tail[B])){
					flag = flag | Add(follow[mapfollow(sentence[i].tail[B])], first[mapfirst(sentence[i].tail[B + 1])]);//FIRST(β)-{ε}->FOLLLOW(B)
				}
			}

			//iii
			for (B = 0; sentence[i].tail[B + 2] != '\0'; B++){}//B找到倒数第二个符号(若只有一个符号，则B为最后一个符号）
			if (hasNone(sentence[i].tail[B + 1])){//A→ αBβ，ε属于FIRST(β)的话
				flag = flag | Add(follow[mapfollow(sentence[i].tail[B])], follow[mapfollow(sentence[i].head)]);//FOLLOW(A)->FOLLOW(B)
			}
			if (sentence[i].tail[B + 1] != '\0') B++;//B是最后一个符号
			if (isuppercase(sentence[i].tail[B])){//A→ αB
				flag = flag | Add(follow[mapfollow(sentence[i].tail[B])], follow[mapfollow(sentence[i].head)]);//FOLLOW(A)->FOLLOW(B)
			}
			i++;
		}
	}
}

void findlowercase(){
	int i = 0;//sentence的指针
	int j = 0;//lowercase的指针
	while (i<SenLength){
		int k = 0;
		for (k = 0; sentence[i].tail[k] != '\0'; k++){
			if (isterminal(sentence[i].tail[k])){
				if (!exist(lowercase, sentence[i].tail[k])){
					lowercase[j] = sentence[i].tail[k];
					j++;
				}
			}
		}
		i++;
	}
	lowercase[i] = '\0';
}

void finduppercase(){
	int i = 0;//sentence的指针
	int j = 0;//uppercase的指针
	while (i<SenLength){
		if (isuppercase(sentence[i].head)){
			if (!exist(uppercase, sentence[i].head)){
				uppercase[j] = sentence[i].head;
				j++;
			}
		}
		i++;
	}
	uppercase[i] = '\0';
}

void initsen(){
	for (int i = 0; i <SenLength; i++){
		sentence[i].flag = true;
	}
}

bool isuppercase(char c){
	if (c >= 65 && c <= 90) return true;
	else return false;
}

bool islowercase(char c){
	if (c >= 96 && c <= 122) return true;
	else return false;
}

bool issymbol(char c){
	if (c >= 40 && c <= 47) return true;
	else return false;
}

int mapfirst(char head){
	int i = 0;
	while (first[i][0] != '\0'){
		if (first[i][0] == head)
			return i;
		i++;
	}
	return -1;
}

int mapfollow(char head){
	int i = 0;
	while (follow[i][0] != '\0'){
		if (follow[i][0] == head)
			return i;
		i++;
	}
	return -1;
}

int maptable(char head,char tm[]){
	int i = 0;
	while(tm[i] != '\0'){
		if (tm[i] == head)
			return i;
		i++;
	}
	return -1;
}

bool Add(char dest[], char source[]){//数组间相加，自动忽略ε
	int d = 1, s = 0;//dest[]是first集，first集的首位存first(a)的a本身，从第二位开始读
	bool success = false;
	if (source[0] == NONE){//如果加ε
		while (dest[d] != '\0'){
			if (dest[d] == NONE){ success = false; break; }//如果已经有ε则不必再加
			d++;
		}
		if (dest[d] == '\0'){ dest[d] = NONE; dest[d + 1] = '\0'; success = true; }
	}
	else{//如果加其他，则不能加ε
		bool flag = true;
		for (s = 1, flag = true; source[s] != '\0'; s++){
			for (d = 1; dest[d] != '\0'; d++){
				if (dest[d] == source[s]){
					flag = false;//说明dest中已经有此元素，不必再加
					break;
				}
			}
			if (flag == true && source[s] != NONE){ dest[d] = source[s]; dest[d + 1] = '\0'; success = true; }//说明需要增加
			flag = true;
		}
	}
	return success;
}

bool hasNone(char c){//first(c)中是否有ε
	int i;
	int j;
	for (i = 0; first[i][0] != '\0'; i++){
		if (first[i][0] == c){
			for (j = 1; first[i][j] != '\0'; j++){
				if (first[i][j] == NONE)
					return true;
			}
		}
	}
	return false;
}

bool exist(char d[], char c){
	int i;
	for (i = 0; d[i] != '\0'; i++){
		if (c == d[i])return true;
	}
	return false;
}

bool isterminal(char c){
	if (islowercase(c) || issymbol(c)) return true;
	else return false;
}

void inittable(){//设置表头
	int i;
	for (i = 0; lowercase[i] != '\0'; i++){
		tm1[i] = lowercase[i];
	}
	tm1[i] = '#';
	for (i = 0; uppercase[i] != '\0'; i++){
		tm2[i] = uppercase[i];
	}
}

void maketable(){
	FIRST();
	FOLLOW();
	inittable();
	int i, j;
	grammar g;
	char a;
	bool flag = false;//判断是否ε属于first()
	for (i = 0; tm2[i] != '\0'; i++){
		//从sentence中找当前第i行非终结符的文法
		for (j = 0; sentence[j].head != '\0'; j++){
			if (sentence[j].head == tm2[i] && sentence[j].tail[0] != NONE){
				g = sentence[j];

				int n;
				for (n = 1; (a = first[mapfirst(g.tail[0])][n]) != '\0'; n++){//循环sentnece[i].tail[0]的first集
					//a是first(D)的元素(E->DT)
					//这段是找follow集求E->ε的部分
					if (first[mapfirst(g.head)][n] == NONE){
						flag = true;
					}//说明需要输入E->ε,查看follow集
					//////////////////////////////////
					int k;
					for (k = 0; tm1[k] != '\0'; k++){//tm1[k]是table中k号位的表头
						if (a == tm1[k]){
							M[i][k] = g;
							break;
						}
					}
				}
				if (flag == true){
					for (n = 1; (a = follow[mapfollow(tm2[i])][n]) != '\0'; n++){
						int k;
						for (k = 0; tm1[k] != '\0'; k++){
							if (tm1[k] == a ){
								M[i][k].head = tm2[i];
								M[i][k].tail[0] = NONE;
								M[i][k].flag = true;
							}
						}
						flag = false;
					}
				}
			}
		}
	}
}
