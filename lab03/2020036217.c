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
*
* 새로운 Stack을 동적할당 받아 Stack포인터를 만들고 stack의 최소 사이즈는 max이므로 max를 이용해 key 배열을 동적할당해준다. S의 max_stack_size는 max로 초기화 해주고 제일 최근에 들어온 요소의 index역할을 하는 top은 현재 아무것도 들어있지 않으므로 -1로 초기화 해준다. 그리고 Stack포인터를 반환해준다.
*/
Stack* CreateStack(int max){
	Stack* S = (Stack*)malloc(sizeof(Stack));
	S -> key = (int*)malloc(sizeof(int)* max);
	S->max_stack_size = max;
	S->top = -1;
	return S;
}
/*
Delete stack
free allocated memory of stack
arguments:
	Stack* S: the pointer of the Stack 
* 동적할당으로 만들어진 S를 없애야 하므로 free()를 이용해 포인터를 통해 전달 받은 stack을 없앤다.
*/
void DeleteStack(Stack* S){
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

*Push는 Stack이 가득 차있을 때 더이상 넣을 공간이 없어 실패할 것이므로 IsFull의 반환값이 1이라면 실패할 때의 1을 반환하도록 하고 가득 차있지 않다면 stack의 key에서, 기존의 top에서 1 증가한 값을 index로 하는 곳에 X를 넣어준다. 그리고 Push가 성공했다는 0을 반환한다.
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

* Pop은 가장 최근에 들어온 요소를 꺼내는 것이므로 top을 이용해서 int형 변수 element에 가장 최근 요소를 저장하고, 그 후 top을 1만큼 감소시킨다. 그리고 그 element를 반환한다.
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
* key에서 가장 최근에 들어온 요소의 index를 뜻하는 top을 이용하여 값을 꺼내 그것을 int형 변수 element에 저장하고 반환한다.
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
*top은 0부터 시작하는 index이므로 그것이 stack의 최대 사이즈보다 1만큼 작은 것과 같다면 가득 찬 것이므로 1을, 같지 않다면 0을 반환하도록 한다.
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

* stack이 비어있다면 가장 최근에 들어온 요소의 index를 지칭하는 top이 -1인 상태일 것이다. 그러므로 stack의 top이
* -1이면 1을 반환하고, 아니라면 0을 반환하도록 한다.
*/
int IsEmpty(Stack* S){
	return S->top == -1;
}
