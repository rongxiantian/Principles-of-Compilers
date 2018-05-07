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
#define NONE 1//��
#define SenLength 12

#define  STACK_INIT_SIZE  100
#define  STACKINCREMENT   10

char input[30];//�����ķ�
char first[30][30];//first��
char follow[30][30];//follow��
char lowercase[30];//�ս����
char uppercase[30];//���ս����

typedef struct{//�ķ���->�����head���ұ���tail
	char head;
	char tail[10];
	bool flag;
}grammar;

char tm1[15], tm2[15];//tm1��table�ĺ����꣬tm2��������
grammar M[10][10];//the parse table
grammar sentence[12] = { { 'E', "TD" }, { 'D', "ATD" }, { 'D', NONE }, { 'T', "FR" }, { 'R', "MFR" }, { 'R', NONE }, { 'F', "(E)" }, { 'F', "i" },
{ 'A', "+" }, { 'A', "-" }, { 'M', "*" }, { 'M', "/" } };//��D����E' R����T'


/////////////ջ///////////////////
#define SElemType char

typedef struct {//ջ�ṹ
	SElemType  *base;
	SElemType  *top;
	int  stacksize;
} SqStack;


Status InitStack(SqStack &S)
{// ����һ����ջS
	S.base = (SElemType*)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if (!S.base) exit(OVERFLOW); //�洢����ʧ��
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
	if (S.top - S.base >= S.stacksize) {//ջ����׷�Ӵ洢�ռ�
		S.base = (SElemType *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemType));
		if (!S.base) exit(OVERFLOW); //�洢����ʧ��
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return OK;
}

Status Pop(SqStack &S, SElemType &e) {
	// ��ջ���գ���ɾ��S��ջ��Ԫ�أ�
	// ��e������ֵ��������OK��
	// ���򷵻�ERROR
	if (S.top == S.base) return ERROR;
	e = *--S.top;
	return OK;
}

Status StackEmpty(SqStack &S){
	if (S.base == S.top) { return OK; }
	else return ERROR;
}
////////////////////ջ/////////////////////

void findlowercase();//��ȡ����ķ��е��ս�Ԫ��
void finduppercase();//��ȡ����ķ��еķ��ս�Ԫ��
void FIRST();//����first��
void FOLLOW();//����follow��
void read();//����ר��һ�е������ַ���
void initsen();//��ʼ���ķ��е�ÿ������
int mapfirst(char head);//����a,��head��first(a)�е�λ��
int mapfollow(char head);//����a,��head��follow(a)�е�λ��
int maptable(char head,char tm[]);//��head��tm[]�е�λ��
bool Add(char dest[], char source[]);//��source�е�Ԫ�ز��뵽dest�У��ظ�Ԫ�ز�����
bool isuppercase(char c);//�ж�c�Ƿ�Ϊ���ս���ţ���д��ĸ��
bool islowercase(char c);//�ж�c�Ƿ�Ϊ�ս���ţ�Сд��ĸ��
bool hasNone(char c);//first(c)���Ƿ��Ц�
bool issymbol(char c);//�ж�c�Ƿ�Ϊ�ս���ţ��������
bool isterminal(char c);//�ж�c�Ƿ�Ϊ�ս���ţ�ȫ����
bool exist(char[], char c);//�ж�c�Ƿ������char[]��
void inittable();//��ʼ��������
void maketable();//���ɷ�����
bool control(SqStack &S);//�ܿس���

