#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right);
int Partition(Array* array, int left, int right);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

/*
Create new array 
input : size of the number to be sorted
output : the pointer of array
*/
Array* CreateArray(int size){
    Array* array = (Array*)malloc(sizeof(Array));
    array->size = size;
    array->values = (int*)malloc(sizeof(int) * size);
    return array;
}

/*
Quick sort
*/
void QuickSort(Array* array, int left, int right){
    int p;
    if(left < right){//QuickSort하면서 left > p-1 인 상황 p+! > right인 상황이 오게 되고 거기서 끝난다.
        p = Partition(array, left, right);//partition후 pivot의 위치가 반한된다.
        QuickSort(array, left, p-1); //partition후에 나온 왼쪽 부분을 정렬하러 간다
        QuickSort(array, p + 1, right);//오른쪽 부분은 얘가 정렬한다.
    }
}

/*
Divide into 2 sub-lists based on pivot.
+ Select right-most data as pivot
+ Print left, right, pivot index at first
+ Print array values for every swap -> Printing array values should be implemented in a PrintArray function
Print format : 
    1. "left : %d, right : %d, pivot : %d\n"
    2. refered on PrintArray function
return:
    pivot index
*/
int Partition(Array* array, int left, int right){
    int pivot = array->values[right];//pivot은 가장 오른쪽의 값으로 해준다
    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot);
    PrintArray(array, left, right);
    int i = left-1, j = right;//i는 왼쪽에서부터, j는 오른쪽에서부터 check한다
    for(;;){
        while(array->values[++i] < pivot);//비교하고 나서 넘어가면 안되니깐 ++가 선행되어야 함
        while(j >= left && array->values[--j] > pivot);//i는 pivot보다 작은 것, j는 큰 것을 보면 넘어가게 되면서 i와 j에 해당하는 element를 바꿔야 하는 상황이 올때까지 진행된다
        if( i < j){//i가 j보다 작은 상태에서 멈췄다는 것은 그냥 바꿔야 하는 상황이 온 것임
            swap(&array->values[i], &array->values[j]);
            PrintArray(array, left, right);
        }else{//여기에 들어왔다는 것은 i가 j를 넘어갔다는 것이고, 그렇다면 이때 i가 가리키는 값은 분명 pivot보다 큰 값임, right에 있는 pivot을 i번째와 바꿔준다.
            if(i < right) {
                swap(&array->values[i], &array->values[right]); //같은 경우에는 
                PrintArray(array, left, right);
            }//i가 right와 같아진 경우엔 굳이 swap할 필요가 없다.
            return i;
        }
    }
}
// fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot);
/*
Print array values 
Print format :
    “%-3s” : out of bound and should be filled with "-" charater
    “%-3d” : array values to be printed
*/
void PrintArray(Array* array, int left, int right){
    for(int i = 0; i < array->size; i++){
        if(i < left || i > right) fprintf(fout, "%-3s", "-");
        else fprintf(fout, "%-3d", array->values[i]);
    }
    fprintf(fout, "\n");
}
// fprintf(fout, "%-3s", "-");
    // fprintf(fout, "%-3d", array->values[i]);
    // fprintf(fout, "\n");
/*
DeleteArray
*/
void DeleteArray(Array* array){
    free(array->values);
    free(array);
}

/*
Swap values
*/
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
