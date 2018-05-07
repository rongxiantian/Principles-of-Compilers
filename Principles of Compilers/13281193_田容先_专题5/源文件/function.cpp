#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "identify.h"


int  lookup (char *word)
{
    if(!strcmp(word,"void")){
        return VOID;
    }
    else if(!strcmp(word,"int")){
        return INT;
    }
    else if(!strcmp(word,"float")){
        return FLOAT;
    }
    else if (!strcmp(word,"double")){
        return DOUBLE;
    }
    else if(!strcmp(word,"if")){
        return IF;
    }
    else if(!strcmp(word,"else")){
        return ELSE;
    }
    else if(!strcmp(word,"for")){
        return FOR;
    }
    else if(!strcmp(word,"do")){
        return DO;
    }
    else if(!strcmp(word,"while")){
        return WHILE;
    }
    else{
        return 0;
    }
}

void out(int c, char* word){

    if(c==1) word="void";
    else if(c==2) word="int";
    else if(c==3) word="float";
    else if(c==4) word="double";
    else if(c==5) word="if";
    else if(c==6) word="else";
    else if(c==7) word="for";
    else if(c==8) word="do";
    else if(c==9) word="while";
	printf("(%d, %s)\n",c,word);

    FILE *fp2;
    if((fp2=fopen("out.txt","ab")) == NULL)//打开操作不成功
    {
        printf("open error");
        exit(1);//结束程序的执行
    }
    if(c<10&&c>0){
        char num[3]={'0',c+48,'\0'};
        char* num1=num;
        fputs(num1,fp2);
    }
    else if(c!=0){
    char ch[20];
    _itoa(c,ch,10);
    fputs(ch,fp2);
    }
    fputs(" ",fp2);
	if (c == 10 || c == 11){
		fputs(word, fp2);
		fputs(" ", fp2);
	}
    fclose(fp2);
}

void report_error(char *wrong){
    printf("can not find the word %s\n",wrong);
}
