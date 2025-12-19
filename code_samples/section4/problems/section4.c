#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Check if a string of parentheses is valid
bool isValid(const char *s) {
    int n = (int)strlen(s);
    char stack[10000];
    int top = -1;

    for (int i = 0; i < n; i++) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            stack[++top] = c;
        } else {
            if (top < 0) return false;
            char open = stack[top--];
            if ((c == ')' && open != '(') ||
                (c == ']' && open != '[') ||
                (c == '}' && open != '{')) {
                return false;
            }
        }
    }
    return top == -1;
}

// Define a stack of integers
typedef struct {
    // array of integers (stack)
    int data[10000];

    // index of next free slot
    int top;
} Stack;

// Initialize the stack
void stackInit(Stack *s) {
    s->top = -1;
}

// Check if the stack is empty
bool stackEmpty(Stack *s) {
    return s->top < 0;
}

// Add value to the stack
void stackPush(Stack *s, int x) {
    s->data[++s->top] = x;
}

// Remove value from the stack
int stackPop(Stack *s) {
    return s->data[s->top--];
}

int stackPeek(Stack *s) {
    return s->data[s->top];
}

// Define a queue of integers
typedef struct {
    // Optimize enqueue and dequeue by using two stacks
    // array of integers (queue-in)
    Stack in;

    // array of integers (queue-out)
    Stack out;
} MyQueue;

// Initialize the queue
void myQueueInit(MyQueue *q) {
    stackInit(&q->in);
    stackInit(&q->out);
}

// Move all elements from in to out
static void moveInToOut(MyQueue *q) {
    if (stackEmpty(&q->out)) {
        while (!stackEmpty(&q->in)) {
            stackPush(&q->out, stackPop(&q->in));
        }
    }
}

// Add value to the queue
void enqueue(MyQueue *q, int x) {
    stackPush(&q->in, x);
}

// Remove value from the queue
int dequeue(MyQueue *q) {
    moveInToOut(q);
    return stackPop(&q->out);
}

// Get the value at the front of the queue
int front(MyQueue *q) {
    moveInToOut(q);
    return stackPeek(&q->out);
}

// Check if the queue is empty
bool empty(MyQueue *q) {
    return stackEmpty(&q->in) && stackEmpty(&q->out);
}


/**
 * This function takes an array of integers and returns an array of the next greater element for each element in the input array.
 * The input array is represented by the pointer `nums`, and the length of the array is `n`.
 * The function uses two arrays: `ans` to store the next greater element for each element in the input array, and `stack` to store the elements of the input array.
 * The function uses a stack to keep track of the elements in the input array from left to right.
 * The function iterates over the input array from right to left, and for each element, it finds the next greater element in the stack.
 * If the stack is empty, the next greater element is -1.
 * The function returns the array of next greater elements.
 *
 * @param nums: A pointer to the input array of integers.
 * @param n: The length of the input array.
 * @return: A pointer to the array of next greater elements.
 */
int* nextGreaterElements(const int *nums, int n) {
    int *ans = (int*)malloc(sizeof(int) * n); // Allocate memory for the output array
    int *stack = (int*)malloc(sizeof(int) * n); // Allocate memory for the stack
    int top = -1; // Initialize the stack

    // Iterate over the input array from right to left
    for (int i = n - 1; i >= 0; i--) {
        int x = nums[i]; // Get the current element

        // Pop elements from the stack until the top of the stack is smaller than the current element
        while (top >= 0 && stack[top] <= x) {
            top--;
        }

        // Set the next greater element for the current element
        ans[i] = (top >= 0) ? stack[top] : -1;

        // Push the current element onto the stack
        stack[++top] = x;
    }

    free(stack); // Free the memory used by the stack
    return ans; // Return the array of next greater elements
}

/**
 * This function takes an array of daily temperatures and returns an array of the number of days the temperature will continue to rise after each day.
 * The input array is represented by the pointer `temps`, and the length of the array is `n`.
 * The function uses two arrays: `ans` to store the number of days the temperature will continue to rise after each day, and `stack` to store the indices of the elements in the input array.
 * The function uses a stack to keep track of the indices of the elements in the input array from left to right.
 * The function iterates over the input array from left to right, and for each element, it finds the number of days the temperature will continue to rise after that day.
 * The function returns the array of the number of days the temperature will continue to rise after each day.
 *
 * @param temps: A pointer to the input array of daily temperatures.
 * @param n: The length of the input array.
 * @return: A pointer to the array of the number of days the temperature will continue to rise after each day.
 */
