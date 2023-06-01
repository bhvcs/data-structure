#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef int ElementType;
typedef ElementType List;
typedef struct HashTbl* HashTable;

typedef struct HashTbl{
	int TableSize;
	List *TheLists;
}HashTbl;

HashTable createTable(int TableSize);
void Insert(HashTable H, ElementType Key, int solution);
void Delete(HashTable H, ElementType Key, int solution);
int Find(HashTable H, ElementType Key, int solution);
void printTable(HashTable H);
void deleteTable(HashTable H);
int hashing(List* TheLists, int size, ElementType Key, int collide);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");


	char solution_str[20];
	int solution, TableSize;

	fscanf(fin, "%s", solution_str);
	if(!strcmp(solution_str, "linear"))
		solution = 1;
	else if(!strcmp(solution_str, "quadratic"))
		solution = 2;
	else{
		fprintf(fout, "Error: Invalid hashing solution!");
		return 0;
	}
	
	fscanf(fin, "%d", &TableSize);

	HashTable H = createTable(TableSize);

	char cv;
	int key;
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				Insert(H, key, solution);
				break;

			case 'f':
				fscanf(fin, "%d", &key);
				int result = Find(H, key, solution);
				if(result)
					fprintf(fout, "%d is in the table\n", key);
				else
					fprintf(fout, "%d is not in the table\n", key);
				break;

			case 'd':
				fscanf(fin, "%d", &key);
				Delete(H, key, solution);
				break;

			case 'p':
				printTable(H);
				break;
		}
	}

	deleteTable(H);

	return 0;
}

/*
Create new Hash Table with given TableSize
reture:
	the pointer of new hash table
*/
HashTable createTable(int TableSize){
    HashTable newTable = (HashTable)malloc(sizeof(HashTbl));
    newTable->TableSize = TableSize;
    newTable->TheLists = (ElementType*)malloc(sizeof(ElementType)*TableSize);
    return newTable;
}

/*
Insert the key in hash table with given solution (linear or quadratic).
print out : 
	Successful insertion : "insert %d into address %d\n"
	When inserting duplicate key value : "insertion error : %d already exists at address %d\n"
	When HashTable is full : "insertion error : table is full\n"
*/
void Insert(HashTable H, ElementType Key, int solution){
	int collide = 0, pos = 0;
	for(;;){//Find를 활용하면 address를 찾을 수가 없다
		if(solution == 1) {
			pos = hashing(H->TheLists, H->TableSize, Key, collide);
		}
		else {
			pos = hashing(H->TheLists, H->TableSize, Key, collide * collide);
		}
		//hashing하고 와서 pos에 반환값 저장
		if(collide > H->TableSize){ //못찾았고 table size 이상 탐색했을 때 그만 탐색해야함
			fprintf(fout, "insertion error : table is full\n");
			return;
		}
		if(H->TheLists[pos] == 0) break;//완전히 빈 경우
		else if(pos > 0 && H->TheLists[pos] == Key){//대상을 찾은 경우
			fprintf(fout, "insertion error : %d already exists at address %d\n", Key, pos);
			return;
		}else if(H->TheLists[pos] == -2){//Del값인 경우
			break; //반복문을 빠져 나가서 삽입 동작이 이루어지면 된다
		}
		else collide++;
	}
	H->TheLists[pos] = Key;//비었을 때 삽입
	fprintf(fout, "insert %d into address %d\n", Key, pos);
}
/*
Delete the key in hash table with given solution (linear or quadratic).
print out : 	
	when key value is found :  "delete %d\n"
	when key value does not exist : %d is not in the table\n"
*/
void Delete(HashTable H, ElementType Key, int solution){
	int collide = 0, pos = 0, m = H->TableSize;
	while(collide <= m / 2){//while문이 끝나기 위해선 pos가 -1 or == Key여야 한다, collide가 H->TableSize를 넘어가면 더 탐색할 필요 없다(확인한 부분만 계속 확인)
		if(solution == 1) pos = hashing(H->TheLists, m, Key, collide);
		else pos = hashing(H->TheLists, m, Key, collide * collide);

		if(H->TheLists[pos] == 0) break;//완전히 비어있는 경우
		else if(pos > 0 && H->TheLists[pos] == Key){//대상을 찾은 경우, Del값을 준다
			H->TheLists[pos] = -2;
			fprintf(fout, "delete %d\n", Key);
			return;
		}
		else collide++;//pos가  -3 즉 해당 키와 다르다 (다만 pos가 -1이어도 결국 돌아가서 끝나게 됨 collide가 증가되는 부분을 타고 올라가야 되는게 보기엔 안 좋다)
	}
	fprintf(fout, "deletion error: %d is not in the table\n", Key);
}
/*
Find the Key from hash table with given solution (linear or quadratic).
return:
	0 : when the key is not found
	1 : when the key is found
*/
int Find(HashTable H, ElementType Key, int solution){
	int collide = 0, pos = 0;
	while(collide <= H->TableSize / 2){
		if(solution == 1) pos = hashing(H->TheLists, H->TableSize, Key, collide);
		else pos = hashing(H->TheLists, H->TableSize, Key, collide * collide);
		if(H->TheLists[pos] == 0) break;//완전히 비어있는 경우
		else if(pos > 0 && H->TheLists[pos] == Key) return 1;//c언어에 shortcut이 없다면 segmentation fault가 뜰 것이다
		else collide++;//Del인 경우 || Key와 다른 경우
	}
	return 0;
}
int hashing(List* TheLists, int size, ElementType Key, int collide){//Del을 다루는건 find, delete에선 똑같다
	int i = collide;
	int pos = (Key%size + i) % size;
	return pos;
}//insert의 작동방식이 find, delete와는 달라 반복문으로 설계함

/*
Print all values of Hash Table
print out the key values ​​in the Hash Table in the order of index in the Hash Table.
	empty Hash Table print : "0 "
	Non empty Hash Table print : "%d "
*/
void printTable(HashTable H){
	for(int i = 0; i < H->TableSize; i++){
		if(H->TheLists[i] > 0) fprintf(fout, "%d ", H->TheLists[i]);//양수 일 떄만 저장된 값 출력
		else fprintf(fout, "0 ");
	}
	fprintf(fout, "\n");
}
/*
delete Table 
*/
void deleteTable(HashTable H){
	free(H->TheLists);
	free(H);
}
