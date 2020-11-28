#pragma once
#include<stdio.h>

int stack[100];
int top = 0;

void clear()
{
	for (int i = 0; i < 100; i++)
		stack[i] = 0;

}

void pop()
{
	if (top > 0)
		top--;
}

void push(int val)
{
	if (top < 100)
	{
		stack[top] = val;
		top++;
		
	}
}

int tos()
{
	return stack[top-1];
}