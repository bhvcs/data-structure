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
*/
Tree insertNode(Tree root, int key){
	Tree tmp = root;
	
	if(tmp == NULL){
		tmp = (BST*)malloc(sizeof(BST));
		tmp -> value = key;
		tmp -> left = NULL;
		tmp -> right = NULL;
		fprintf(fout, "insert %d\n", tmp->value);
	}
	else if(key < tmp -> value){
		tmp->left = insertNode(tmp->left, key);
	}else if(key > tmp -> value){
		tmp->right = insertNode(tmp->right, key);
	}
	return tmp;
}
/*
Delete the value key in Tree root
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the new node to be deleted.
return :
	pointer of root
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
		}else if(root->left == NULL){
			root = root->left;
		}
		free(tmp);
	}
	//fprintf(fout, "delete %d and tmp %d\n",root->value, tmp->value);
	return root;	
}
/*
Find the value key in Tree root
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the node trying to find.
return :
	1, key is found
	0, key is not found
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
*/
void printInorder(Tree root){
	Tree tmp = root;
	if(tmp ->left!= NULL){
		printInorder(tmp->left);
	}
	if(tmp ->right!= NULL){
		fprintf(fout, "%d ", tmp->value);
		printInorder(tmp->right);
		return;
	}
	/*if(tmp->left == NULL && tmp->right == NULL){
*/      fprintf(fout, "%d ", tmp->value);
	
	return;
}
/*
Delete Tree
free allocated memory
Tree root : A pointer to the root node of the binary search tree.
*/
void deleteTree(Tree root){
	Tree tmp = root;
	if(tmp->left != NULL){
		deleteTree(tmp->left);
	}
	if(tmp->right != NULL){
		//fprintf(fout, "%d ", tmp->value);
		free(tmp);
		deleteTree(tmp->right);
		return;
	}
	//fprintf(fout, "%d ", tmp->value);
	free(tmp);
	return;	
}

