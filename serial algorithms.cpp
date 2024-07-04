#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//#define MAX_SIZE 100 // for 100 & 1000 elements

// bubble sort
void bubbleSort(int arr[], int n) 
{
    int i, j;
    for (i = 0; i < n-1; i++) 
	{
        for (j = 0; j < n-i-1; j++) 
		{
            if (arr[j] > arr[j+1]) 
			{
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// merge sort
void merge(int arr[], int left, int middle, int right) 
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create temporary arrays
    int leftArr[n1], rightArr[n2];

    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        rightArr[j] = arr[middle + 1 + j];

    i = 0; 
    j = 0; 
    k = left; // Initial index of merged subarray

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

    // Copy the remaining elements of leftArr[]
    while (i < n1) 
	{
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArr[]
    while (j < n2) 
	{
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int left, int right) 
{
    if (left < right) 
	{
        int middle = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);

        // Merge the sorted halves
        merge(arr, left, middle, right);
    }
}

// quick sort
void swap(int* a, int* b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition the array using the last element as the pivot
int partition(int arr[], int low, int high) 
{
    int pivot = arr[high];  
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high - 1; j++) 
	{
        // If the current element is smaller than or equal to the pivot
        if (arr[j] <= pivot) 
		{
            i++;  
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Recursive function to perform quick sort
void quickSort(int arr[], int low, int high) 
{
    if (low < high) 
	{
        // Partition the array
        int pi = partition(arr, low, high);

        // Sort the subarrays recursively
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() 
{
	int i;
	clock_t start, end;
    float cpu_time_used;

    start = clock(); // start recording time for a sorting algorithm
	
	//for 100 & 1000 elements
//	int arr[MAX_SIZE];
//	for (i = 0; i < MAX_SIZE; i++) 
//	{
//        arr[i] = rand() % 1000 + 1; // Generate a random positive integer between 1 and 1000
//    }
	
    int arr[] = {64, 34, 25, 12, 22, 11, 90, 55, 78, 47}; // 10 elements
    int n = sizeof(arr)/sizeof(arr[0]);

    bubbleSort(arr, n);
//	mergeSort(arr, 0, n - 1);
//	quickSort(arr, 0, n - 1);
	
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

