#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue {
	int* key;
	int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	int** matrix;
}Graph;

Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void DeleteGraph(Graph* G);
int* Topsort(Graph* G);
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
int* sorting(int* nodes, int n);
void countInput(int* n, char* numbers){
    char nums[100];
    strcpy(nums, numbers);
    char* ptr = strtok(nums, " ");
    while(ptr != NULL){
        (*n)++;
        ptr = strtok(NULL, " ");
    }
}
void parseInput(int* nodes, char* numbers, int n){
    char* ptr = strtok(numbers, " ");
    int i = 0;
    while(ptr != NULL){
        nodes[i++] = atoi(ptr);
        ptr = strtok(NULL, " ");
    }
}

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	char numbers[100], inputs[150];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
    int* nodes = (int*)malloc(sizeof(int)*n);
	int* sorted_arr = NULL;
	parseInput(nodes, numbers, n);

	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	// PrintGraph(G);
	fprintf(fout, "%-3s", " ");
	for (i = 0; i < G->size; i++) fprintf(fout, "%-3d", G->node[i]);
	fprintf(fout, "\n");
	for (i = 0; i < G->size; i++) {
		fprintf(fout, "%-3d", G->node[i]);
		for (j = 0; j < G->size; j++) fprintf(fout, "%-3d", G->matrix[i][j]);
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n");

	sorted_arr = Topsort(G);
	if(sorted_arr == NULL){
		fprintf(fout, "sorting error : cycle!");
	}else{
		for(i = 0; i < G->size; i ++)
			fprintf(fout, "%d ", sorted_arr[i]);
		fprintf(fout, "\n");
	}
	free(sorted_arr);
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}

/*
Create new graph with given nodes and the count of nodes.
saved nodes should be sorted.
return: 
        the pointer of new graph
 */
Graph* CreateGraph(int* nodes, int n) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->size = n;
    graph->node = sorting(nodes, n);//sort된 nodes로 초기화
    graph->matrix = (int**)malloc(sizeof(int*) * n);
    for(int i = 0; i < n; i++){//처음엔 edge가 없으므로 2차원 배열을 할당하면서 0으로 모두 초기화한다.
        graph->matrix[i] = (int*)malloc(sizeof(int) * n);
        for(int j = 0; j < n; j++){
            graph->matrix[i][j] = 0;
        }
    }
    return graph;
}
/*
Insert edge from a to b in the graph.
*/
void InsertEdge(Graph* G, int a, int b) {
    for(int i = 0; i < G->size; i++){
        if( G->node[i] == a) a = i;
        else if( G->node[i] == b) b = i;
    }
    G->matrix[a][b] = 1;//edge가 있는 곳을 1로 할 것인데, 이미 edge가 있더라도 상관없다.
}
/*
Deallocate the memory of the graph G.
*/
void DeleteGraph(Graph* G){//node 해제, matrix해제, TODO: Queue를 어디서 Delete할 것인지 생각
    free(G->node);
    for(int i = 0; i < G->size; i++){
        free(G->matrix[i]);
    }
    free(G->matrix);
    free(G);
}

