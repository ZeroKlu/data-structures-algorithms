#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define STACK_MAX 1024
#define QUEUE_MAX 1024
#define PAREN_STACK_MAX 1024

// Defines a stack of integers
typedef struct
{
    // array of integers (stack)
    int data[STACK_MAX];
    int top; // index of next free slot
} IntStack;

// Initialize the stack
void stackInit(IntStack *s)
{
    s->top = 0;
}

// Add value to the stack
bool stackPush(IntStack *s, int value)
{
    if (s->top >= STACK_MAX)
    {
        return false; // overflow
    }
    s->data[s->top++] = value;
    return true;
}

// Remove value from the stack
bool stackPop(IntStack *s, int *out)
{
    if (s->top == 0)
    {
        return false; // underflow
    }
    s->top--;
    *out = s->data[s->top];
    return true;
}

// Defines a queue of integers
typedef struct
{
    // array of integers (queue)
    int data[QUEUE_MAX];
    int head; // index of current front
    int tail; // index of next free slot
    int size;
} IntQueue;

// Initialize the queue
void queueInit(IntQueue *q)
{
    q->head = 0;
    q->tail = 0;
    q->size = 0;
}

// Add value to the queue
bool queueEnqueue(IntQueue *q, int value)
{
    if (q->size == QUEUE_MAX)
    {
        return false; // full
    }
    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % QUEUE_MAX;
    q->size++;
    return true;
}

// Remove value from the queue
bool queueDequeue(IntQueue *q, int *out)
{
    if (q->size == 0)
    {
        return false; // empty
    }
    *out = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_MAX;
    q->size--;
    return true;
}

// Check if two parentheses are matching
bool isMatching(char open, char close)
{
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

// Check if a string of parentheses is valid
bool isValidParentheses(const char *s)
{
    char stack[PAREN_STACK_MAX];
    int top = 0;

    for (int i = 0; s[i] != '\0'; i++)
    {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{')
        {
            if (top >= PAREN_STACK_MAX)
                return false;
            stack[top++] = c;
        }
        else if (c == ')' || c == ']' || c == '}')
        {
            if (top == 0)
                return false;
            char open = stack[--top];
            if (!isMatching(open, c))
                return false;
        }
    }
    return top == 0;
}

/**
 * This function implements the Breadth-first search algorithm.
 * It starts from a given node and explores all the connected nodes in a graph.
 * The graph is represented by an adjacency matrix.
 *
 * @param start: The starting node for the BFS traversal.
 * @param adjacency: The adjacency matrix representing the graph.
 * @param n: The number of nodes in the graph.
 * @param visited: An array to keep track of visited nodes.
 */
void bfs(int start, int adjacency[][QUEUE_MAX], int n, bool visited[])
{
    // Initialize a queue for BFS traversal
    IntQueue q;
    queueInit(&q);

    // Clear the visited array if it wasn't done by the caller
    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
    }

    // Mark the start node as visited and enqueue it
    visited[start] = true;
    queueEnqueue(&q, start);

    // Print the BFS traversal starting from the start node
    printf("BFS starting from %d:\n", start);

    int current;
    while (queueDequeue(&q, &current))
    {
        // Log the current node as visited and print it
        printf("  visiting %d\n", current);

        // Look at all possible neighbors of 'current'
        for (int neighbor = 0; neighbor < n; neighbor++)
        {
            // If there's an edge from current to neighbor and neighbor is not visited
            if (adjacency[current][neighbor] && !visited[neighbor])
            {
                // Mark the neighbor as visited and enqueue it
                visited[neighbor] = true;
                queueEnqueue(&q, neighbor);
            }
        }
    }
}

int main()
{
    printf("==== TESTING IntStack ====\n");
    IntStack stack;
    stackInit(&stack);

    printf("Pushing 10, 20, 30...\n");
    stackPush(&stack, 10);
    stackPush(&stack, 20);
    stackPush(&stack, 30);

    int val;
    printf("Popping values:\n");
    while (stackPop(&stack, &val))
    {
        printf("  popped: %d\n", val);
    }

    printf("\n==== TESTING IntQueue ====\n");
    IntQueue q;
    queueInit(&q);

    printf("Enqueuing 1, 2, 3...\n");
    queueEnqueue(&q, 1);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 3);

    printf("Dequeuing values:\n");
    while (queueDequeue(&q, &val))
    {
        printf("  dequeued: %d\n", val);
    }

    printf("\n==== TESTING is_valid_parentheses ====\n");

    const char *tests[] = {
        "()",
        "([])",
        "{[()]}",
        "([)]",
        "((())",
        "abc(def[ghi]{jkl})",
        "",
        "{[}]",
        NULL};

    for (int i = 0; tests[i] != NULL; i++)
    {
        printf("Test \"%s\": %s\n",
               tests[i],
               isValidParentheses(tests[i]) ? "valid" : "invalid");
    }

    printf("\n==== TESTING bfs ====\n");

    // Graph:
    // 0 -- 1
    // |  /
    // 2
    int n = 3;
    int adjacency[3][QUEUE_MAX] = {0};
    bool visited[3];

    // Undirected edges: (0-1), (0-2), (1-2)
    adjacency[0][1] = 1;
    adjacency[1][0] = 1;
    adjacency[0][2] = 1;
    adjacency[2][0] = 1;
    adjacency[1][2] = 1;
    adjacency[2][1] = 1;

    bfs(0, adjacency, n, visited);

    printf("Visited array after BFS:\n");
    for (int i = 0; i < n; i++)
    {
        printf("  Node %d visited = %s\n", i, visited[i] ? "true" : "false");
    }

    printf("\n==== ALL TESTS COMPLETE ====\n");

    return 0;
}
