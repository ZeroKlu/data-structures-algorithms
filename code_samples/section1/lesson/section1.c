#include <stdio.h>
#include <stdlib.h>

// Ex 1 - Print all elements of an array
void ex1(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d\n", arr[i]);
    }
}

// Ex 2 - Print all pairs of elements of an array (nested loops)
void ex2(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            printf("%d, %d\n", arr[i], arr[j]);
        }
    }
}

// Ex 3 - Print the first and last elements of an array
void ex3(int arr[], int n) {
    if (n > 0) {
        printf("%d %d\n", arr[0], arr[n - 1]);
    }
}

// Ex 4 - Print all elements of an array twice
void ex4(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d\n", arr[i]);
    }
    for (int i = 0; i < n; i++) {
        printf("%d\n", arr[i]);
    }
}

// Ex 5 - Double all elements of an array
int* ex5(int arr[], int n) {
    int* newArr = (int*)malloc(sizeof(int) * n);
    if (!newArr) return NULL; // handle allocation failure

    for (int i = 0; i < n; i++) {
        newArr[i] = arr[i] * 2;
    }
    return newArr; // caller should free(newArr)
}

int main() {
    int arr[5] = {1, 2, 3, 4, 5};

    printf("Ex 1:\n");
    ex1(arr, 5);

    printf("\nEx 2:\n");
    ex2(arr, 5);

    printf("\nEx 3:\n");
    ex3(arr, 5);

    printf("\nEx 4:\n");
    ex4(arr, 5);

    printf("\nEx 5:\n");
    int* newArr = ex5(arr, 5);
    for (int i = 0; i < 5; i++) {
        printf("%d\n", newArr[i]);
    }
    free(newArr);

    return 0;
}
