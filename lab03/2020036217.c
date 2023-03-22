#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key;
	int top;
	int max_stack_size;
}Stack;

Stack* CreateStack(int max);
int Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);

void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

void main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max=20, i=0,a,b,result, error_flag=0;
	int ret;
	fgets(input_str,101,fin);
	stack = CreateStack(max);
	
	fprintf(fout, "top numbers : ");
	while(input_str[i]!='#'){
		if('1'<=input_str[i] && input_str[i]<='9'){
			ret = Push(stack,input_str[i]-'0');
			if(ret){
				error_flag = ISFULL;
				break;
			}
		}
		else{
			if(IsEmpty(stack)){
				error_flag = ISEMPTY;
				break;
			}
			int b = Pop(stack);
			if(IsEmpty(stack)){
				error_flag = ISEMPTY;
				break;
			}
			int a = Pop(stack);
			
			if(IsFull(stack)){
				error_flag = ISFULL;
				break;
			}
			switch (input_str[i]){
				case '+' :
					ret = Push(stack, a+b);
					if(ret){
						error_flag = ISFULL;
					}
					break;
				case '-' :
					ret = Push(stack, a-b);
					if(ret){
						error_flag = ISFULL;
					}
					break;
				case '*' :
					ret = Push(stack, a*b);
					if(ret){
						error_flag = ISFULL;
					}
					break;
				case '/' : 
					if(b==0){
						error_flag = DIVIDEZERO;
						break;
					}
					ret = Push(stack, a/b);
					if(ret){
						error_flag = ISFULL;
					} 
					break;
				case '%' : 
					if(b==0){
						error_flag = DIVIDEZERO;
						break;
					}
					ret = Push(stack, a%b);
					if(ret){
						error_flag = ISFULL;
					} 
					break;
				default : break;
			}
		}

		if(error_flag) break;

		int t = Top(stack);
		fprintf(fout, "%d ",t);
		result=t;
		i++;
	}

	if(error_flag == ISFULL){
		fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
	}
	else if(error_flag == ISEMPTY){
		fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
	}
	else if(error_flag == DIVIDEZERO){
		fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
	}

	else{
		if(stack->top+1 > 1){
			fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top+1);
		}
		else{
			fprintf(fout, "\nevaluation result : %d\n",result);
		}
	}
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}


/*
Create new stack with given max size
arguments:
	int max: maximum size of stack
return:
	Stack*: the pointer of new stack  
*/
Stack* CreateStack(int max){
	Stack* S = (Stack*)malloc(sizeof(Stack));
	S -> key = (int*)malloc(sizeof(int)* max);
	S->max_stack_size = max;
	S->top = -1;
}
/*
Delete stack
free allocated memory of stack
arguments:
	Stack* S: the pointer of the Stack 
*/
void DeleteStack(Stack* S){
	//key도 free해줘야하나?
	free(S);
}

/*
Push the value into Stack
arguments:
	Stack* S: the pointer of the Stack  
	int X: positive integer
return:
	0, success
	1, stack is full
*/
int Push(Stack* S, int X){
	if(IsFull(S)){
		return 1;
	}else{
		S->key[++(S->top)] = X;
		return 0;
	}
}

/*
Pop the top value from Stack and return the top value
arguments:
	Stack* S: the pointer of the Stack
return:
	int: top value
*/
int Pop(Stack* S){
	int element = S->key[(S->top)--];
	return element;
}
/*
Get the top value from Stack
arguments:
	Stack* S: the pointer of the Stack 
return:
	top value
*/
int Top(Stack* S){
	int element = S->key[S->top];
	return element;
}
/*
Check stack is full
arguments:
	Stack* S: the pointer of the Stack
return:
	1, stack is full
	0, stack is not full
*/
int IsFull(Stack* S){
	return S->top == S->max_stack_size-1;
}
/*
Check stack is empty
arguments:
	Stack* S: the pointer of the Stack
return:
	1, stack is empty
	0, stack is not empty
*/
int IsEmpty(Stack* S){
	return S->top == -1;
}
