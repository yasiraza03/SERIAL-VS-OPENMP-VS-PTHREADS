#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
//#define MAX_SIZE 100 // (for 100 and 1000 elements)

// bubble sort
void bubbleSort(int arr[], int n) 
{
    int i, j;
    #pragma omp parallel for shared(arr, n) private(i, j)
    for (i = 0; i < n-1; i++) 
    {
        for (j = 0; j < n-i-1; j++) 
        {
            if (arr[j] > arr[j+1]) 
            {
                // Swap arr[j] and arr[j+1]
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// merge sort
void merge(int arr[], int left, int mid, int right) 
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) 
	{
        if (L[i] <= R[j]) 
		{
            arr[k] = L[i];
            i++;
        } 
		
		else 
		{
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[]
    while (i < n1) 
	{
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[]
    while (j < n2) 
	{
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

// Recursive function to perform merge sort
void mergeSort(int arr[], int left, int right) 
{
    if (left < right) 
	{
        int mid = left + (right - left) / 2;

        // Sort first and second halves in parallel
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergeSort(arr, left, mid);
            }
            #pragma omp section
            {
                mergeSort(arr, mid + 1, right);
            }
        }

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

// quick sort
int partition(int arr[], int low, int high) 
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) 
	{
        if (arr[j] < pivot) 
		{
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

// Recursive function to perform quick sort
void quickSort(int arr[], int low, int high) 
{
    if (low < high) 
	{
        int pi = partition(arr, low, high);

        // Sort the partitions in parallel
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                quickSort(arr, low, pi - 1);
            }
            #pragma omp section
            {
                quickSort(arr, pi + 1, high);
            }
        }
    }
}

int main() 
{
	int i;
    clock_t start, end;
    float cpu_time_used;

    start = clock(); // start recording time for a sorting algorithm
    
//	for 100 & 1000 elements
//	int arr[MAX_SIZE];
//	for (i = 0; i < MAX_SIZE; i++) 
//	{
//        arr[i] = rand() % 1000 + 1; // Generate a random positive integer between 1 and 1000
//    }
    
    int arr[] = {64, 34, 25, 12, 22, 11, 90, 55, 78, 47}; // for 10 elements
    int n = sizeof(arr) / sizeof(arr[0]);

    bubbleSort(arr, n);
    // mergeSort(arr, 0, n - 1);
    // quickSort(arr, 0, n - 1);

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
