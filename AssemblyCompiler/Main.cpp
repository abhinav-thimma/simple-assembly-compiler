#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "InterCodeGenerator.h"



char* memory;
int registers[8];

int retrieveMem()
{
	return 0;
}


int computeRelation(int a,int b,int op)
{
	switch (op)
	{
		case 7: return a == b;
		case 8: return a < b;
		case 9: return a > b;
		case 10: return a <= b;
		case 11: return a >= b;
		default: return 0;
	}
}


void evaluateInterCode()
{
	int  i = 0;
	int boolCheck = 1;

	//using stack to store the result of if expression
	clear();

	while (i < instCount)
	{
		
		int inst_no = interCode[i].opcode;

		if (inst_no == 1)
		{
			
			int order = interCode[i].arg[2];

			
			
			if (order == 0)
			{
				int mem = interCode[i].arg[0];
				int reg = interCode[i].arg[1];

				int offset = symbolTable[mem].offset;
				int size = symbolTable[mem].size;

				memcpy((int*)memory + (offset*sizeof(int)), &registers[reg], sizeof(int));
				printf("\nvalue stored in memory: %d\n", *((int*)memory + (offset*sizeof(int))));
			}
			else
			{
				int mem = interCode[i].arg[1];
				int reg = interCode[i].arg[0];

				int offset = symbolTable[mem].offset;
				int size = symbolTable[mem].size;

				registers[reg] = *((int*)memory + offset*sizeof(int));
				//memcpy(&registers[reg],(int*)((int*)memory + (offset*sizeof(int))), sizeof(int));
				printf("value stored in register: %d\n", registers[reg]);
			}
		}
		else if (inst_no == 2)
		{
			int in1 = interCode[i].arg[1];
			int in2 = interCode[i].arg[2];

			int out = interCode[i].arg[0];

			registers[out] = registers[in1] + registers[in2];
		}


		else if (inst_no == 3)
		{
			int in1 = interCode[i].arg[1];
			int in2 = interCode[i].arg[2];

			int out = interCode[i].arg[0];

			registers[out] = registers[in1] - registers[in2];
		}


		else if (inst_no == 4)
		{
			int in1 = interCode[i].arg[1];
			int in2 = interCode[i].arg[2];

			int out = interCode[i].arg[0];

			registers[out] = registers[in1] * registers[in2];
		}

		else if (inst_no == 5)
		{
			int dest = interCode[i].arg[0];
			i = dest;
		}
		
		else if (inst_no == 12)
		{
			int reg = interCode[i].arg[0];

			printf("\noutput: %d\n", registers[reg]);
		}
		else if (inst_no == 13)
		{
			int reg = interCode[i].arg[0];
			printf("\nEnter a number:");
			scanf("%d", &registers[reg]);

		}

		else if (inst_no == 6)
		{
			//if
			int val1 = registers[interCode[i].arg[0]];
			int val2 = registers[interCode[i].arg[1]];

			int oper = interCode[i].arg[2];
			int elseNo = interCode[i].arg[3];

			int res = computeRelation(val1, val2, oper);

			if (res <= 0)
			{
				i = elseNo;
				boolCheck = 0;
				push(0);
				continue;
			}
			else
			{
				push(1);
			}


		}
		else if (inst_no == 16)
		{
			int topBool = tos();
			
			if (topBool == 1)
			{
				i = interCode[i].arg[0];

				boolCheck = 1;
			}

				pop();


		}

		i++;
	}

	getchar();
}



int  main()
{
	//creating the tables
	symbolTable = (struct symbolTableRow*)malloc(sizeof(struct symbolTableRow) * 113);
	interCode = (struct interCodeRow*)malloc(sizeof(struct interCodeRow) * 1000);
	labelTable = (struct labelTableRow*)malloc(sizeof(struct labelTableRow) * 100);


	//allocating a memory heap for entire program
	memory = (char*)malloc(4096);

	generateInterCode("input3.txt");

	printf("\n--------------label table-------------\n");
	printLabelTable();

	printf("\n--------------inter code-------------\n");
	printInterCode();
	//storeInterCode("interCode.txt");


	//interCode = retrieveInterCode("interCode.txt");
	evaluateInterCode();
	printf("\n");
	//printInterCode();
	//printf("\n---------symbol table --------------\n");

	//printf("offset = %d\n", offset);
	//	printf("%s %d %d\n", symbolTable[0].name, symbolTable[0].offset, symbolTable[0].size);
	//	printf("%s %d %d\n", symbolTable[1].name, symbolTable[1].offset, symbolTable[1].size);
	//	
	getchar();

}