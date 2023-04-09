#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST{
	int value;
	struct BST* left;
	struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	Tree root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				if(!findNode(root, key))
					root = insertNode(root, key);
				else
					fprintf(fout, "insertion error: %d is already in the tree\n", key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key))
					fprintf(fout, "%d is in the tree\n", key);
				else
					fprintf(fout, "finding error: %d is not in the tree\n", key);
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
					fprintf(fout, "delete %d\n", key);
				}
				else{
					fprintf(fout, "deletion error: %d is not in the tree\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL)
						fprintf(fout, "tree is empty");
					else
						printInorder(root);
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}
/*
Insert the value "key" in Tree "root"
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the new node to insert.
return:
	pointer of root
print out:
	"insert key\n"
*
* 현 root의 value보다 key가 작으면 왼쪽 자식을 기준으로 하는 insertNode를, 크면 오른쪽 자식을 기준으로 하는 insertNode를 호출한다. 이 과정을 반복하다가 root==NULL이라는 조건문에 들어간다는 것은  insert할 위치를 찾았다는 뜻이고 그 때 새로운 노드를 동적 할당하여, 받은 key를 value에 넣어주고 leaf노드일 것이기 때문에 자식들은 모두 NULL로 해준다. 이때 root노드를 반환해주어야 하므로 insertNode를 호출하는 부분에서 해당 함수의 return 값을 root->left,root->right에 넣어주어 재귀되면서 끝까지 진행하면 결국 root는 처음 파라미터로 받은 root로 돌아온다.
*/
Tree insertNode(Tree root, int key){
	if(root == NULL){
		root = (BST*)malloc(sizeof(BST));
		root -> value = key;
		root -> left = NULL;
		root -> right = NULL;
		fprintf(fout, "insert %d\n", root->value);
	}
	else if(key < root -> value){
		root->left = insertNode(root->left, key);
	}else if(key > root -> value){
		root->right = insertNode(root->right, key);
	}
	return root;
}
/*
Delete the value key in Tree root
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the new node to be deleted.
return :
	pointer of root
*
*
* 먼저 delete할 노드를 찾아야 하므로 root에서부터 시작하여 현 위치를 기준으로 key가 그 위치의 value보다 크면 오른쪽 자식을 파라미터로 담아서 deleteNode를 호출, 작으면 왼쪽 자식을 담아서 호출한다. 그 후엔 delete를 해야하는데 삭제할 떄는 해당 노드에 자식이 두 개인지 혹은 한 개나 0개 인지로 구분한다. 자식 노드가 두개라면 오른쪽 노드들 중 가장 작은 노드와 value를 바꾸고 오른쪽 자식을 루트로 하는 subtree에서 deletemNode를 실행한다. 그렇게 되면 그 서브트리에서 가장 왼쪽에 있는 노드에 우리가 받은 key가 있을 것이고 그 노드의 자식은 하나 있거나 0개가 있게 될것이고 그것을 자식이 0개 혹은 1개 있을 떄 삭제하는 방법으로 delete한다. 그래서 결국 0개 혹은 1개 있을 때 삭제하는 방법으로 free를 진행하는데 그 방법은 먼저 현재 root가 가리키고 있는 위치를 tmp에 담는다. 그리고 root에서 왼쪽 자식이 없으면 오른쪽 자식으로 이동하고 오른쪽 자식이 없으면 왼쪽으로 이동하게 한다. 그리고는 tmp를 free하여 삭제하고자하는 노드를 delete하고 root를 반환한다. 이 함수는 재귀의 형태로 진행되기 때문에 DeleteNode를 호출할때마다 반환 값을 각 시점에서 이동하고자 하는 자식에 담아 결국 끝까지 진행되었을 때 BST의 루트 포인터가 반환되게 한다.
*/
Tree deleteNode(Tree root, int key){
	Tree tmp;
	if(key < root->value){
		root->left = deleteNode(root->left, key);
	}else if( key > root->value){
		root->right = deleteNode(root->right, key);
	}else if( root->left && root->right){//key == value
		tmp = root->right;
		while(tmp->left != NULL){
			tmp = tmp->left;
		}//오른쪽에서 제일 작은 노드 찾음
		root->value = tmp->value;
		root->right = deleteNode(root->right, key);//root는 deleteNode를 통해서만 움직임
	}else{
		tmp = root;
		if(root->left == NULL){
			root = root->right;
		}else if(root->right == NULL){
			root = root->left;
		}
		free(tmp);
	}
	return root;	
}
/*
Find the value key in Tree root
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the node trying to find.
return :
	1, key is found
	0, key is not found
*
* 현 위치를 가리키는 포인터인 tmp를 먼저 root로 초기화하여 root부터 찾게한다. 현 위치의 value 값보다 key값이 크면 오른쪽 자식을 담은 findNode를, 작으면 왼쪽 자식을 담은 findNode를 반환한다. 그렇게 해서 찾게 된다면 1을 반환하도록 하고 찾지 못한다면 0을 반환하게 한다.
*/
int findNode(Tree root, int key){
	Tree tmp = root;
	if(tmp == NULL){
		return 0;
	}
	else if(key > tmp -> value){
		return findNode(tmp->right, key);
	}
	else if(key < tmp -> value){
		return findNode(tmp->left, key);
	}
	else{
		return 1;
	}
}
/*
In Order Traversal
Tree root : A pointer to the root node of the binary search tree.
print out:
	"root->value "
*
* inorder방식으로 출력한다는 것은 각 단계마다 왼쪽 자식/자신/오른쪽 자식 순서로 print하겠다는 의미이고 이를 이용해서 재귀함수를 설계한다. root가 NULL이 아니라면 왼쪽 자식이 안나올 때까지 계속 왼쪽으로 이동하면서 printInorder을 호출한다. 더이상 없다면 그 전에 호출되었던 곳으로 돌아가 현 위치의 value를 출력하고(자신 출력) 오른쪽 자식으로 이동해서 printInorder을 호출한다. 있다면 거기서 이 왼쪽 자식/ 자신/ 오른쪽 자식 구조를 반복하며 print할 것이다. 이 방식으로 트리 안의 모든 노드를 다 중위순회하며 print할 수 있다. 
*/
void printInorder(Tree root){
	if(root != NULL){
		printInorder(root->left);
	      	fprintf(fout, "%d ", root->value);
		printInorder(root->right);
	}
}
/*
Delete Tree
free allocated memory
Tree root : A pointer to the root node of the binary search tree.
*
* 각 노드들을 이동하며 한번 씩만 삭제해야 하는데 이 구조는 위의 printInorder방식과 유사하다. print대신 free를 하여 각 노드들을 모두 삭제한다. inorder방식으로 삭제한다는 것은 각 단계마다 왼쪽 자식/자신/오른쪽 자식 순서로 삭제하겠다는 의미이다. root가 NULL이 아니라면 왼쪽 자식이 안나올 때까지 계속 왼쪽으로 이동하면서 deleteTree를 호출한다. 더이상 없다면 그 전에 호출되었던 곳으로 돌아가  현 위치의 value를 삭제하고(자신 삭제) 오른쪽 자식으로 이동해서 deleteTree를 호출한다. 있다면 거기서 왼쪽 자식/ 자신/ 오른쪽 자식 구조를 반복하며 삭제할 것이다. 이 방식으로 트리 안의 모든 노드를 다 중위순회하며 delete 한다.
*/
void deleteTree(Tree root){
	if(root != NULL){
		deleteTree(root->left);
		free(root);
		deleteTree(root->right);
	}
}

