#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node {
	int vertex;
	int dist;	//distance
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** vertices;
	Node* nodes;
}Graph;

typedef struct Heap {
	int Capacity;
	int Size;
	Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void dijkstra(Graph* g);
int* shortestPath(Graph* g, int dest);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);
int Find(Heap* minHeap, int vertex);

void swap(Node* node1, Node* node2) {
	Node temp = *node1;
	*node1 = *node2;
	*node2 = temp;
}

void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = createGraph(size + 1);

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->vertices[node1][node2] = weight;
		tmp = fgetc(fin);
	}

	dijkstra(g);

	int j;
	for(int i = 2; i <= g->size; i++){
		int* path = shortestPath(g, i);
		if(path == NULL){
			fprintf(fout, "can not reach to node %d\n", i);
			continue;
		}
		for (j = g->size - 1; j > 0; j--){
			if(path[j] == 0) continue;
			fprintf(fout, "%d->", path[j]);
		} 
		fprintf(fout, "%d (cost : %d)\n", i, g->nodes[i].dist);
		free(path);
	}
	deleteGraph(g);
}
/*
Allocate Graph Matrix

Initial distance: INF except 1st node (source)
Initial prev: -1
*/
Graph* createGraph(int size) {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->size = size-1;
    g->vertices = (int**)malloc(sizeof(int*) * size);//index가 1부터 시작
    for(int i = 0; i < size; i++){
        g->vertices[i] = (int*)malloc(sizeof(int) * size);
    }
    g->nodes = (Node*)malloc(sizeof(Node) * size);
    for(int i = 1; i < size; i++){//처음 상태 초기화
        g->nodes[i].vertex = i;
        g->nodes[i].dist = INF;
        g->nodes[i].prev = -1;
    }
    g->nodes[1].dist = 0;
    return g;
}
// deallocate graph
void deleteGraph(Graph* g){
    for(int i = 0; i < g->size+1; i++){
        free(g->vertices[i]);
    }
    free(g->vertices);
    free(g->nodes);
    free(g);
}
/*
Allocate min heap
*/ 
Heap* createMinHeap(int heapSize) {
    Heap* minHeap = (Heap*)malloc(sizeof(Heap));
    minHeap->Capacity = heapSize;
    minHeap->Size = 0;
    minHeap->Element = (Node*)malloc(sizeof(Node) * (heapSize+1));//index를 1부터로 할 것이다
}
/*
Deallocate min heap
*/
void deleteMinHeap(Heap* minHeap){
    free(minHeap->Element);
    free(minHeap);
}
/*
Dijkstra Algorithm
*/
void dijkstra(Graph* g){
    int start = 1, pos;
    Heap* minHeap = createMinHeap(g->size);
    Node min;
    insertToMinHeap(minHeap, g->nodes[start].vertex, g->nodes[start].dist);//start를 넣어놓고 시작
    while(minHeap->Size > 0){
        min = deleteMin(minHeap);//현 위치에서 dist가 가장 작은 곳으로 간것이다. 현 위치가 여기로 바뀐것이다.
        start = min.vertex;
        for(int i = 1; i <= g->size; i++){//여기서 i는 g->nodes에 저장된 노드에 해당하는 index임
            if(g->vertices[start][i] && min.dist + g->vertices[start][i] < g->nodes[i].dist){//
                g->nodes[i].dist = min.dist + g->vertices[start][i];//update
                g->nodes[i].prev = start;//update되었다면 prev도 당연히 바뀌어야 함
                pos = Find(minHeap, g->nodes[i].vertex);//pos는 minHeap에서의 위치
                if(pos == -1) insertToMinHeap(minHeap, g->nodes[i].vertex, g->nodes[i].dist);//update된 애들중 minHeap에 없는 애들만 넣는다
                else minHeap->Element[pos].dist = g->nodes[i].dist;//이미 있던 노드라면 dist만 수정해준다
            }
        }
    }
    deleteMinHeap(minHeap);
}
int Find(Heap* minHeap, int vertex){
    for(int i = 1; i <= minHeap->Size; i++){//minHeap에서 위치 찾기
        if(minHeap->Element[i].vertex == vertex){
            return i;
        }
    }
    return -1;//minHeap에 없다면 -1 반환
}
/*
Return shortest path from source(1) to dest(i)
Return type is array and the size is the number of node.
You should save the path reversely. 
For example, If shortest path is 1->2->3 
and the number of node(g->size) is 5,
you must return [3, 2, 1 , 0, 0]
If there is no path, return NULL 
*/
int* shortestPath(Graph* g, int dest){//prev를 따라가면 shortestPath
    int i = 0;
    int* path = (int*)malloc(sizeof(int) * g->size);
    path[i++] = dest;
    while(i < 5){
        if(g->nodes[dest].prev != -1){
            path[i] = g->nodes[dest].prev;
            dest = g->nodes[dest].prev;
        }
        else if(dest == 1)  path[i] = 0;
        else return NULL;
        i++;
    }
    return path;
}
/*
Insert Node with vertex and distance into minHeap
*/
void insertToMinHeap(Heap* minHeap, int vertex, int distance) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->dist = distance;
    minHeap->Element[++minHeap->Size] = (*newNode);//heap제일 마지막에 넣어놓고
    for(int i = minHeap->Size; minHeap->Element[i/2].dist > distance; i /= 2){//여기서 위치 재정렬
        swap(&(minHeap->Element[i/2]), &(minHeap->Element[i/2]));
    }
}
/*
pop the Node with minimum distance from minHeap
return:
	Node with minimum distance
*/
Node deleteMin(Heap *minHeap) {//왼쪽 오른쪽 child와 마지막 leaf노드 세개를 비교하며 내려가기
    int i, child;
    int lastPos = minHeap->Size--;
    for(i = 1; i * 2 <= minHeap->Size; i = child){
        child = i * 2;
        if(child < minHeap->Size && minHeap->Element[child+1].dist < minHeap->Element[child].dist) child++;
        if(minHeap->Element[lastPos].dist < minHeap->Element[child].dist){
            break;
        }
        swap(&(minHeap->Element[i]), &(minHeap->Element[child]));
    }
    swap(&(minHeap->Element[i]), &(minHeap->Element[lastPos]));
    return minHeap->Element[lastPos];
}

