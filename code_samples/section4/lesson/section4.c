#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define STACK_MAX 1024
#define QUEUE_MAX 1024
#define PAREN_STACK_MAX 1024

typedef struct
{
    int data[STACK_MAX];
    int top; // index of next free slot
} IntStack;

void stack_init(IntStack *s)
{
    s->top = 0;
}

bool stack_push(IntStack *s, int value)
{
    if (s->top >= STACK_MAX)
    {
        return false; // overflow
    }
    s->data[s->top++] = value;
    return true;
}

bool stack_pop(IntStack *s, int *out)
{
    if (s->top == 0)
    {
        return false; // underflow
    }
    s->top--;
    *out = s->data[s->top];
    return true;
}

typedef struct
{
    int data[QUEUE_MAX];
    int head; // index of current front
    int tail; // index of next free slot
    int size;
} IntQueue;

void queue_init(IntQueue *q)
{
    q->head = 0;
    q->tail = 0;
    q->size = 0;
}

bool queue_enqueue(IntQueue *q, int value)
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

bool queue_dequeue(IntQueue *q, int *out)
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

bool is_matching(char open, char close)
{
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

bool is_valid_parentheses(const char *s)
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
            if (!is_matching(open, c))
                return false;
        }
    }
    return top == 0;
}

void bfs(int start, int adjacency[][QUEUE_MAX], int n, bool visited[])
{
    IntQueue q;
    queue_init(&q);

    // Optionally clear visited (if caller didn't)
    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
    }

    visited[start] = true;
    queue_enqueue(&q, start);

    int current;
    printf("BFS starting from %d:\n", start);

    while (queue_dequeue(&q, &current))
    {
        printf("  visiting %d\n", current);

        // Look at all possible neighbors of 'current'
        for (int neighbor = 0; neighbor < n; neighbor++)
        {
            // adjacency[current][neighbor] != 0 means there's an edge
            if (adjacency[current][neighbor] && !visited[neighbor])
            {
                visited[neighbor] = true;
                queue_enqueue(&q, neighbor);
            }
        }
    }
}

int main()
{
    printf("==== TESTING IntStack ====\n");
    IntStack stack;
    stack_init(&stack);

    printf("Pushing 10, 20, 30...\n");
    stack_push(&stack, 10);
    stack_push(&stack, 20);
    stack_push(&stack, 30);

    int val;
    printf("Popping values:\n");
    while (stack_pop(&stack, &val))
    {
        printf("  popped: %d\n", val);
    }

    printf("\n==== TESTING IntQueue ====\n");
    IntQueue q;
    queue_init(&q);

    printf("Enqueuing 1, 2, 3...\n");
    queue_enqueue(&q, 1);
    queue_enqueue(&q, 2);
    queue_enqueue(&q, 3);

    printf("Dequeuing values:\n");
    while (queue_dequeue(&q, &val))
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
               is_valid_parentheses(tests[i]) ? "valid" : "invalid");
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
