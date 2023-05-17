#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode{
    int order;
    int size;           /* number of children */
    BNodePtr *child;    /* children pointers */
    int *key;           /* keys */
    int is_leaf;
}BNode;

BNodePtr CreateTree(int order);

void Insert(BNodePtr *root, int key);

int Find(BNodePtr root, int key);

void PrintTree(BNodePtr root);

void DeleteTree(BNodePtr root);

int keysLength(BNodePtr root);

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);

    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                else
                    Insert(&root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
                break;
            case 'p':
                if (root->size == 1)
                    fprintf(fout, "print error : tree is empty!");
                else
                    PrintTree(root);
                fprintf(fout, "\n");
                break;
        }
    }
   
    DeleteTree(root);
    fclose(fin);
    fclose(fout);
				
    return 0;
}

/*
Create new BTree with given order
order: order of BTree (order >= 2)
return: 
        the pointer of new BTree
 */
BNodePtr CreateTree(int order){
    BNodePtr bNode = (BNodePtr)malloc(sizeof(BNode));
    bNode->order = order;
    bNode->size = 1;//최소 두개부터니깐
    bNode->child = (BNodePtr*)malloc(sizeof(BNodePtr) * (order+1));//split할 때 편의상 +1로 해놓음
    bNode->key = (int*)malloc(sizeof(int) * order);//key는 최대 m-1개, insert의 편의를 위해 하나 늘림
    bNode->is_leaf = 1;
    return bNode;
}

/*
Insert the key value into BTree 
key: the key value in BTree node 
*/
BNodePtr split(int parent_pos, BNodePtr parent_node, BNodePtr node){
    int middle_pos = keysLength(node)/2, middleKey = node->key[middle_pos];
    BNodePtr right_node = CreateTree(node->order);//CreateTree()의 order, key배열 동적할당, child할당이 같다.
    right_node->size = 0;//밑에서 child를 하나하나 채워주면서 증가시킬 것이다
    right_node->is_leaf = node->is_leaf;//split이 leaf에서만 일어나는 것은 아니니 1로 해서는 안된다.
    int len = keysLength(node);
    for(int i = middle_pos+1; i < len; i++){//full해져있으니 length가 order와 같아져 있을 것임, 그래도 코드상 length를 사용하겠다
        right_node->key[i-(middle_pos + 1)] = node->key[i];
        node->key[i] = 0;//right_node에 값 복사해주고 넘겨주었으니 0으로 초기화 한다.
    }//key들은 다 옮겨 준 상태, leaf이든 아니든 key들은 옮겨주어야함
    node->key[middle_pos] = 0;//node의 length를 초기화 해준 뒤 중간값에 0으로 수정
    if(!node->is_leaf){ //node가 leaf가 아니라면 child도 같이 옮겨주어야함
        len = node->size;
	for(int i = middle_pos+1; i < len ; i++){
            right_node->child[i-(middle_pos+1)] = node->child[i];//child도 middle_pos를 기준으로 오른쪽을 싹다 옮겨야함
            right_node->size++;
	    node->size--;//child는 size를 멤버변수로 갖고 있으니 따로 null 초기화 필요 없다.
        }
    }
    
    if(parent_node == node){//현 node 위치가 root일 때. 처음 insertkey를 부를 떄, 부모 노드도 root로 초기화 해줬음, parent_pos는 0으로 해줬다.
        BNodePtr new_root = CreateTree(node->order);
        new_root->is_leaf = 0;//새로운 root이기 때문
        new_root->key[0] = middleKey;
	new_root->child[0] = node;
	new_root->child[1] = right_node;
	new_root->size++;
	return new_root;
    }else{//parent node에 split되서 나오는 middleKey를 꽂고, parent_pos기준 하나 오른쪽 child에 right_child를 꽂는다
        len = keysLength(parent_node);
	    for(int i = len; i > parent_pos; i--){//parent_pos를 기준으로 오른쪽들을 싹 옮긴다, 이 때, 한칸씩 옮기는 작업은 child와 동향이 겹침
            parent_node->key[i] = parent_node->key[i-1];
            parent_node->child[i+1] = parent_node->child[i];
        }
	parent_node->key[parent_pos] = middleKey;
    	parent_node->child[parent_pos+1] = right_node;
    	parent_node->size++;
    }
    return node;
}
BNodePtr insertKey(int parent_pos, BNodePtr parent_node, BNodePtr node, int key){
    int node_pos;
    for(node_pos = 0; node_pos < keysLength(node); node_pos++){
        if(key < node->key[node_pos]) break;
    }//if문에 걸렸다면 멈춘 keyPos위치가 들어가야 할 위치, if문 안들어간채로 나왔다면 제일 끝에 key가 들어가는 것
    if(node->is_leaf){
        for(int i = keysLength(node); i > node_pos; i-- ){//끝에서부터 옮겨야 하니 i를 key배열의 끝에서부터 시작하는게 좋다.
            node->key[i] = node->key[i-1];//leaf니깐 child는 안옮겨줘도됨
        }
        node->key[node_pos] = key;//key삽입!!!!!!!!!!!!!!!!!!!!!!!!!!
    }else{//node가 leaf가 아닐때, 다시 이동
        node->child[node_pos] = insertKey(node_pos, node, node->child[node_pos], key);//node로 해도 되지만 node부터 찾아가는 수고를 또 할 필요가 없다
    }

    if(keysLength(node) == node->order){//nodeSplit은 leaf든 아니든 검사하고 full하다면 해야함, 검사하는건 return 바로 위에서 해주어 root까지 올라가는 모든 node를 검사한다.
            node = split(parent_pos, parent_node, node);//parent_pos만으론 parent를 이용할 수 없음, 파라미터로 넘겨주어야함, split에 key는 필요없음(key배열에 middlekey도 있기 때문)
    }
    return node;//node가 leaf일때만 여기에 도달할 수 있음, leaf일땐 split도 필요하면 하고 왔겠다, 이제 재귀를 통해 검사하며 split 필요한 부분은 하면 됨.
}
void Insert(BNodePtr* root, int key){
    *root = insertKey(0, *root, *root, key); //부모도 root로 한다
}


