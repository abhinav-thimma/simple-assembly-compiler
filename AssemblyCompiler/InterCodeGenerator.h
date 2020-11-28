#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "InsturctionStack.h"


//symbol table entry
struct symbolTableRow
{
	char name[100];
	int offset;
	int size;
	int constValue;
};

//label tabel entry
struct labelTableRow
{
	char name[100];
	int inst_no;
};

//intermediate code entry
struct interCodeRow
{
	int inst_no;
	int opcode;
	int arg[4];
};

//-------------------0---1-------2------3-----4--------5------6-----7-----8----9-----10------11------12-------13------14-----15--------16------17------18
char opcode[][7] = { "", "MOV", "ADD", "SUB", "MUL", "JMP", "IF", "EQ", "LT", "GT", "LTEQ", "GTEQ", "PRINT", "READ", "DATA", "CONST", "ELSE", "THEN", "ENDIF" };

struct symbolTableRow * symbolTable;		int symCount = 0;
struct labelTableRow * labelTable;			int labelCount = 0;
struct interCodeRow* interCode;				int instCount = 0;



//this function returns token of string based on delimiter
char** getTokens(char* str, char* delim)
{
	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0';

	char** arr = (char**)malloc(sizeof(char*) * 10);
	char* s;
	int i = 1;


	arr[0] = strtok(str, delim);

	while (s = strtok(NULL, delim))
	{
		arr[i] = s;
		i++;
	}

	return arr;
}



//hash function
unsigned int hash(char *str)
{
	/*unsigned int hash = 5381;
	int c;

	while (c = *str++)
	hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	int hash = str[0] - 'A';
	return hash;
}


int getOpCode(char* code)
{
	for (int i = 0; i < 19; i++)
		if (strcmp(code, opcode[i]) == 0)
			return i;
	return -1;
}

//this function returns the inst_no for a matching label in label table
int getInst_No(char* in)
{
	for (int i = 0; i < labelCount; i++)
	{
		if (strcmp(in, labelTable[i].name) == 0)
			return labelTable[i].inst_no;
	}

	return -1;
}

int getArgCount(int code)
{
	int argCount = 0;
	if (code == 1 || code == 6)
		argCount = 2;
	else if (code == 12 || code == 13 || code == 14 || code == 5 )
		argCount = 1;
	else if (code == 2 || code == 3 )
		argCount = 3;


	return argCount;
}


void generateInterCode(char* filename)
{
	int offset = 0;


	FILE* fp = fopen(filename, "r");


	char* buff = (char*)malloc(sizeof(char) * 100);


	//reading file data line by line
	while (fgets(buff, 100, fp))
	{

		char** tokens = getTokens(buff, " ");
		int opcode = getOpCode(tokens[0]);


		if (opcode == -1)
		{
			//this is a Label

			//--------------tokens[0][strlen(tokens[0]) - 2] = '\0';

			tokens[0][strlen(tokens[0]) - 1] = '\0';
			strcpy(labelTable[labelCount].name, tokens[0]);
			labelTable[labelCount].inst_no = instCount;

			interCode[instCount].inst_no = instCount;
			interCode[instCount].opcode = -1;
			interCode[instCount].arg[0] = -1;



			labelCount++;
			instCount++;
			continue;
		}

		if (opcode == 5)
		{
			char* label = (char*)malloc(sizeof(char) * 100);
			strcpy(label,tokens[1]);
			//--------------label[strlen(label) - 1] = '\0';
			int inst = getInst_No(label);
			interCode[instCount].inst_no = instCount;
			interCode[instCount].opcode = 5;
			interCode[instCount].arg[0] = inst;

			instCount++;

			continue;
		}

		if (opcode == 16)
		{
			//else
			int lastIf = tos();
			pop();
			push(instCount);


			interCode[lastIf].arg[3] = instCount;

			interCode[instCount].opcode = 16;
			interCode[instCount].inst_no = instCount;
			instCount++;

			continue;
		}

		if (opcode == 18)
		{
			//endif
			int lastElse = tos();
			pop();
			
			interCode[lastElse].arg[0] = instCount + 1;

			interCode[instCount].opcode = 18;
			interCode[instCount].inst_no = instCount;
			instCount++;

			continue;

		}

		if (opcode == 6)
		{
			//if
			char* op1 = tokens[1];
			char* op2 = tokens[2];
			char* oper = tokens[3];


			//removing \n from op2
			char* temp = (char*)malloc(sizeof(char) * 100);
			strcpy(temp, oper);
			//---------------temp[strlen(temp) - 1] = '\0';

			push(instCount);
			interCode[instCount].inst_no = instCount;
			interCode[instCount].opcode = 6;
			/*interCode[instCount].arg[0] = atoi(op1);
			interCode[instCount].arg[2] = atoi(temp);*/
			interCode[instCount].arg[2] = getOpCode(temp);

		}

		

		//MOV A B
		//0: B-reg A-mem
		//1: B-mem A-reg
		int orderOfMOV = 0;

		if (opcode == 1)
		{
			char* var1 = (char*)malloc(sizeof(char) * 100);
			char* var2 = (char*)malloc(sizeof(char) * 100);

			if (tokens[1][strlen(tokens[1]) - 1] == 'X')
				orderOfMOV = 1;
			else
				orderOfMOV = 0;

			interCode[instCount].arg[2] = orderOfMOV;
			
		}
		interCode[instCount].inst_no = instCount;
		interCode[instCount].opcode = opcode;

		int fillSymbolTable = 0, fillLabelTable = 0;

		if (strcmp(tokens[0], "DATA") == 0 || strcmp(tokens[0], "CONST") == 0)
			fillSymbolTable = 1;


		int argCount = getArgCount(opcode);

		for (int i = 0; i < argCount; i++)
		{
			int diff = 0;
			/*---------------if (tokens[i + 1][strlen(tokens[i + 1]) - 1] == '\n')
				diff = 1;
			else
				diff = 0;*/

			if (tokens[i + 1][strlen(tokens[i + 1]) - 1 - diff] == ']')
			{
				char** t = getTokens(tokens[i + 1], "[");

				char* num = (char*)malloc(sizeof(char) * 100);
				strcpy(num, t[1]);
				num[strlen(t[1])] = '\0';


				int hashval = hash(t[0]);

				if (fillSymbolTable == 1)
				{
					strcpy(symbolTable[hashval].name, t[0]);
					symbolTable[hashval].offset = offset;
					symbolTable[hashval].size = atoi(num);

					offset = offset + atoi(num);
				}

				interCode[instCount].arg[i] = hashval;


			}
			else if (tokens[i + 1][strlen(tokens[i + 1]) - 1 - diff] != 'X')
			{
				int hashval = hash(tokens[i + 1]) % 113;


				if (fillSymbolTable == 1)
				{

					strcpy(symbolTable[hashval].name, tokens[i + 1]);
					symbolTable[hashval].offset = offset;
					symbolTable[hashval].size = 1;

					offset = offset + 1;
				}

				interCode[instCount].arg[i] = hashval;
			}
			else
			{
				interCode[instCount].arg[i] = tokens[i + 1][0] - 'A';
			}

		}

		instCount++;
	}
}