int _tmain(int argc, _TCHAR* argv[])
{
	initsen();
	SqStack S;
	InitStack(S);
	read();
	maketable();
	if (control(S)){
		printf("��ȷ\n");
	}
	else{
		printf("����\n");
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
		else if (M[maptable(x,tm2)][maptable(a,tm1)].flag == true){//������ջ
				grammar t = M[maptable(x, tm2)][maptable(a, tm1)];
				if (t.tail[0] != 1){//˵�����Ʀ�
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
	if ((fp2 = fopen("out.txt", "r")) == NULL)//�򿪲������ɹ�
	{
		printf("open error");
		exit(1);//���������ִ��
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

void FIRST(){//first��0��λ�Ǹ÷��ű���1��λ��ʼ�����ļ���
	findlowercase();
	finduppercase();
	int upperp = 0;
	int firstp = 0;//��ǰfirst������ָ��

	bool flag = true;//�ж��ķ��Ƿ��ڱ仯
	while (flag){
		flag = false;

		/****************************************************************************/
		for (firstp = 0; lowercase[firstp] != '\0'; firstp++){//Ϊ�ս����ʱ
			first[firstp][0] = first[firstp][1] = lowercase[firstp];
			//first[firstp][2] = '\0';
		}
		/****************************************************************************/
		for (upperp = 0; uppercase[upperp] != '\0'; upperp++, firstp++){//Ϊ���ս����ʱ
			int i = 0;
			int firstq = 1;//��ǰ�е�ָ��
			first[firstp][0] = uppercase[upperp];
			while (i<SenLength){//���ķ�������һ�֣��ҵ���ǰ�����first��
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
		for (i = 0; i<SenLength; i++){//ÿ���ķ���Ҫ���е�����i��Ŀǰ�жϵ���sentence[i]
			//i
			if (isuppercase(sentence[i].tail[0])){
				flag = flag | Add(first[mapfirst(sentence[i].head)], first[mapfirst(sentence[i].tail[0])]);
			}
		}
		for (i = 0; i < SenLength; i++){//ÿ���ķ���Ҫ���е�����ii��Ŀǰ�жϵ���sentence[i]
			//ii
			int I = 1;//�μ��е�i
			int j = I - 1;//�μ��е�j
			bool fl = false;
			while (isuppercase(sentence[i].tail[I]) && sentence[i].tail[I] != '\0'){
				if (!hasNone(sentence[i].tail[j])){
					break;
				}
				fl = true;
				I++;
				j = I - 1;
			}
			I--;			//��ʱj��ָԪ��Ϊ��һ��first���ަŵģ�Iָ��'\0'������Ӧ����ǰ��һλ��֮��Iָ��Ϊ��һ��first���ަŵ�
			if (fl){ flag = flag | Add(first[mapfirst(sentence[i].head)], first[mapfirst(sentence[I].head)]); }

			//iii
			if (isuppercase(sentence[i].tail[I]) && sentence[i].tail[I + 1] == '\0' && I != 0){//˵��I�����һ�������з��Ŷ��Ƿ��ս����

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
		int B = 0;		//A�� ��B��ָ��B��ָ��
		while (sentence[i].head != '\0'){
			for (B = 0; sentence[i].tail[B + 1] != '\0'; B++){
				if (isuppercase(sentence[i].tail[B])){
					flag = flag | Add(follow[mapfollow(sentence[i].tail[B])], first[mapfirst(sentence[i].tail[B + 1])]);//FIRST(��)-{��}->FOLLLOW(B)
				}
			}

			//iii
			for (B = 0; sentence[i].tail[B + 2] != '\0'; B++){}//B�ҵ������ڶ�������(��ֻ��һ�����ţ���BΪ���һ�����ţ�
			if (hasNone(sentence[i].tail[B + 1])){//A�� ��B�£�������FIRST(��)�Ļ�
				flag = flag | Add(follow[mapfollow(sentence[i].tail[B])], follow[mapfollow(sentence[i].head)]);//FOLLOW(A)->FOLLOW(B)
			}
			if (sentence[i].tail[B + 1] != '\0') B++;//B�����һ������
			if (isuppercase(sentence[i].tail[B])){//A�� ��B
				flag = flag | Add(follow[mapfollow(sentence[i].tail[B])], follow[mapfollow(sentence[i].head)]);//FOLLOW(A)->FOLLOW(B)
			}
			i++;
		}
	}
}

void findlowercase(){
	int i = 0;//sentence��ָ��
	int j = 0;//lowercase��ָ��
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
	int i = 0;//sentence��ָ��
	int j = 0;//uppercase��ָ��
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

bool Add(char dest[], char source[]){//�������ӣ��Զ����Ԧ�
	int d = 1, s = 0;//dest[]��first����first������λ��first(a)��a�����ӵڶ�λ��ʼ��
	bool success = false;
	if (source[0] == NONE){//����Ӧ�
		while (dest[d] != '\0'){
			if (dest[d] == NONE){ success = false; break; }//����Ѿ��Ц��򲻱��ټ�
			d++;
		}
		if (dest[d] == '\0'){ dest[d] = NONE; dest[d + 1] = '\0'; success = true; }
	}
	else{//��������������ܼӦ�
		bool flag = true;
		for (s = 1, flag = true; source[s] != '\0'; s++){
			for (d = 1; dest[d] != '\0'; d++){
				if (dest[d] == source[s]){
					flag = false;//˵��dest���Ѿ��д�Ԫ�أ������ټ�
					break;
				}
			}
			if (flag == true && source[s] != NONE){ dest[d] = source[s]; dest[d + 1] = '\0'; success = true; }//˵����Ҫ����
			flag = true;
		}
	}
	return success;
}

bool hasNone(char c){//first(c)���Ƿ��Ц�
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

void inittable(){//���ñ�ͷ
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
	bool flag = false;//�ж��Ƿ������first()
	for (i = 0; tm2[i] != '\0'; i++){
		//��sentence���ҵ�ǰ��i�з��ս�����ķ�
		for (j = 0; sentence[j].head != '\0'; j++){
			if (sentence[j].head == tm2[i] && sentence[j].tail[0] != NONE){
				g = sentence[j];

				int n;
				for (n = 1; (a = first[mapfirst(g.tail[0])][n]) != '\0'; n++){//ѭ��sentnece[i].tail[0]��first��
					//a��first(D)��Ԫ��(E->DT)
					//�������follow����E->�ŵĲ���
					if (first[mapfirst(g.head)][n] == NONE){
						flag = true;
					}//˵����Ҫ����E->��,�鿴follow��
					//////////////////////////////////
					int k;
					for (k = 0; tm1[k] != '\0'; k++){//tm1[k]��table��k��λ�ı�ͷ
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
