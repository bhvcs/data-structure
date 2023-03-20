#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node{
	ElementType element;
	Position next;
};

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert(ElementType X, List L, Position P);
Position FindPrevious(ElementType X, List L);
Position Find(ElementType X, List L);
void Delete(ElementType X, List L);
void DeleteList(List L);
int* GetElements(List L);
int ListSize=0;

int main(int argc, char **argv){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");
	char x;
	int* elements;

	Position header=NULL, tmp=NULL;
	header = MakeEmpty(header);
	while(fscanf(fin,"%c",&x)!=EOF){
		if(x=='i'){
			int a,b;	fscanf(fin,"%d%d",&a,&b);
			tmp = Find(a, header);
			if(tmp!=NULL){
				fprintf(fout, "insertion(%d) failed : the key already exists in the list\n", a);
				continue;
			}
			tmp = Find(b, header);
			if(tmp==NULL){
				fprintf(fout, "insertion(%d) failed : can not find the location to be inserted\n", a);
				continue;
			}
			Insert(a, header, tmp);
		}
	
		else if(x=='d'){
			int a;	fscanf(fin,"%d",&a);
			tmp = Find(a, header);
			if(tmp==NULL){
				fprintf(fout, "deletion(%d) failed : element %d is not in the list\n", a, a);
				continue;
			}
			Delete(a, header);
		}
		else if(x=='f'){
			int a;	fscanf(fin,"%d",&a);
			tmp = FindPrevious(a, header);
			if(IsLast(tmp, header)) fprintf(fout, "finding(%d) failed : element %d is not in the list\n",a,a);
			else{
				if(tmp->element>0) fprintf(fout, "key of the previous node of %d is %d\n", a, tmp->element);
				else fprintf(fout, "key of the previous node of %d is head\n",a);
			}
		}
		else if(x=='p') {
			if(IsEmpty(header)){
				fprintf(fout, "empty list!\n");
			}
			else
			{
				elements = GetElements(header);
				for(int i = 0; i < ListSize; i++){
						fprintf(fout, "key:%d ", elements[i]);
					}
					fprintf(fout, "\n");
				free(elements);
			}
		}
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);

	return 0;
}

List MakeEmpty(List L){
	L = (List)malloc(sizeof(struct Node));
	L -> element = -1;
	L -> next = NULL;
	return L;
/*
 * malloc을 사용해 새로운 list를 할당받고
 * 그 리스트는 헤더가 될 것이니 element가 -1
 * , 아직 또 다른 요소는 없으니 next는 NULL을 가리키도록 한 후 그 리스트의 포인터를 반환*/
/*
Make new header
element should be -1
return:
	the pointer of new list
*/
}

int IsEmpty(List L){
	return L->next == NULL;
/*
 * 비어있다면 헤더의 next가 NULL이어야 한다.
 * 그래서 헤더의 next가 NULL이면 1을, 아니라면 0을 반환하도록 한다.*/
/*
Check if list L is empty or not
return:
	1, list is full
	0, list is not full
*/
}

int IsLast(Position P, List L){
	return P->next == NULL;
/*
 * P에서의 위치가 마지막이라면 P의 next는 아무것도 없는 NULL이어야 한다.
 * 따라서 P의 next가 NULL이면 1을, 아니라면 0을 반환하도록 한다.*/
/*
Check if Position P is last or not
return:
	1, list is full
	0, list is not full
*/
}
void Insert(ElementType X, List L, Position P){
	List list = (List)malloc(sizeof(struct Node));
	list -> element = X;
	list->next = P->next;
	P->next = list;
/*
 * 새로운 list를 만들어서 그 리스트에 전달 받은 X를 넣고 그 새로운 리스트는 전달 받은 위치 P
 * 다음의 리스트가 될 것이므로 기존의 P의 next위치를 list의 next가 받게 되고, P 다음 위치에 배치 되었으므로
 * P의 next를 list가 받는다.*/
/*
Insert X in position P of list L
*/
}

int* GetElements(List L){
	Position p = L;
	ListSize = 0;
	while( p->next != NULL ){
		p = p->next;
		ListSize++;
	}
	if(ListSize==0){
		return NULL;
	}
	else{
		int i = 0;
		p = L->next;
		int* array = (int*)malloc(ListSize * sizeof(int));
		while( p != NULL ){
			array[i] = p->element;
			p = p->next;
			i++;
		}
		return array;
	}
/*
 * 시작지점(여기선 header)을 받고 while문 안에서 next로 넘어가며 ListSize를 1씩 증가시키면서 총 몇개의 노드가 있는지 확인한다. 이때 ListSize가 0이면 NULL을 반환하고 아니라면, ListSize만한 크기의 int형 배열을 동적 할당, while문으로 리스트의 각 element를 배열에 저장한다. 그리고 그 배열의 포인터를 반환한다.
Return an array of copied elements from the List L
The length of the returned array should be the same as the size of the ListSize
The order of the returned array should be the same as the order of L->element  
If List is empty, return NULL
***WARNING***
You must copy the elements to another allocated array
*/
}

Position FindPrevious(ElementType X, List L){
	Position p = L;
	while( p->next != NULL && p->next->element != X){
		p = p->next;
	}
	return p;
/*
 * L의 위치를 p에 저장하고 while문 안에서 next로 다음 리스트의 element가 X와 같아질 때까지 이동한다. 이때
 * 다음 위치가 마지막 노드라면 while문을 빠져나올 수 있도록 한다. 찾았다면 해당 노드의 직전 노드의 위치가
 * p에 있을 것이고 그 p를 반환한다.
Find the previous node of the node with the given element X in List L
return:
	the pointer of previous node of node that has element X  
*/
}

Position Find(ElementType X, List L){
	Position p = L;
	while(p != NULL && p -> element != X){
		p = p -> next;
	}
	return p;
/*
 * L을 p에 저장하고 while문 안에서 리스트의 element가 X가 될 때까지 next로 이동한다. 찾게 되면 p가 해당 위치일 것이고 그 p를 반환한다. 이때 X를 못 찾았다면 p는 NULL이 되어 반환된다.
Find the node with the given element X in List L
return:
	the pointer of node has element X
*/
}

void Delete(ElementType X, List L){
	Position q;
	Position p = FindPrevious(X, L);
	if(!IsLast(p, L)){
		q = p->next;
		p->next = p->next->next;
		free(q);
	}
/*
 * linkedList에서 X를 element로 갖는 리스트의 직전 리스트의 위치를 p에 준다. 이때 X를 못 찾는다면 마지막 리스트의 위치가 반환될 것이므로 마지막 위치의 리스트가 아닐 때 기능을 실행하도록 if문을 설계한다. 삭제되어야 하는 노드는 p->next이고 중간에 이 노드가 없어지면 p의 next가 기존의 p->next->next를 가리켜야 하기 때문에 p->next를 q에 저장해 놓고 앞서 말한대로 삭제하고자 하는 노드의 전, 후 노드를 연결 시켜준다. 그리고 q를 없앤다.
Delete the node that has element X in List L
*/
}

void DeleteList(List L){
	Position p = L;
	Position q;
	while(p->next != NULL){
		q = p->next;
		free(p);
		p = q;
	}
/*
 * 시작 지점(L)을 p에 저장하고 while문 안에서 next로 이동하면서 각 리스트 없앤다. 이때 다음 리스트로 넘어가기 
 * 위해 삭제하기 전에 새로운 Position q에 p 다음 위치를 저장한 후, 삭제한 다음 다시 p에 q를 저장한다.
Delete the List
*/
}