int* dailyTemperatures(const int *temps, int n) {
    int *ans = (int*)calloc(n, sizeof(int)); // Allocate memory for the output array
    int *stack = (int*)malloc(sizeof(int) * n); // Allocate memory for the stack
    int top = -1; // Initialize the stack

    // Iterate over the input array from left to right
    for (int i = 0; i < n; i++) {
        while (top >= 0 && temps[i] > temps[stack[top]]) {
            int idx = stack[top--]; // Get the index of the top element of the stack
            ans[idx] = i - idx; // Set the number of days the temperature will continue to rise after the top element
        }
        stack[++top] = i; // Push the current index onto the stack
    }

    free(stack); // Free the memory used by the stack
    return ans; // Return the array of the number of days the temperature will continue to rise after each day
}

// A minimal tree node definition and BFS outline.
// In a real program you would implement a proper queue.
typedef struct TreeNode {
    // Node value
    int val;

    // Pointer to left child
    struct TreeNode *left;

    // Pointer to right child
    struct TreeNode *right;
} TreeNode;

// For brevity, this shows the BFS structure using an array queue.

/**
 * This function performs a level-order traversal of a binary tree and returns the result as a 2D array.
 * The input is the root of the binary tree, and the function returns a pointer to the 2D array of the level-order traversal.
 * The function also returns the size of the 2D array and the sizes of each column.
 *
 * @param root: A pointer to the root of the binary tree.
 * @param returnSize: A pointer to an integer to store the size of the 2D array.
 * @param returnColumnSizes: A pointer to an array of integers to store the sizes of each column.
 * @return: A pointer to the 2D array of the level-order traversal.
 */
int** levelOrder(TreeNode *root, int *returnSize, int **returnColumnSizes) {
    if (!root) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    TreeNode **queue = (TreeNode**)malloc(sizeof(TreeNode*) * 10000); // Allocate memory for the queue
    int head = 0, tail = 0; // Initialize the head and tail of the queue
    queue[tail++] = root; // Enqueue the root node

    int capacity = 1000; // Initialize the capacity of the result array
    int **result = (int**)malloc(sizeof(int*) * capacity); // Allocate memory for the result array
    int *colSizes = (int*)malloc(sizeof(int) * capacity); // Allocate memory for the column sizes array
    int levels = 0; // Initialize the number of levels

    while (head < tail) {
        int levelSize = tail - head; // Get the size of the current level
        int *level = (int*)malloc(sizeof(int) * levelSize); // Allocate memory for the current level
        for (int i = 0; i < levelSize; i++) {
            TreeNode *node = queue[head++]; // Dequeue a node from the queue
            level[i] = node->val; // Set the value of the current level
            if (node->left) queue[tail++] = node->left; // Enqueue the left child of the node
            if (node->right) queue[tail++] = node->right; // Enqueue the right child of the node
        }
        result[levels] = level; // Store the current level in the result array
        colSizes[levels] = levelSize; // Store the size of the current level in the column sizes array
        levels++; // Increment the number of levels
    }

    *returnSize = levels; // Store the number of levels in the returnSize variable
    *returnColumnSizes = colSizes; // Store the column sizes array in the returnColumnSizes variable
    free(queue); // Free the memory used by the queue
    return result; // Return the result array
}

// Returns array of length n - k + 1.
/**
 * This function takes an array of integers, the length of the array, and the size of the sliding window,
 * and returns an array of the maximum values of each window.
 * The input array is represented by the pointer `nums`, and the length of the array is `n`.
 * The size of the sliding window is `k`.
 * The function uses a deque to keep track of the indices of the elements in the window.
 * The function iterates over the input array and for each element, it updates the deque to only contain the indices of the elements in the window.
 * The function returns an array of the maximum values of each window.
 *
 * @param nums: A pointer to the input array of integers.
 * @param n: The length of the input array.
 * @param k: The size of the sliding window.
 * @return: A pointer to the array of the maximum values of each window.
 */
int* maxSlidingWindow(const int *nums, int n, int k) {
    if (n == 0 || k == 0) return NULL;

    int outLen = n - k + 1; // Calculate the length of the output array
    int *result = (int*)malloc(sizeof(int) * outLen); // Allocate memory for the output array
    int *dequeIdx = (int*)malloc(sizeof(int) * n); // Allocate memory for the deque
    int front = 0, back = 0; // Initialize the front and back of the deque

    int outPos = 0; // Initialize the output position
    for (int i = 0; i < n; i++) {
        // Remove indices out of this window
        while (front < back && dequeIdx[front] <= i - k) {
            front++;
        }
        // Remove smaller values from the back
        while (front < back && nums[dequeIdx[back - 1]] <= nums[i]) {
            back--;
        }
        dequeIdx[back++] = i; // Add the current index to the deque

        if (i >= k - 1) {
            result[outPos++] = nums[dequeIdx[front]]; // Store the maximum value of the current window in the output array
        }
    }

    free(dequeIdx); // Free the memory used by the deque
    return result; // Return the output array
}

