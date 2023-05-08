#include<stdio.h>
#include<stdlib.h>
#define INF 1e9


FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
int Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
int* GetElements(Heap* heap);
int IsFull(Heap *heap);
int IsEmpty(Heap *heap);
int Size(Heap *heap);

void swap(int* a, int* b){
	int t = *a;
	*a = *b;
	*b = t;
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;
	int ret;
	int* elements;
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				ret = Insert(maxHeap, key);
				if(ret == 0) fprintf(fout, "insert %d\n", key);
				else if(ret == 1) fprintf(fout, "insert error : heap is full\n");
				else if(ret == 2) fprintf(fout, "insert error : %d is already in the heap\n", key);
				else {
					fprintf(fout, "error: unknown return type\n");
					return -1;
				}

				break;
			case 'd':
				max_element = DeleteMax(maxHeap);
				if(max_element == 0){
					fprintf(fout, "delete error : heap is empty\n");

				}else if(max_element > 0){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				if(IsEmpty(maxHeap)){
					fprintf(fout, "print error : heap is empty\n");
				}else{
					elements = GetElements(maxHeap);
					int size = Size(maxHeap);
					for(int i = 0; elements[i] > -1 && i < size; i++){
						fprintf(fout, "%d ", elements[i]);
					}
					fprintf(fout, "\n");
					free(elements);
				}
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "finding error : %d is not in the heap\n", key);
				break;
		}
	}

	return 0;
}

/*
Create new heap with given heapSize
Element[0] should save INF
heapSize: positive integer
return:
	the pointer of new heap  
*/
Heap* CreateHeap(int heapSize){
	Heap* ptrHeap = (Heap*)malloc(sizeof(Heap));
	ptrHeap->Elements = (int*)malloc(sizeof(Heap)*heapSize+1);//인덱스 1부터 사용할 것이기 때문에 heapSize+1
	ptrHeap->Elements[0] = INF;//인덱스 0은 INF로 할당
	ptrHeap->Capacity = heapSize;
	ptrHeap->Size = 0;//현재 빈 heap이기 때문이다
	return ptrHeap;
}

/*
Insert the value into Heap
value: positive integer
return:
	0 , success
	1 , heap is full
	2 , duplicated
*/
int Insert(Heap *heap, int value){
	int i;
	if(IsFull(heap)) return 1;//이미 다 heap이 다 찬 경우
	else if(Find(heap, value)) return 2;//이미 insert하려는 것이 있는 경우
	//ap->Elements[++Size] = value; 굳이 여기엔 필요 없다는거
	for(i = ++heap->Size; heap->Elements[i/2] < value; i /= 2){
		heap->Elements[i] = heap->Elements[i/2];
	//일단 마지막 노드에 넣는다 가정하고 부모 노드가 자식보다 큰 구조가 지켜지게끔 부모로 올라가면서 검사
	}
	heap->Elements[i] = value;//수정이 끝난 위치에 insert하고자 하는 value를 삽입
	return 0;
}

/*
Find the value in Heap
return:
	1, success
	0, fail  
*/
int Find(Heap* heap, int value){
	for(int i = 1; i <= heap->Size; i++){//1부터 시작하기 때문에 heapSize와 같아질 때까지 확인해야함
		if(heap->Elements[i] == value) return 1;//value와 같으면 찾았다는 것이니 return 1
	}
	return 0;//못찾음
}

/*
Delete the maximum value in Heap
return:
	Max value, success
	0, heap is empty  
*
*
*/
int DeleteMax(Heap* heap){
	int child, i;
	if(IsEmpty(heap)) return 0;//비어있는 경우
	int maxElement = heap->Elements[1];//max는 첫번째에 있음
	for( i = 1; 2 * i <= heap->Size; i = child){
		child = 2 * i;
		if(child < heap->Size && heap->Elements[child] < heap->Elements[child+1]) child++;
		if(heap->Elements[child] < heap->Elements[heap->Size]) child = heap->Size;
		heap->Elements[i] = heap->Elements[child];
	}/*우선 자식은 부모 인덱스에 두배 혹은 그것보다 1큼, 먼저 왼쪽 자식과 오른쪽 자식의 크기를 비교해 큰 값의 index를 child에 저장, 후에 그 child와 heap의 마지막 노드 값을 비교하여 마지막이 크다면 child를 마지막 인덱스로 저장, 자식들 중 크기가 큰 값이 있으면 그것을 현재 확인중인 index에 저장, 이를 for문으로 leaf보다 한단계 위의 level까지 반복한다. 이때 만약 last 노드 값이 왼쪽, 오른쪽 자식보다 크다면 for문 조건에 의해 반복문이 끝나게 됨.*/
	heap->Size--;//값 하나를 지우는 것이니 size를 하나 줄인다
	return maxElement;//max 값을 반환한다.
}

/*
Return an array of copied elements from the heap
The length of the returned array should be the same as the size of the heap
The order of the returned array should be the same as the order of heap->Elements  
If heap is empty, return NULL
***WARNING***
Do not just return heap->Elements
You must copy the elements to another allocated array
*/
int* GetElements(Heap* heap){//array의 index 순서가 level-order이다
	int* arr = (int*)malloc(sizeof(int) * heap->Size);//heap-size만한 배열 할당
	for(int i = 0; i < heap->Size; i++){
		arr[i] = heap->Elements[i+1];//size만큼 for문을 돌며 각 value 복사
	}
	return arr;
}

/*
check heap is full
return:
	1, heap is full
	0, heap is not full
*/
int IsFull(Heap* heap){
	return heap->Capacity == heap->Size;//Capacity와 size가 같으면 다 찬거고, 다르면 덜 찬거
}

/*
check heap is empty
return:
	1, heap is empty
	0, heap is not empty
*/
int IsEmpty(Heap *heap){
	return heap->Size == 0;//size가 0이면 빈 것이고, 아니면 비어있지 않다는 것
}

/*
get size of heap
return:
	size of heap
*/
int Size(Heap *heap){
	return heap->Size;//heap의 size 반환
}

