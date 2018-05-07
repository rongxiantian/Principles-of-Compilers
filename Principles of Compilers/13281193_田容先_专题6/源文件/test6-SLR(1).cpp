// test6-SLR(1).cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"
#include "string.h"

#define OVERFLOW -2
#define OK 1;
#define ERROR 0
#define MAXQSIZE 20
#define  STACK_INIT_SIZE  100
#define  STACKINCREMENT   10


typedef int Status;

typedef int state;

typedef struct{
	char c;//char字符
	state s;//state状态
	char* o;//归约出当前符号的终结符号，用来最后输出
}SElemType;//char字符 state状态

typedef struct{
	char type[5];//s,r,a,E
}TableType;

typedef struct{//文法，->左边是head，右边是tail
	char head;
	char tail[10];
}grammar;

int NextSym;//当前输入指针
char input[30]="i=(i+i)*i+i**#";//输入字符串
grammar sentence[11] = { { 'S', "A" }, { 'A', "V=E" }, //1 2
						{ 'E', "E+T" }, //{ 'E', "E-T" },//3
						{ 'E', "T" }, { 'T', "T*F" }, //4 5
						//{ 'T', "T/F" }, 
						{ 'T', "F" }, //6
						{ 'F', "(E)" }, { 'F', "i" }, { 'V', "i" }};//7 8 9
TableType ACTION[16][7] = {
// i	    =		  +		  *		 (		  )		   #
{ "s03",	 "E",	 "E",	 "E",	 "E",	 "E",	 "acc" },//0
{ "E",		 "E",	 "E",	 "E",	 "E",	 "E",	 "r01" },//1
{ "E",		"s04",	 "E",	 "E",	 "E",	 "E",	 "E" }, //2
{ "E",		"r09",	 "E",	 "E",	 "E",	 "E",	 "E" }, //3
{ "s09",	 "E",	 "E",	 "E",	 "s08",	 "E",	 "E" }, //4
{ "E",		 "E",	 "s10",	 "E",	 "E",	 "E",	 "r02" }, //5
{ "E",		"E",	 "r04",	 "s11",	 "E",	 "r04",	 "r04" }, //6
{ "E",		 "E",	 "r06",	 "r06",	 "E",	 "r06",	 "r06" }, //7
{ "s09",	 "E",	 "E",	 "E",	 "s08",	 "E",	 "E" },//8
{ "E",		 "E",	 "r08",	 "r08",	 "E",	 "r08",	 "r08" },//9
{ "s09",	 "E",	 "E",	 "E",	 "s08",	 "E",	 "E" },//10
{ "s09",	 "E",	 "E",	 "E",	 "s08",	 "E",	 "E" },//11
{ "E",		"E",	 "s10",	 "E",	 "E",	 "s15",	 "E" },//12
{ "E",		 "E",	 "r03",	 "s11",	 "E",	 "r03",	 "r03" }, //13
{ "E",		 "E",	 "r05",	 "r05",	 "E",	 "r05",	 "r05" },//14
{ "E",		"E",	 "r07",	 "r07",	 "E",	 "r07",	 "r07" }, //15
};
// i    =    +    -    *	/	 (    )	   #

int GOTO[16][6] = {
//	S		A		V		E		T		F		i		=		+		-		*		/		(		)		#
	0,		1,		2,		0,		0,	    0,		//0
	0,		0,		0,		0,		0,	    0,		//1
	0,		0,		0,		0,		0,	    0,		//2
	0,		0,		0,		0,		0,	    0,		//3
	0,		0,		0,		5,		6,	    7,		//4
	0,		0,		0,		0,		0,	    0,		//5
	0,		0,		0,		0,		0,	    0,		//6
	0,		0,		0,		0,		0,	    0,		//7
	0,		0,		0,		12,		6,	    7,		//8
	0,		0,		0,		0,		0,	    0,		//9
	0,		0,		0,		0,		13,	    7,		//10
	0,		0,		0,		0,		0,	    14,		//11
	0,		0,		0,		0,		0,	    0,		//12
	0,		0,		0,		0,		0,	    0,		//13
	0,		0,		0,		0,		0,	    0,		//14
	0,		0,		0,		0,		0,	    0,		//15
};


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

Status GetTop(SqStack &S, SElemType &e){
	if (S.top == S.base) return ERROR;
	e = *(S.top - 1);
	return OK;
}//GetTop

char* Get(SqStack &S, int num){//取元素，num：栈顶是1，往下一个是2，以此类推
	SElemType e;
	if (S.top == S.base) return ERROR;
	e = *(S.top - num);
	return e.o;
}

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


