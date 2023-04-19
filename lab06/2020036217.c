#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode{
	ElementType element;
	AVLTree left, right;
	int height;
}AVLNode;

AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
void PrintPreorder(AVLTree T);
void DeleteTree(AVLTree T);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);

/*
 * 트리에서 해당 노드의 height값을 반환해준다
 * */
int Height(AVLTree T){
	if(T == NULL) return -1;
	return T->height;
}
// 두 값중 큰 값을 반환해준다. 나중에 자신의 height가 자식의 height들 중 큰 값보다 1커야된다는 logic에 사용된다
int Max(int a, int b){
	return a > b ? a : b;
}

/*
Insert the value X in AVLTree T
return:
	pointer of root
print out:
	"insertion error : X is already in the tree!\n" , X is already in T
*
* 먼저 일반 트리에서처럼 insert 되어야 하는 곳을 찾아야 하므로 현재 위치의 element보다 작으면 왼쪽, 크면 오른쪽으로 재귀함수를 이용하여 보낸다. 현재 위치가 NULL이라는 것은 삽입될 위치를 찾았다는 얘기고 그곳에 새로운 메모리를 할당해주며 element에는 X를 당장은 left, right자식이 없으니 NULL을 height또한 0으로 할당한다. 일단 insert까지 되었고 재귀함수로 반복하며 return되며 root노드까지 올라가는데 도중 왼쪽과 오른쪽 자식의 height값이 2보다 커서 unbalance해지는 곳을 찾는다. 찾았을 떄 현재 위치와 insert된 곳의 관계가 left-left/right인지 right-right/left인지 구분하여 각 상황에 맞는 rotation을 실행하면 된다.
*/
AVLTree Insert(ElementType X, AVLTree T){
	if( T == NULL){
			T = (AVLNode*)malloc(sizeof(AVLNode));
			T->element = X;
			T->left = NULL;
			T->right = NULL;
			T->height = 0;
	}
	else if( X < T->element){
		T->left = Insert(X, T->left);
		if(Height(T->left) - Height(T->right) >= 2){
			if( X < T->left->element){
				T = SingleRotateWithLeft(T);
			}else T = DoubleRotateWithLeft(T);
		}
	}
	else if(X > T->element){
		T->right = Insert(X, T->right);
		if(Height(T->right) - Height(T->left) >= 2){
			if( X > T->right->element){
				T = SingleRotateWithRight(T);
			}else T = DoubleRotateWithRight(T);
		}
	}else{//이미 존재하는 경우
		fprintf(fout, "insertion error : %d is already in the tree!\n", X);
	}
	T->height = Max(Height(T->left), Height(T->right)) + 1;
	return T;
}

