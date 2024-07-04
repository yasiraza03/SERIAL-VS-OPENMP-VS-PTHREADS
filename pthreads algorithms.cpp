#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 100

// for bubble sort
typedef struct 
{
    int index;
    int* arr;
} 
ThreadArgs1;

void* compareAndSwap(void* arg) 
{
    ThreadArgs1* args = (ThreadArgs1*)arg;
    int index = args->index;
    int* arr = args->arr;

    if (arr[index] > arr[index+1]) 
	{
        // Swap arr[index] and arr[index+1]
        int temp = arr[index];
        arr[index] = arr[index+1];
        arr[index+1] = temp;
    }

    pthread_exit(NULL);
}

void parallelBubbleSort(int arr[], int n) 
{
    pthread_t threads[MAX_SIZE];
    ThreadArgs1 args[MAX_SIZE];

    for (int i = 0; i < n-1; i++) 
	{
        for (int j = 0; j < n-i-1; j++) 
		{
            args[j].index = j;
            args[j].arr = arr;
            pthread_create(&threads[j], NULL, compareAndSwap, (void*)&args[j]);
        }

        for (int j = 0; j < n-i-1; j++) 
		{
            pthread_join(threads[j], NULL);
        }
    }
}

// for merge & quick sort
typedef struct 
{
    int end;
    int start;
    int* arr;
} 
ThreadArgs2;

// merge sort
void merge(int arr[], int left, int middle, int right) 
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    int* leftArr = (int*)malloc(n1 * sizeof(int));
    int* rightArr = (int*)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
        
    for (j = 0; j < n2; j++)
        rightArr[j] = arr[middle + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) 
	{
        if (leftArr[i] <= rightArr[j]) 
		{
            arr[k] = leftArr[i];
            i++;
        } 
		
		else 
		{
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) 
	{
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) 
	{
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    free(leftArr);
    free(rightArr);
}

void* mergeSort(void* arg) 
{
    ThreadArgs2* args = (ThreadArgs2*)arg;
    int start = args->start;
    int end = args->end;
    int* arr = args->arr;

    if (start >= end)
        return NULL;

    int middle = (start + end) / 2;

    ThreadArgs2 args_left = {start, middle, arr};
    ThreadArgs2 args_right = {middle + 1, end, arr};

    pthread_t left_thread, right_thread;
    pthread_create(&left_thread, NULL, mergeSort, (void*)&args_left);
    pthread_create(&right_thread, NULL, mergeSort, (void*)&args_right);

    pthread_join(left_thread, NULL);
    pthread_join(right_thread, NULL);

    merge(arr, start, middle, end);

    pthread_exit(NULL);
}

void parallelMergeSort(int arr[], int n) 
{
    ThreadArgs2 args = {0, n - 1, arr};

    pthread_t main_thread;
    pthread_create(&main_thread, NULL, mergeSort, (void*)&args);
    pthread_join(main_thread, NULL);
}


// quick sort
void swap(int* a, int* b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) 
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) 
	{
        if (arr[j] < pivot) 
		{
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void* quickSort(void* arg) 
{
    ThreadArgs2* args = (ThreadArgs2*)arg;
    int start = args->start;
    int end = args->end;
    int* arr = args->arr;

    if (start < end) 
	{
        int pi = partition(arr, start, end);

        ThreadArgs2 args_left = {start, pi - 1, arr};
        ThreadArgs2 args_right = {pi + 1, end, arr};

        pthread_t left_thread, right_thread;
        pthread_create(&left_thread, NULL, quickSort, (void*)&args_left);
        pthread_create(&right_thread, NULL, quickSort, (void*)&args_right);

        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);
    }

    pthread_exit(NULL);
}

void parallelQuickSort(int arr[], int n) 
{
    ThreadArgs2 args = {0, n - 1, arr};

    pthread_t main_thread;
    pthread_create(&main_thread, NULL, quickSort, (void*)&args);
    pthread_join(main_thread, NULL);
}

int main() 
{
	int i, size;
	clock_t start, end;
    float cpu_time_used;

    start = clock(); // start recording time for a sorting algorithm
	
	//	for 100 & 1000 elements
//	int arr[size];
//	for (i = 0; i < 1000; i++) 
//	{
//        arr[i] = rand() % 1000 + 1; // Generate a random positive integer between 1 and 1000
//    }
	
    int arr[] = {64, 34, 25, 12, 22, 11, 90, 55, 78, 47};
    int n = sizeof(arr) / sizeof(arr[0]);

    parallelBubbleSort(arr, n);
//	parallelMergeSort(arr, n);
//	parallelQuickSort(arr, n);
	 
    printf("Sorted array: \n");
    for (int i = 0; i < n; i++)
    {
    	printf("%d ", arr[i]);
	}
	
	end = clock(); // stop recording time
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//  float milliseconds = cpu_time_used * 1000;

    printf("\n\nExecution Time: %f seconds\n", cpu_time_used);

    return 0;
}

