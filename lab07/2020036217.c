#include<stdio.h>
#include<stdlib.h>
#include<time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
	int size_maze;
	int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);
int isJoint(DisjointSets *sets);

int main(int argc, char* agrv[]) {
	srand((unsigned int)time(NULL));

	int num, i;
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	DisjointSets *sets, *maze_print;
	fscanf(fin, "%d", &num);
	sets = (DisjointSets*)malloc(sizeof(DisjointSets));
	maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

	init(sets, maze_print, num);
	createMaze(sets, maze_print, num);
	printMaze(maze_print, num);

	freeMaze(sets, maze_print);

	fclose(fin);
	fclose(fout);

	return 0;
}
/*
Allocate and Initialize Disjoint sets
    "sets": have num*num disjoint sets
    "maze_print": have num*num*2 values (two directions: right, down)   
        the values are either 0(no wall) or 1 (wall)
*
* 각 방마다 오른쪽 벽과 아래 벽을 갖고있어야 하므로 maze_print의 size는 sets의 두배로 한다.
*/
void init(DisjointSets *sets, DisjointSets *maze_print, int num) {
	maze_print->size_maze = num*num*2;
	maze_print->ptr_arr = (int*)malloc(sizeof(int) * (maze_print->size_maze));
	for(int i=0; i < maze_print->size_maze; i++){
		(maze_print->ptr_arr)[i] = 1;//0부터시작하니깐 짝수는 오른쪽 벽, 홀수는 아래 벽을 의미
	}
	maze_print->ptr_arr[maze_print->size_maze-2] = 0;//출구 오른쪽 벽은 없는걸로
	sets->size_maze = num*num + 1;//강의자료와 다르게 index0부터 쓰자
	sets->ptr_arr = (int*)malloc(sizeof(int) * (sets->size_maze));
	for(int i=1; i < sets->size_maze; i++){
		(sets->ptr_arr)[i] = 0;
	}
	//처음엔 모두가 Root이므로 0으로 초기화
}

/*
Merge two disjoint sets including i and j respectively
*/
void Union(DisjointSets *sets, int i, int j) {//뭐를 어디에 연결시켜야할지 정해지지 않았으므로 각 경우를 다 비교, root index를 parameter로 받게하고 rank가 작은 노드에 연결되도록 한다.
	if(sets->ptr_arr[i] < sets->ptr_arr[j]){
		sets->ptr_arr[j] = i;
	}else if(sets->ptr_arr[i] == sets->ptr_arr[j]){
		sets->ptr_arr[j] = i;
		sets->ptr_arr[i]--;
	}else sets->ptr_arr[i] = j;
}
/*
Find the set including given element "i" and return the representative element  
*
* value가 0보다 작거나 같을 때까지 find를 재귀적으로 반복하여 반복이 끝났을 때 해당 index를 반환
*/
int find(DisjointSets *sets, int i) {//재귀로 해보고 싶었음
	if(sets->ptr_arr[i] > 0){
		return find(sets, sets->ptr_arr[i]);
	}else return i;//부모노드에 저장된 값이 아닌 진짜 index가 필요함, 그게 여기서 부모니깐
}
/*
Create Maze without cycle
You should generate the maze by randomly choosing cell and direction 
*
* 모든 방이 연결 될 때까지 union을 하는데 인접한 방끼리만 가능하도록 설계한다.
*/
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num) {
	while(!isJoint(sets)){//입구와 출구가 하나의 set으로 연결될 때까지
		int a = rand() % (num*num) + 1, b = rand() % (num*num) + 1;
		if(a - b != 1 && a - b != -1 && a - b != num && a - b != -num ) continue;//위아래 인접 조건
		else if( a % num + b % num == 1) continue;//오른쪽 끝과 그 다음 인덱스는 실제로 인접하지 않음
		else{
			if(find(sets, a) != find(sets, b)){//cycle이 발생하지 않기 위한 조건
				Union(sets, find(sets, a), find(sets, b));
				if(a - b == 1) maze_print->ptr_arr[(b-1)*2] = 0;//스위치가 더 보기 힘들 듯
				else if(a - b == -1) maze_print->ptr_arr[(a-1)*2] = 0; 
				else if(a - b == num) maze_print->ptr_arr[b*2 - 1] = 0; 
				else if(a - b == -num) maze_print->ptr_arr[a*2 - 1] = 0;//인접 상황에 맞게 초기화
			}else continue;
		}
	}//while문이 끝났다는건 입구와 출구가 하나의 set이 되었다는 것
}//cycle: 이미 joint한걸 다시 joint해서는 안된다

/*
Print Maze
Example> 
+---+---+---+---+---+
            |       |
+   +   +---+   +---+
|   |       |       |
+---+   +---+   +   +
|               |   |
+   +---+   +   +---+
|   |       |       |
+   +   +---+---+---+
|   |                
+---+---+---+---+---+
*/
// 0일때는 벽이 없이 1일때는 벽 있이, 필수적으로 있어야하는 벽은 직접 print한다.
void printMaze(DisjointSets *sets, int num) {
	for(int i=0; i < num; i++){
		fprintf(fout, "+---");
	}
	fprintf(fout, "+\n");//맨 윗 줄
	for(int i = 0; i < num; i++){
		if(i==0) fprintf(fout, " ");
		else fprintf(fout, "|");
		for(int j = num*i; j < num*(i+1); j++){//세로 줄 출력
			if(sets->ptr_arr[2*j] == 1) fprintf(fout, "   |");
			else if(sets->ptr_arr[2*j] == 0) fprintf(fout, "    ");//0일 때 공백 4개로 출력
		}
		fprintf(fout, "\n");
		fprintf(fout, "+");
		for(int j = num*i; j < num*(i+1); j++){//가로 줄 출력
			if(sets->ptr_arr[2*j + 1] == 1) fprintf(fout, "---+");
			else if(sets->ptr_arr[2*j + 1] == 0) fprintf(fout, "   +");//0일 때 공백3 "+"4개로 출력
		}
		fprintf(fout, "\n");
	}
}
/*
Deallocate sets
*
* 부모와 rank가 담긴 sets와 미로를 구현하기 위해 만든 maze_print 삭제
*/
void freeMaze(DisjointSets *sets, DisjointSets * maze_print) {
	free(sets->ptr_arr);
	free(maze_print->ptr_arr);
	free(sets);
	free(maze_print);
}
//입구부터 모든 방이 모두 연결되었는지 확인한다.
int isJoint(DisjointSets *sets){
        for(int i=1; i < sets->size_maze; i++){
                if(find(sets, 1) != find(sets, i)) return 0;
        }
        return 1;
}
