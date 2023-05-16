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
		printf("%d\n", root->key[0]);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
		printf("%d\n", root->key[0]);
                break;
            case 'p':
                if (root->size == 1)
                    fprintf(fout, "print error : tree is empty!");
                else
                    PrintTree(root);
                fprintf(fout, "\n");
		printf("%d\n", root->key[0]);
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
    bNode->size = 0;//최소 두개부터니깐
    bNode->child = (BNodePtr*)malloc(sizeof(BNodePtr) * (order+1));
    bNode->key = (int*)malloc(sizeof(int) * order);//key는 최대 m-1개, insert의 편의를 위해 하나 늘림
    bNode->is_leaf = 1;
    return bNode;
}

/*
Insert the key value into BTree 
key: the key value in BTree node 
*/
BNodePtr split(int pos, BNodePtr node, BNodePtr parentNode){
    int middle_pos = keysLength(node)/2, middleKey = node->key[middle_pos];
    BNodePtr right_node = (BNodePtr)malloc(sizeof(BNode));
    right_node->is_leaf = node->is_leaf;
    right_node->size = 0;
    right_node->order = node->order;
    right_node->child = (BNodePtr*)malloc(sizeof(BNodePtr) * (node->order+1));
    right_node->key = (int*)malloc(sizeof(int) * node->order);
    
    int len = keysLength(node);
    for(int i = middle_pos+1; i< len; i++){ // 분리할 노드에 키 담기(리프이든 아니든)
        right_node->key[i-(middle_pos+1)] = node->key[i];
        node->key[i] = 0;
    }

    if (!node->is_leaf){ // 현재 노드가 리프가 아니면, 자식 담기
	    len = node->size;//확인
            for(int i = middle_pos+1; i < len; i++){//시2발
	    	right_node->child[i-(middle_pos+1)] = node ->child[i];
            	right_node-> size++; // 새로 채워준 노드의 자식 개수는 증가, 현재 노드에서는 빼기
            	node->size--;//size의 개수만큼으로 접근을 하겠다는 거지
	    }   
    }
    node->key[middle_pos] = 0;
    if(node == parentNode){//현재 노드가 루트 노드인지 확인.., node == parentNode로 할 수도 있음
	    BNodePtr new_parent_node = CreateTree(node->order); // 중앙값 가지고 새 부모 노드 만들기
        new_parent_node->is_leaf = 0;
        new_parent_node->key[0] = middleKey;
        new_parent_node->child[0] = node; // 새부모노드의 왼쪽 자식은 현재 노드
        new_parent_node->child[1] = right_node;
        new_parent_node->size = 2;
        return new_parent_node; 
    }else{
        for (int i= keysLength(parentNode); i> pos; i--){ // 부모 노드에 넣어야되니까 거기있던 키 배치 다시하기
            parentNode->key[i] = parentNode->key[i-1];
            parentNode->child[i+1] = parentNode->child[i];
        }

        parentNode->key[pos]= middleKey; // 부모 노드에 넣어야될 자리에 값 넣기
        parentNode-> child[pos+1] = right_node; // 왼쪽 노드는 원래 연결되어있으니 오른쪽만 부모노드에 연결.
        parentNode->size++;
    }
    return node;
}
BNodePtr insertNode(int parent_pos, int key, BNodePtr node, BNodePtr parentNode){
    int pos; // 현재 노드에서 키의 위치를 갖고 있어야 함. 왜냐면 넣으려고 하는 값의 위치를 찾아야 하기 때문.
    for (pos =0; pos < keysLength(node); pos++ ) {// pos 위치는 0부터 해서, 현재 노드의 키 개수만큼 탐색
	    if (key< node->key[pos]){ // val이 node의 pos번째 키보다 작으면 그 pos에서 멈춘다.
            break;
        }
    }
    
    if(node->is_leaf==0){
        node->child[pos] = insertNode(pos, key, node->child[pos], node);
        if (keysLength(node) == node->order){ // 현재 노드 키 개수가 규칙에서 벗어날거같으면
            node = split(parent_pos, node, parentNode); // 윗 방향으로 분리를 해야 함.
        }
    }else{
        for(int i = keysLength(node); i > pos; i--){
            node->key[i] = node->key[i-1];
            //node->child[i+1] = node->child[i];//leaf인데 옮기는 이유가 뭐지
        }//수정해볼만함
        node->key[pos] = key;
        if(keysLength(node) == node->order){
            node = split(parent_pos, node, parentNode);//pos인것 같은데
        }
    }
    return node;//재귀할 떄 필요한 return
}
void Insert(BNodePtr* root, int key){

    *root = insertNode(0, key, *root, *root);
}
/*
Find node that has key in BTree
key: the key value in BTree node 
*/
int Find(BNodePtr root, int key){
    for(int i = 0; i < keysLength(root); i++){
        if(root->is_leaf == 0){
            if(key < root->key[i]) return Find(root->child[i], key);
            else if(key == root->key[i]) return 1;
            if(i == keysLength(root)-1) return Find(root->child[i+1], key);
        }else{
            if(key == root->key[i]) return 1;
        }
    }
    return 0;
}

/* 
Print Tree, inorder traversal 
*/
void PrintTree(BNodePtr root){
	if(!root) return;
    if(root->is_leaf == 0){
    	for(int i = 0; i < root->size; i++){
            PrintTree(root->child[i]);
	    if(i < keysLength(root)) fprintf(fout, "%d ", root->key[i]);
	    continue;
        }
    }else{
        for(int j = 0; j < keysLength(root); j++){
            fprintf(fout, "%d ", root->key[j]);
        }
    }
}

/*
Free memory, delete a BTree completely 
*/
void DeleteTree(BNodePtr root){
    for(int i = 0; i < root->size; i++){
            //printf("%d r-s%d\n ", root->key[0], root->size);
	    DeleteTree(root->child[i]);
	    continue;
    }
    if(!root) return;
    printf("%d r-s%d\n ", root->key[0], root->size);
    free(root);
   
}

int keysLength(BNodePtr root){
    int length = 0;
    for(int i = 0; i < root->order; i++){
        if(root->key[i] > 0) length++;
    }
    return length;
}