/*
Find node that has key in BTree
key: the key value in BTree node 
*/
int Find(BNodePtr root, int key){
    int pos;
    for(pos = 0; pos < keysLength(root); pos++){//모든 키를 대상으로 비교함
        if(root->key[pos] == key) return 1;
        if(key < root->key[pos]) break;
    }//for문을 나온다면 pos는 마지막 child를 가리키는 index가 된다
    
    if(!root->is_leaf) return Find(root->child[pos], key);
    else return 0;//leaf인데도 1이 return되지 않고 여기까지 온거면 그건 못 찾은거다
}

/* 
Print Tree, inorder traversal 
*/
void PrintTree(BNodePtr root){
    if(root == NULL) return;
    if(!root->is_leaf){
    	for(int i = 0; i < root->size; i++){//모든 child를 들어갈 것이다
     	    PrintTree(root->child[i]);
        	if(i < keysLength(root)) fprintf(fout, "%d ", root->key[i]);//밑 for문 출력하고 올라와서 출력
    	}
    }
    if(root->is_leaf){//leaf일 때만 실행, 외의 노드는 위의 반복문에서 찍어질 것이기 때문이다.)
        for(int j = 0; j < keysLength(root); j++){
            fprintf(fout, "%d ", root->key[j]);
        }
    }
}

/*
Free memory, delete a BTree completely 
*/
void DeleteTree(BNodePtr root){//leaf일 때까지 내려가서 자식들 다 삭제하면 부모로 가서 삭제하고, root까지 올라간다
    if(root == NULL) return;
    if(!root->is_leaf){
    	for(int i = 0; i < root->size; i++){
		DeleteTree(root->child[i]);
    	}
    }
    free(root->child);
    free(root->key);
    free(root);
}

int keysLength(BNodePtr root){//key의 개수를 세어주는 함수를 생성했다, 0보다 큰 값들만 센다.
    int length = 0;
    for(int i = 0; i < root->order; i++){
        if(root->key[i] > 0) length++;
    }
    return length;
}