void printInterCode()
{


	for (int i = 0; i < instCount; i++)
	{
		printf("%d %d ", interCode[i].inst_no, interCode[i].opcode);


		int opcode = interCode[i].opcode;
		int argCount = getArgCount(opcode);
		if (opcode == 6)
			argCount = 4;
		else if (opcode == 16)
			argCount = 1;
		else if (opcode == 1)
			argCount = 3;

		for (int j = 0; j < argCount; j++)
		{
			printf("%d ", interCode[i].arg[j]);
		}

		printf("\n");
	}



}


void storeInterCode(char* filename)
{

	FILE *fp = fopen(filename,"w");
	for (int i = 0; i < instCount; i++)
	{
		fprintf(fp,"%d %d ", interCode[i].inst_no, interCode[i].opcode);


		int opcode = interCode[i].opcode;
		int argCount = getArgCount(opcode);
		if (opcode == 6)
			argCount = 4;
		else if (opcode == 16)
			argCount = 1;
		else if (opcode == 1)
			argCount = 3;

		for (int j = 0; j < argCount; j++)
		{
			fprintf(fp,"%d ", interCode[i].arg[j]);
		}

		fprintf(fp,"\n");
	}

	fclose(fp);
}


struct interCodeRow* retrieveInterCode(char* filename)
{
	FILE* fp = fopen(filename,"r");
	struct interCodeRow* interCode1 = (struct interCodeRow*)malloc(sizeof(struct interCodeRow));

	int i = 0;
	
	char * buff = (char*)malloc(sizeof(char) * 100);

	while (buff = fgets(buff, 100, fp))
	{
		char** tokens = getTokens(buff," ");
		int noOfArgs = sizeof(tokens) / sizeof(char**) - 2;
		interCode1[i].inst_no = atoi(tokens[0]);
		interCode1[i].opcode = atoi(tokens[1]);

		for (int j = 0; j < noOfArgs; j++)
		{
			interCode1[i].arg[j] = atoi(tokens[2 + j]);
		}
	}

	return interCode1;

}


void printLabelTable()
{
	for (int i = 0; i < labelCount; i++)
	{
		printf("%s  %d\n", labelTable[i].name, labelTable[i].inst_no);
	}
}