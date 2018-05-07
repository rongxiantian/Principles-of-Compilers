#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "identify.h"
#include "LL(1).h"
char  TOKEN[20];


extern  int  lookup(char *);
extern void out(int, char*);
extern void report_error(char *);
char scanner_example(FILE *fp)
{
	char  ch;
	int  i, c;
	ch = fgetc(fp);
	if (ch == ' ' || ch == '\n' || ch == EOF){}
	else if (isalpha(ch))    /*it  must  be  a  identifer*/
	{
		TOKEN[0] = ch;
		ch = fgetc(fp);  i = 1;
		while (isalnum(ch))
		{
			TOKEN[i] = ch; i++;
			ch = fgetc(fp);
		}
		TOKEN[i] = '\0';
		fseek(fp, -1, 1);        /*retract*/
		c = lookup(TOKEN);
		if (c == 0) out(ID, TOKEN);
		else  out(c, " ");
	}
	else if (isdigit(ch))
	{
		TOKEN[0] = ch;
		ch = fgetc(fp); i = 1;

		while (isdigit(ch))
		{
			TOKEN[i] = ch; i++;
			ch = fgetc(fp);
		}
		TOKEN[i] = '\0';
		if (isalpha(ch)){
			char wrong[20];
			strcpy(wrong, TOKEN);
			wrong[i++] = ch;
			while (isalpha(ch = fgetc(fp)) && ch != EOF){
				wrong[i] = ch;
				i++;
			}
			wrong[i] = '\0';
			report_error(wrong);
		}
		else{
			fseek(fp, -1, 1);
			out(NUM, TOKEN);
		}
	}
	else
		switch (ch)
	{
		case'<': ch = fgetc(fp);
			if (ch == '=')  out(SEQ, "<=");
			else  if (ch == '>') out(NEQ, "<>");
			else if (ch == '<') out(LEFT, "<<");
			else
			{
				fseek(fp, -1, 1);
				out(SMALL, "<");
			}
			break;
		case '=':  ch = fgetc(fp);
			if (ch == '=') out(EQ, "==");
			else{
				fseek(fp, -1, 1);
				out(ASSIGN, "=");
			}
			break;
		case '>':  ch = fgetc(fp);
			if (ch == '=')out(LEQ, ">=");
			else if (ch == '>')out(RIGHT, ">>");
			else
			{
				fseek(fp, -1, 1);
				out(LARGE, ">");
			}
			break;
		case '+': ch = fgetc(fp);
			if (ch == '+')out(PLUSD, "++");
			else if (ch == '=')out(PLEQ, "+=");
			else
			{
				fseek(fp, -1, 1);
				out(PLUS, "+");
			}
			break;
		case '-': ch = fgetc(fp);
			if (ch == '-')out(SUBD, "--");
			else if (ch == '=')out(SUEQ, "-=");
			else
			{
				fseek(fp, -1, 1);
				out(SUB, "-");
			}
			break;
		case '*': ch = fgetc(fp);
			if (ch == '=')out(MUEQ, "*=");
			else
			{
				fseek(fp, -1, 1);
				out(MUL, "*");
			}
			break;
		case '/': ch = fgetc(fp);
			if (ch == '=')out(DIEQ, "/=");
			else if (ch == '*') {
				while ((ch = fgetc(fp)) != EOF){
					if (ch == '*'){
						ch = fgetc(fp);
						if (ch == '/'){ break; }
						else{ fseek(fp, -1, 1); }
					}
				}
			}
			else
			{
				fseek(fp, -1, 1);
				out(LEAN, "/");
			}
			break;
		case '!': ch = fgetc(fp);
			if (ch == '=')out(NEQ, "!=");
			else {
				fseek(fp, -1, 1); out(NOT, "!");
			}
			break;
		case '%': ch = fgetc(fp);
			if (ch == 'd')out(PD, "%d");
			else if (ch == 's')out(PS, "%s");
			else if (ch == 'c')out(PC, "%c");
			else if (ch == 'l') {
				ch = fgetc(fp);
				if (ch == 'f')out(PLF, "%lf");
				else {
					fseek(fp, -2, 1);
					out(PER, "%");
				}
			}
			else { out(PER, "%"); }
			break;
		case '\\': out(RLEAN, "\\"); break;
		case ';':out(FEN, ";"); break;
		case ',':out(DOU, ","); break;
		case '(':out(LS, "("); break;
		case ')':out(RS, ")"); break;
		case '{':out(LL, "{"); break;
		case '}':out(RL, "}"); break;
		case '"':out(YIN, "\""); break;
		default:report_error(""); break;

	}
	return ch;
}

int main()
{
	FILE *fp;
	if ((fp = fopen("test.txt", "r")) == NULL)//打开操作不成功
	{
		printf("open error");
		exit(1);//结束程序的执行
	}
	out(0, " ");
	printf("词法分析结果为\n");
	while (scanner_example(fp) != EOF){}
	fclose(fp);

	/*****来自于实验二****/
	printf("\n语法分析结果为\n");
	read();
	current = input[NextSym];
	if (E()) { printf("grammar is right\n"); }
	else { printf("grammar is wrong\n"); }
	return 0;
}
