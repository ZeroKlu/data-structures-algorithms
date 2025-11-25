#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

typedef struct {
    int data[10000];
    int top;
} Stack;

void stackInit(Stack *s) { s->top = -1; }
bool stackEmpty(Stack *s) { return s->top < 0; }
void stackPush(Stack *s, int x) { s->data[++s->top] = x; }
int stackPop(Stack *s) { return s->data[s->top--]; }
int stackPeek(Stack *s) { return s->data[s->top]; }

typedef struct {
    Stack in;
    Stack out;
} MyQueue;

void myQueueInit(MyQueue *q) {
    stackInit(&q->in);
    stackInit(&q->out);
}

static void moveInToOut(MyQueue *q) {
    if (stackEmpty(&q->out)) {
        while (!stackEmpty(&q->in)) {
            stackPush(&q->out, stackPop(&q->in));
        }
    }
}

void enqueue(MyQueue *q, int x) {
    stackPush(&q->in, x);
}

int dequeue(MyQueue *q) {
    moveInToOut(q);
    return stackPop(&q->out);
}

int front(MyQueue *q) {
    moveInToOut(q);
    return stackPeek(&q->out);
}

bool empty(MyQueue *q) {
    return stackEmpty(&q->in) && stackEmpty(&q->out);
}

int* nextGreaterElements(const int *nums, int n) {
    int *ans = (int*)malloc(sizeof(int) * n);
    int *stack = (int*)malloc(sizeof(int) * n);
    int top = -1;

    for (int i = n - 1; i >= 0; i--) {
        int x = nums[i];
        while (top >= 0 && stack[top] <= x) {
            top--;
        }
        ans[i] = (top >= 0) ? stack[top] : -1;
        stack[++top] = x;
    }

    free(stack);
    return ans;
}

int* dailyTemperatures(const int *temps, int n) {
    int *ans = (int*)calloc(n, sizeof(int));
    int *stack = (int*)malloc(sizeof(int) * n);
    int top = -1;

    for (int i = 0; i < n; i++) {
        while (top >= 0 && temps[i] > temps[stack[top]]) {
            int idx = stack[top--];
            ans[idx] = i - idx;
        }
        stack[++top] = i;
    }

    free(stack);
    return ans;
}

// A minimal tree node definition and BFS outline.
// In a real program you would implement a proper queue.
typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// For brevity, this shows the BFS structure using an array queue.
#include <stdlib.h>

int** levelOrder(TreeNode *root, int *returnSize, int **returnColumnSizes) {
    if (!root) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    TreeNode **queue = (TreeNode**)malloc(sizeof(TreeNode*) * 10000);
    int head = 0, tail = 0;
    queue[tail++] = root;

    int capacity = 1000;
    int **result = (int**)malloc(sizeof(int*) * capacity);
    int *colSizes = (int*)malloc(sizeof(int) * capacity);
    int levels = 0;

    while (head < tail) {
        int levelSize = tail - head;
        int *level = (int*)malloc(sizeof(int) * levelSize);
        for (int i = 0; i < levelSize; i++) {
            TreeNode *node = queue[head++];
            level[i] = node->val;
            if (node->left) queue[tail++] = node->left;
            if (node->right) queue[tail++] = node->right;
        }
        result[levels] = level;
        colSizes[levels] = levelSize;
        levels++;
    }

    *returnSize = levels;
    *returnColumnSizes = colSizes;
    free(queue);
    return result;
}

// Returns array of length n - k + 1.
int* maxSlidingWindow(const int *nums, int n, int k) {
    if (n == 0 || k == 0) return NULL;

    int outLen = n - k + 1;
    int *result = (int*)malloc(sizeof(int) * outLen);
    int *dequeIdx = (int*)malloc(sizeof(int) * n);
    int front = 0, back = 0; // [front, back)

    int outPos = 0;
    for (int i = 0; i < n; i++) {
        // Remove indices out of this window
        while (front < back && dequeIdx[front] <= i - k) {
            front++;
        }
        // Remove smaller values from the back
        while (front < back && nums[dequeIdx[back - 1]] <= nums[i]) {
            back--;
        }
        dequeIdx[back++] = i;

        if (i >= k - 1) {
            result[outPos++] = nums[dequeIdx[front]];
        }
    }

    free(dequeIdx);
    return result;
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
