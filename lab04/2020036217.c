
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
*
* root 노드를 만들어서 자식은 없을 것이므로 오른쪽, 왼쪽 자식은 모두 자기 자신을 가리키도록 왼쪽, 오른쪽 스레드도 모두 1로 만든다. root의 data는 -1로 한다.
*/
ThreadedPtr CreateTree(){
	ThreadedPtr tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
	tree->left_thread = 1;
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

*
*새로운 노드를 만들고 그 노드에 parameter로 받은 data를 넣어준다. 새로운 노드가 들어갈 위치에  가지(edge)가 몇개인지 세기 위해 tmpIdx를 2로 반복해서 나누어 계산한다. 삽입할 노드의 위치를 찾기 위해서 root와 root의 index를 담고 있는 변수를 생성한다. 삽입할 노드의 부모 노드를 찾기 위해서 현재 위치와 idx사이의 가지 개수로 dividor를 만들어 idx를 나누면 그 몫이 바로 현재 위치에서 부모 노드를 찾아가기 위해 나아가야 할 방향을 제공한다. 그렇게 부모 노드로 찾아가서 idx를 2로 나누어 나머지가 0이면 왼족 자식으로 1이면 오른쪽 자식으로 저장된다. 이 떄 마지막에 insert된 노드는 leaf노드 이므로 왼쪽 자식으로 저장되었으면, 자신의 왼쪽 쓰레드를 부모의 원래 왼쪽 쓰레드로 저장하고 부모는 자식이 생겼으니 0으로 바꾼다. 자신의 오른쪽 쓰레드는 부모 노드를 가르키게 하고 오른쪽 자식으로 연결되었으면 왼쪽 자식이었을 때와 반대로 수행한다. 제대로 삽입되었다면 1을 반환하고 안되었다면 0을 반환한다. */
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
		for(int i=0; i < edges-1; i++){
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
	}else if(idx % 2 == 1){
		newNode->right_child = currentPtr->right_child;
		newNode->right_thread = currentPtr->right_thread;
		newNode->left_child = currentPtr;
		newNode->left_thread = 1;
		currentPtr->right_child = newNode;
		currentPtr->right_thread = 0;
		return 1;
	}else{
		return 0;
	}
}

/*
In Order Traversal
Implementing in a recursive form is prohibited.
When printing out, the interval between each key is one space
print out:
	"inorder traversal : data1 data2 data3" (When there are 3 data, except root node)

*
* sTmp로 root 노드의 포인터를 받아 움직이는 동안 기준의 역할을 하고, mTmp은 sTmp를 기준으로 움직인다.
* 중위 순회를 하며 mTmp가 root와 같아질때 까지 for루프문을 실행한다. 그 동안 mTmp는 sTmp(기준)의 오른쪽 자식으로 시작해서 기준이 leaf였다면, 그리고 현재 위치(mTmp)에 왼쪽 자식이 있다면 왼쪽 자식이 더이상 없을 때까지 계속 왼쪽 자식으로 이동한다. 왼쪽 자식이 없다면 멈추고 sTmp에 현재위치를 주고 mTmp의 data를 출력한다. 기준은 mTmp가 이동을 멈추면 그 위치를 기준으로 하고 mTmp는 반복문에서 그것을 기준으로 움직이다가 root와 같아지면 반복문을 멈춘다.
*/

void printInorder(ThreadedPtr root){
    ThreadedPtr mTmp;
    ThreadedPtr sTmp = root;
    fprintf(fout, "inorder traversal :");
    for(;;) {
	    mTmp = sTmp->right_child;
	    if(sTmp->right_thread == FROMPARENT){
		    while(mTmp->left_thread == FROMPARENT){
			    mTmp = mTmp->left_child;
		    }
	    }
	    sTmp = mTmp;
	    if(mTmp == root) break;

	    fprintf(fout, " %d", mTmp->data);
    }
}

/*
Delete the Tree
*중위 순회 방법으로 이동하면서 이동을 멈췄을 떄 mTmp 위치의 노드를 삭제한다. mTmp가 root와 같아질때는 트리의 모든 부분을 돌았다는 뜻이고 그 떄 반복문에서 빠져나간다.
*/
void DeleteTree(ThreadedPtr root){
	ThreadedPtr mTmp;
    ThreadedPtr sTmp = root;
    for(;;) {
	    mTmp = sTmp->right_child;
	    if(sTmp->right_thread == FROMPARENT){
		    while(mTmp->left_thread == FROMPARENT){
			    mTmp = mTmp->left_child;
		    }
	    }
	    free(sTmp);
	    sTmp = mTmp;
	    if(mTmp == root){
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