int CheckIndeg(Queue* queue, Graph* G, int* checkList){
    int isAllZero, i, j, state = 0;//i가 기준이 되고, j는 하나하나 돌면서 지우는데 사용, enqueue되는게 없다면 cycle이 있는것->state로 확인(확인 필요)
    for(i = 0; i < G->size; i++){
        isAllZero = 1;
        if(checkList[i] == -1) continue;//해당 index의 checkList가 -1이라면 그 부분은 이전에 Enqueue된 요소이고, 빼고 체크한다.
        for(j = 0; j < G->size; j++){
            if(G->matrix[j][i] != 0){//열 하나를 검사하면서 0이 아닌 요소가 포함되어있다면 isAllZero를 0으로 만들고 그 열에서의 검사를 멈춘다.
                isAllZero = 0;
                break;
            }
        }
        if(isAllZero){//여기서 outgoing을 지우게 되면, 자기 혼자 다음단계로 넘어갈 수도 있음(이래서 최대한 기능을 소분하는 것이 중요한 것 같다)
            state = 1;
            Enqueue(queue, G->node[i]);//검사를 해서 열의 모든 요소가 0이라면 Enqueue한다. 이때 이미 sorting되어 있기 때문에 priority는 자동적으로 지켜진다.
            checkList[i] = -1;//isAllZero를 확인해서 Enqueue된 애들은 다음 CheckIndeg때 확인하지 않기 위해 -1로 지정
        }
    }
    return state;//indeg가 0인게 있다면 isAllZero 조건문에서 state가 1로 되었을 것임
}
void DeleteOutgoing(Graph* G, int target){
    int idx;
    for(idx = 0; idx < G->size; idx++){//node의 몇 번째 index에 위치하는지 찾을 것이다.
        if(G->node[idx] == target) break;
    }//찾음

    for(int k = 0; k < G->size; k++){//Degueue된 vertex의 행에서 edge가 있는 애들을 없앰
        if(G->matrix[idx][k] == 1) G->matrix[idx][k] = 0;
    }
}
/*
Topological sort
If two or more nodes have same priority,
    the node with the smaller number takes precedence.
return:
    the array of sorted
    NULL if the graph has a cycle
*/
int* Topsort(Graph* G){//Topsort가 된 배열을 반환해야함, TODO: cycle 확인 필요: 더이상 Dequeue 할게 없는데 indeg가 0인 것도 없을 때 cycle이 존재한다.
    int* topSorted_array = (int*)malloc(sizeof(int) * G->size);
    int* checkList = (int*)malloc(sizeof(int) * G->size);
    for(int i = 0; i < G->size; i++){//각 단계마다 확인해야하는 index 리스트를 만듬
        checkList[i] = 1;
    }
    int idx = 0;//dequeue된 것을 array에 편하게 저장하기 위한 idx
    Queue* queue = MakeNewQueue(G->size);//일단 Topsort할 떄 사용할 Queue를 만들어야함
    while(idx != G->size){//idx가 G->size와 같아졌다는 것은 topSorted_array에 다 채웠우고 왔다는 이야기
        if(queue->rear < queue->first && !CheckIndeg(queue, G, checkList)) return NULL;//맨 처음 indeg가 0인 애들을 queue에 enqueue할 것이다. topSorted_array에서 있는지 확인하는 것보다 checkList로 확인하는 것이 낫다고 판단 됨.
        topSorted_array[idx] = Dequeue(queue);
        //printf("ts_arrary: %d\n", topSorted_array[idx]);
        DeleteOutgoing(G, topSorted_array[idx++]);
    }
    DeleteQueue(queue);//Queue를 다 사용했으니 메모리 해제
    return topSorted_array;//cycle이 있다면 위에서 NULL이 반환 될 것이다.
}

/*
Create new Queue with maximum size X
return:
        the pointer of new queue
*/
Queue* MakeNewQueue(int X){
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->max_queue_size = X;
    queue->key = (int*)malloc(sizeof(int)*X);
    queue->first = 0;
    queue->rear = -1;
    queue->qsize = 0;
    return queue;
}
/*
Check the queue either is empty or not.
return:
        1, empty
        0, non-empty
*/
int IsEmpty(Queue* Q){
    return Q->rear < Q->first;
}
/*
pop the front key in queue
return:
        the front key in queue
*/
int Dequeue(Queue* Q){
    if(IsEmpty(Q)) return -1;//비어있는 경우엔 -1을 반환하게 한다.
    Q->qsize--;//하나 뺄 것이기 때문에 qsize를 하나 감소시킨다.
    return Q->key[Q->first++];//first에 해당하는 index 요소를 반환하고 first는 하나 증가시킨다.
}
/*
push the key in queue
*/
void Enqueue(Queue* Q, int X){//rear의 위치를 하나 증가시킨 자리에 삽입할 X를 넣고 qsize를 하나 증가 시킨다.
    if(Q->qsize == Q->max_queue_size) return;//이미 full한 경우
    Q->key[++Q->rear] = X;
    Q->qsize++;
}

/*
Deallocate the memory of the queue
*/
void DeleteQueue(Queue* Q){
    free(Q->key);
    free(Q);
}

int* sorting(int* nodes, int n){
    int min, temp;
    for(int i = 0; i < n-1; i++){
        min = i;
        for(int j = i; j < n; j++){
            if(nodes[j] < nodes[min]){
                min = j;
            }
        }
        temp = nodes[i];
        nodes[i] = nodes[min];
        nodes[min] = temp;
    }
    return nodes;
}