void analyze(SqStack S);//SLR(1)分析函数
SElemType readS(SqStack);//用来读入当前字符和下一个状态
int stateof(TableType);//求这个表项中的数字（如s10，结果就是10）
int mapA(char c);//根据c求c在ACTION表中的位置
int mapG(char c);//根据c求c在GOTO表中的位置
//////////////////////////////////

char* NewTemp();//生成新的变量
int GEN(int Op, int Arg1, int Arg2, int Result);//将四元式输出到文件里
char* work(SqStack S, int num);//根据num决定要进行哪步语义分析
////////




typedef struct{
	char Type;
	int PLACE;
}SymbolTable;

typedef struct{
	int Op;
	int Arg1;
	int Arg2;
	int Result;
}FourTable;

char* TT[6] = {"T1","T2","T3","T4","T5","T6"};
int Tp = 0;//临时变量表指针


int _tmain(int argc, _TCHAR* argv[])
{
	SqStack S;
	InitStack(S);
	analyze(S);
	return 0;
}

void analyze(SqStack S){//总控程序
	SElemType e = { '#' ,0, NULL };
	Push(S, e);
	while (1){
		e = readS(S);
		TableType temp = ACTION[e.s][mapA(input[NextSym])];
		if (temp.type[0] == 's'){//读状态
			char c = input[NextSym++];
			int s = stateof(temp);//求字符串中状态
			Push(S, { c, s,NULL }); //[当前输入字符][根据这个字符推出来的状态]
		}
		else if (temp.type[0] == 'r'){//规约状态
			grammar g = sentence[stateof(temp)-1];//这个是要归约的句子

			/********/
			char* tem = work(S, stateof(temp) - 1);//这里返回终结字符，或者临时变量，之后这个值要压栈
			/********/
			int max = strlen(g.tail);
			int i;
			SElemType f;//临时生成，没什么用
			for (i = 0; i < max; i++){
				Pop(S, f);
			}
			e = readS(S);
			int T = GOTO[e.s][mapG(g.head)];//[当前状态]["B"]将表中内容读出来 B->β
			Push(S, { g.head, T, tem }); //[当前输入字符][根据这个字符推出来的状态][归约出他的符号]
			if (g.head == 'S'){printf("成功");break;}
		}
		else{
			printf("ERROR");
			exit(0);
		}
	}
}

SElemType readS(SqStack S){
	SElemType e;
	GetTop(S, e);
	return e;
}

int stateof(TableType temp){
	return (temp.type[1] - 48) * 10 + (temp.type[2] - 48);
}

int mapA(char c){
	switch (c){
	case 'i':return 0;
	case '=':return 1;
	case '+':return 2;
	case '*':return 3;
	case '(':return 4;
	case ')':return 5;
	case '#':return 6;
	}
}

int mapG(char c){
	switch (c){
	case 'S':return 0;
	case 'A':return 1;
	case 'V':return 2;
	case 'E':return 3;
	case 'T':return 4;
	case 'F':return 5;
	}
}

char* NewTemp(){
	return TT[Tp++];
}

void GEN(int Op, char* Arg1, char* Arg2, char* Result){
	printf("(%c %s %s %s)\n",Op,Arg1,Arg2,Result);
	FILE *fp2;
	if ((fp2 = fopen("out.txt", "ab")) == NULL)//打开操作不成功
	{
		printf("open error");
		exit(1);//结束程序的执行
	}
	fprintf(fp2,"(%c %s %s %s) ", Op, Arg1, Arg2, Result);
	fclose(fp2);
}

char* work(SqStack S,int num){
	switch (num+1){
	case 1:{
		return ""; 
	}
	case 2:	{
			GEN('=', Get(S, 1), "_", Get(S, 3));
			return Get(S, 3);
	}
	case 3:{
		char * temp = NewTemp();
		GEN('+', Get(S, 3), Get(S, 1), temp);
		return temp;
	}
	case 4:{
		return Get(S, 1);
	}
	case 5:{
		char *temp = NewTemp();
		GEN('*', Get(S, 3), Get(S, 1), temp);
		return temp;
	}
	case 6:{
		return Get(S, 1);
	}
	case 7:{
		return Get(S, 2);
	}
	case 8:{
		char *temp = new char[3];
		temp[0] = (*(S.top - 1)).c;
		temp[1] = 0;
		return temp;
	}
	case 9:{
		char *temp = new char[3];
		//char temp[3];用这个的话，字符串生成在栈中，会被程序覆盖掉
		temp[0] = (*(S.top - 1)).c;
		temp[1] = 0;
		return temp;
	}
	}
}