int main() {

    printf("==== TEST isValid() ====\n");
    const char *tests[] = {
        "()",
        "([])",
        "{[()]}",
        "([)]",
        "((())",
        "abc(def[ghi]{jkl})",
        "",
        "{[}]",
        NULL
    };
    for (int i = 0; tests[i] != NULL; i++) {
        printf("Test \"%s\": %s\n",
               tests[i],
               isValid(tests[i]) ? "valid" : "invalid");
    }

    printf("\n==== TEST MyQueue (queue using two stacks) ====\n");
    MyQueue q;
    myQueueInit(&q);

    printf("Enqueue: 1, 2, 3\n");
    enqueue(&q, 1);
    enqueue(&q, 2);
    enqueue(&q, 3);

    printf("Front: %d\n", front(&q));
    printf("Dequeue: %d\n", dequeue(&q));
    printf("Dequeue: %d\n", dequeue(&q));
    printf("Is empty? %s\n", empty(&q) ? "true" : "false");
    printf("Dequeue: %d\n", dequeue(&q));
    printf("Is empty? %s\n", empty(&q) ? "true" : "false");

    printf("\n==== TEST nextGreaterElements() (monotonic stack) ====\n");
    int nums[] = {2, 1, 2, 4, 3};
    int n_nums = (int)(sizeof(nums) / sizeof(nums[0]));
    int *nge = nextGreaterElements(nums, n_nums);

    printf("nums: ");
    for (int i = 0; i < n_nums; i++) printf("%d ", nums[i]);
    printf("\nNGE : ");
    for (int i = 0; i < n_nums; i++) printf("%d ", nge[i]);
    printf("\n");
    free(nge);

    printf("\n==== TEST dailyTemperatures() ====\n");
    int temps[] = {73, 74, 75, 71, 69, 72, 76, 73};
    int n_temps = (int)(sizeof(temps) / sizeof(temps[0]));
    int *dt = dailyTemperatures(temps, n_temps);

    printf("temps: ");
    for (int i = 0; i < n_temps; i++) printf("%d ", temps[i]);
    printf("\ndays : ");
    for (int i = 0; i < n_temps; i++) printf("%d ", dt[i]);
    printf("\n");
    free(dt);

    printf("\n==== TEST levelOrder() BFS on a tree ====\n");

    // Build this tree:
    //
    //        1
    //       / \
    //      2   3
    //     /   / \
    //    4   5   6
    //
    TreeNode *node4 = malloc(sizeof(TreeNode)); node4->val = 4; node4->left = node4->right = NULL;
    TreeNode *node5 = malloc(sizeof(TreeNode)); node5->val = 5; node5->left = node5->right = NULL;
    TreeNode *node6 = malloc(sizeof(TreeNode)); node6->val = 6; node6->left = node6->right = NULL;
    TreeNode *node2 = malloc(sizeof(TreeNode)); node2->val = 2; node2->left = node4; node2->right = NULL;
    TreeNode *node3 = malloc(sizeof(TreeNode)); node3->val = 3; node3->left = node5; node3->right = node6;
    TreeNode *root = malloc(sizeof(TreeNode));  root->val  = 1; root->left  = node2; root->right = node3;

    int levelCount = 0;
    int *colSizes = NULL;
    int **levels = levelOrder(root, &levelCount, &colSizes);

    for (int i = 0; i < levelCount; i++) {
        printf("level %d: ", i);
        for (int j = 0; j < colSizes[i]; j++) {
            printf("%d ", levels[i][j]);
        }
        printf("\n");
        free(levels[i]); // free each level array
    }
    free(colSizes);
    free(levels);

    // free tree nodes
    free(node4);
    free(node5);
    free(node6);
    free(node2);
    free(node3);
    free(root);

    printf("\n==== TEST maxSlidingWindow() ====\n");
    int arr[] = {1, 3, -1, -3, 5, 3, 6, 7};
    int n_arr = (int)(sizeof(arr) / sizeof(arr[0]));
    int k = 3;
    int *msw = maxSlidingWindow(arr, n_arr, k);

    printf("nums: ");
    for (int i = 0; i < n_arr; i++) printf("%d ", arr[i]);
    printf("\nmax window size %d: ", k);
    for (int i = 0; i < n_arr - k + 1; i++) printf("%d ", msw[i]);
    printf("\n");
    free(msw);

    printf("\n==== ALL TESTS COMPLETE ====\n");

    return 0;
}
