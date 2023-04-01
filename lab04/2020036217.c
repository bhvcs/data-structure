
#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

#define FROMPARENT 0
#define FROMTHREAD 1

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
	int left_thread; // flag if ptr is thread, 1, if it is a thread or 0, if it points to a child node
	ThreadedPtr left_child;
	ElementType data;
	ThreadedPtr right_child;
	int right_thread; // flag if ptr is thread, 1, if it is a thread or 0, if it points to a child node
}ThreadedTree;



/*
Create a root of new Threaded Tree
Root data should be -1
return:
	the pointer of new Threaded Tree
*/
ThreadedPtr CreateTree(){
	ThreadedPtr tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
	tree->left_thread = 1;//1이 맞다고 생각중
	tree->right_thread = 1;
	tree->left_child = tree;
	tree->right_child = tree;
	tree->data = -1;

	return tree;
}


/*
Insert the key value "data" in Threaded Tree
root : Pointer to the root node of Threaded Tree
root_idx : Index of the root node of Threaded Tree, means the first node to find the insertion position
data : data key value of the node to be newly inserted,
idx : The index of the node to be newly inserted, 0 means root index
return :
	1, the insertion succeeds
	0, the insertion fails.
*/
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx){
	ThreadedPtr newNode = (ThreadedPtr)malloc(sizeof(ThreadedTree));
	if(newNode == NULL){
		return 0;
	}
	newNode->data = data;
	ThreadedPtr currentPtr = root;
	int currentIdx = root_idx;

	int edges = 0;
	int tmpIdx = idx;
	while(tmpIdx != 0){
		tmpIdx= tmpIdx/2;
		edges++;
	}//edges개수 세기 완료

	while(edges != 1){
		int dividor = 1;
		for(int i=0; i < edges-1; i++){  //for문을 while문 밖으로 보내고 dividor을 while문 안에서 2로 나누는게 좀 더 나을지도
			dividor *= 2;
		}//edges의 개수에 맞게 2의 거듭제곱을 구함
		if(idx / dividor == 2 * currentIdx){
			currentPtr = currentPtr->left_child;
			currentIdx *= 2;
		}else if(idx / dividor == 2 * currentIdx + 1){
			currentPtr = currentPtr->right_child;
			currentIdx = 2 * currentIdx + 1;
		}else{
			return 0;
		}
		edges--;
	}
	if(idx % 2 == 0){
		newNode->left_child = currentPtr->left_child;
		newNode->left_thread = currentPtr->left_thread;
		newNode->right_child = currentPtr;
		newNode->right_thread = 1;
		currentPtr->left_child = newNode;
		currentPtr->left_thread = 0;
		return 1;
		//printf("right_child:  %d", newNode->right_child->data );
		//TODO: thread 추가
	}else if(idx % 2 == 1){
		newNode->right_child = currentPtr->right_child;
		newNode->right_thread = currentPtr->right_thread;
		newNode->left_child = currentPtr;
		newNode->left_thread = 1;
		currentPtr->right_child = newNode;
		currentPtr->right_thread = 0;
		return 1;
		//TODO: thread 추가
		//printf("left_chil: %d", newNode->left_child->data);
	}else{
		return 0;
	}
 //TODO: 오류 관리해서 return 0 쓰기
}

/*
In Order Traversal
Implementing in a recursive form is prohibited.
When printing out, the interval between each key is one space
print out:
	"inorder traversal : data1 data2 data3" (When there are 3 data, except root node)

*/

void printInorder(ThreadedPtr root){
    ThreadedPtr mTmp;
    ThreadedPtr sTmp = root;
    fprintf(fout, "inorder traversal :");
    for(;;) {
	    mTmp = sTmp->right_child;
	    if(!sTmp->right_thread){
		    while(!mTmp->left_thread){
			    mTmp = mTmp->left_child;
		    }
	    }
	    sTmp = mTmp;
	    if(mTmp == root) break;

	    fprintf(fout, "%2d", mTmp->data);
    }
}

/*
Delete the Tree
*/
void DeleteTree(ThreadedPtr root){
	ThreadedPtr mTmp;
    ThreadedPtr sTmp = root;
    for(;;) {
	    mTmp = sTmp->right_child;
	    if(!sTmp->right_thread){
		    while(!mTmp->left_thread){
			    mTmp = mTmp->left_child;
		    }
	    }
	    //fprintf(fout, "free: %2d", sTmp->data);
	    free(sTmp);
	    sTmp = mTmp;
	    if(mTmp == root) {
	//	    printf("break");
		    break;
	    }
    }
}

int main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	ThreadedPtr root = CreateTree();

	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);

	int root_idx=0, idx=0;


	while(++idx <= NUM_NODES){
		ElementType data;
		fscanf(fin, "%d", &data);

		if(Insert(root, root_idx, data, idx) == 0){
			fprintf(fout, "Insertion failed!\n");
			return 0;
		}
	}
	printInorder(root);
	DeleteTree(root);
	
	
	fclose(fin);
	fclose(fout);

	return 0;
}