/*
Delete the value X in AVLTree T
return:
	pointer of root
print out:
	"deletion error : X is not in the tree!\n", X is not in T
*X에 맞는 노드를 먼저 찾아야 하기 때문에 insert의 로직과 유사하게 X를 찾아간다. 만약 해당 노드가 NULL이라면 트리 안에 없다는 것이고 deletion error을 출력하게 한다. 찾았다면 삭제를 해야하는데 노드가 0개이면 그냥 삭제하고 1개이면 자식 노드를 삭제한 노드가 있는 위치에 두어야 한다. 2개를 삭제할 때에는 오른쪽 서브트리에서 가장 작은 값을 찾아 삭제하고자하는 노드의 element만 바꾸고 다시 오른쪽 서브트리부터 Delete를 재귀적으로 호출한다. 그렇게 해서 바뀌어진 노드가 있던 위치를 삭제한다. 삭제를 끝낸 후 return 되면서 root까지 올라가는데 도중 height의 차이가 2이상인 곳을 찾고 찾은 곳에서 왼쪽 자식과 오른쪽 자시의 height를 비교해 큰 쪽을 기준으로 해서 left-left/right, right-right/left를 판별한다. 각 위치관계에 맞는 rotation을 호출해서 밸런스를 맞춰주고 다시 root까지 올라간다.
*/
AVLTree Delete(ElementType X, AVLTree T){
	if(T == NULL){//못 찾은 경우
		fprintf(fout, "deletion error : %d is not in the tree!\n", X);
	}else if( X < T->element){
		T->left = Delete(X, T->left);
	}else if( X > T->element){
		T->right = Delete(X, T->right);
	}
	else if(T->left == NULL || T->right == NULL){
		AVLTree tmp = T;
		if(T->left == NULL){
			T = T->right;
		}else{
			T = T->left;
		}
		free(tmp);
	}else{
		AVLTree tmp = T->right;
		while(tmp->left != NULL){
			tmp = tmp->left;
		}
		T->element = tmp->element;
		tmp->element = X;
		T->right = Delete(X, T->right);
	}
	if(T){
		T->height = Max(Height(T->left), Height(T->right)) + 1;//높이 재조정
		if(Height(T->left)-Height(T->right) >= 2){
			AVLTree tmp = T->left;
			if(Height(tmp->left) >= Height(tmp->right)){
				T = SingleRotateWithLeft(T);
			}else T = DoubleRotateWithLeft(T);
		}
		else if(Height(T->right)-Height(T->left) >= 2){
			AVLTree tmp = T->right;
			if(Height(tmp->right) >= Height(tmp->left)){
				T = SingleRotateWithRight(T);
			}else T = DoubleRotateWithRight(T);
		}
	}
	return T;
}
/*
Pre order Traversal
*
* Preorder방식으로 출력하는 것은 자신 왼쪽 자식 오른쪽 자식 순으로 출력하는 것이므로 T가 null이 아닐때 일단 출력하여 자신을 print해주고 왼쪽 자식을 가지고 PrintPreorder을 재귀적으로 호출한다. 왼쪽 자식을 봤으면 오른쪽 자식도 볼 수 있도록 밑에 오른쪽 자식으로 함수를 호출하게 한다.
*/
void PrintPreorder(AVLTree T){
	if(T != NULL ){
		fprintf(fout, "%d(%d) ", T->element, Height(T));
		PrintPreorder(T->left);
		PrintPreorder(T->right);
	}
}
/*
Delete Tree
free allocated memory
*
* 모든 노드를 중복 없이 한번만 반복해야 하므로 왼쪽 오른쪽 자식을 먼저 삭제하고 자신을 삭제하는 식으로 모든 노드의 메모리를 해제한다.
*/
void DeleteTree(AVLTree T){
	if(T != NULL){	
		DeleteTree(T->left);
		DeleteTree(T->right);
		free(T);
	}
}
/*
Rotation functions for AVLTree
*
*left-left상황일때의 rotation이므로 기준 노드의 왼쪽 자식을 tmp에 저장하고 그 tmp의 오른쪽 자식을 기준 노드의 왼쪽 자식으로 할당해준다. 다시 tmp의 오른쪽 노드는 기준 노드를 가리키도록 하여 돌렸을 떄 겹치는 부분을 tree 규칙에 맞게 배치한다. 위치를 조정해줬으므로 내가 저장한 높이값들이 안 맞을 것이므로 다시 height들을 조정한다.
*/
Position SingleRotateWithLeft(Position node){
	Position tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;

	node->height = Max(Height(node->left), Height(node->right)) + 1;
	tmp->height = Max(Height(tmp->left), Height(tmp->right)) + 1;
	return tmp;
}
/*
 * right-right상황일때의 rotation이므로 기준 노드의 오른쪽 자식을 tmp에 저장하고 그 tmp의 왼쪽쪽 자식을 기준 노드의 오른쪽 자식으로 할당해준다. 다시 tmp의 왼쪽 노드는 기준 노드를 가리키도록 하여, 돌렸을 떄 겹치는 부분을 tree 규칙에 맞도록 배치한다. 위치를 조정해줬으므로 내가 저장한 높이 값들이 안 맞을 것이므로 다시 height들을 조정한다.
 */
Position SingleRotateWithRight(Position node){
	Position tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;
	node->height = Max(Height(node->left), Height(node->right)) + 1;
        tmp->height = Max(Height(tmp->left), Height(tmp->right)) + 1;
	return tmp;
}
/*
 *left-right상황일때의 rotation이므로 기준 노드의 왼쪽 자식을 그 노드의 왼쪽자식을 기준으로 왼쪽으로 돌려야 하므로 right-right상황의 rotation을 적용한다. 그리고 다시 기준노드를 기준으로 오른쪽으로 돌려야 하므로 left-left상황일 때 사용했던 rotation을 적용한다. 
 */
Position DoubleRotateWithLeft(Position node){
	node->left = SingleRotateWithRight(node->left);
	return SingleRotateWithLeft(node);
}
/*
 *right-left상황일때의 rotation이므로 기준 노드의 오른쪽 자식을 다시 기준 노드의 오른쪽자식을 기준으로, 오른쪽으로 돌려야 하므로 left-left상황의 rotation을 적용한다. 그리고 다시 기준노드를 기준으로 왼쪽으로 돌려야 하므로 right-right상황일 때 사용했던 rotation을 적용한다.
 */
Position DoubleRotateWithRight(Position node){
	node->right = SingleRotateWithLeft(node->right);
	return SingleRotateWithRight(node);
}

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLTree Tree = NULL;
	char cv;
	int key;

	int i=0;

	while(!feof(fin)){
		printf("%d: ", ++i);
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintPreorder(Tree);
		fprintf(fout, "\n");
		printf("\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);
	return 0;
}



