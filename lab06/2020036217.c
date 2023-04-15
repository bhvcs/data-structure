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

int Height(AVLTree T){
	if(T == NULL) return -1;
	return T->height;
}
int Max(int a, int b){
	return a > b ? a : b;
}

/*
Insert the value X in AVLTree T
return:
	pointer of root
print out:
	"insertion error : X is already in the tree!\n" , X is already in T
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
	}else{
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
*/
AVLTree Delete(ElementType X, AVLTree T){
	if(T == NULL){
		fprintf(fout, "deletion error : %d is not in the tree!\n", X);
	}else if( X < T->element){
		T->left = Delete(X, T->left);
		//여기서 rebalancing이 있으면 안되나?
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
		T->height = Max(Height(T->left), Height(T->right)) + 1;
		if(Height(T->left)-Height(T->right) >= 2){
			AVLTree tmp = T->left;
			if(Height(tmp->left) >= Height(tmp->right)){//TODO: 부등호 확인
				T = SingleRotateWithLeft(T);
			}else T = DoubleRotateWithLeft(T);
		}
		else if(Height(T->right)-Height(T->left) >= 2){
			AVLTree tmp = T->right;
			if(Height(tmp->right) >= Height(tmp->left)){//TODO: 부등호 확인
				T = SingleRotateWithRight(T);
			}else T = DoubleRotateWithRight(T);
		}
	}
	return T;
}
/*
Pre order Traversal
*/
void PrintPreorder(AVLTree T){
	if(T){
		fprintf(fout, "%d(%d) ", T->element, Height(T));
		PrintPreorder(T->left);
		PrintPreorder(T->right);
	}
}
/*
Delete Tree
free allocated memory
*/
void DeleteTree(AVLTree T){
	if(T){	
		DeleteTree(T->left);
		printf("delete: %d", T->element);
		free(T);
		DeleteTree(T->right);
	}
}
/*
Rotation functions for AVLTree
*/
Position SingleRotateWithLeft(Position node){
	Position tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;

	node->height = Max(Height(node->left), Height(node->right)) + 1;
	tmp->height = Max(Height(tmp->left), Height(tmp->right)) + 1;
	return tmp;
}
Position SingleRotateWithRight(Position node){
	Position tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;
	node->height = Max(Height(node->left), Height(node->right)) + 1;
        tmp->height = Max(Height(tmp->left), Height(tmp->right)) + 1;
	return tmp;
}

Position DoubleRotateWithLeft(Position node){
	node->left = SingleRotateWithRight(node->left);
	return SingleRotateWithLeft(node);
}
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



