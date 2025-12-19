#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

/**
 * Creates a new binary tree node with the given value
 * @param value The value to set on the new node
 * @return A pointer to the newly created node
 */
TreeNode* create_node(int value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) return NULL;

    node->value = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/**
 * Prints the values of the nodes in the binary tree in preorder traversal order.
 * @param root The root of the binary tree to traverse.
 */
void preorder(TreeNode* root) {
    if (root == NULL) return;

    printf("%d\n", root->value);

    // Recursively traverse the left subtree
    preorder(root->left);

    // Recursively traverse the right subtree
    preorder(root->right);
}

// Define a binary tree node queue
typedef struct {
    // Array to store the nodes
    TreeNode* data[1024];

    // Pointer to head of the queue
    int head;

    // Pointer to tail of the queue
    int tail;
} NodeQueue;

/**
 * Initializes a NodeQueue object.
 * @param q The NodeQueue object to initialize.
 */
void queue_init(NodeQueue* q) {
    // Initialize the head and tail of the queue to the start of the data array
    q->head = 0;
    q->tail = 0;
}

/**
 * Checks if a NodeQueue is empty.
 * @param q The NodeQueue object to check.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int queue_is_empty(NodeQueue* q) {
    return q->head == q->tail;
}

/**
 * Adds a node to the end of the queue.
 * @param q The NodeQueue object to modify.
 * @param node The node to add to the end of the queue.
 */
void queue_push(NodeQueue* q, TreeNode* node) {
    q->data[q->tail++] = node;
}

/**
 * Removes a node from the front of the queue.
 * @param q The NodeQueue object to modify.
 * @return The node at the front of the queue, or NULL if the queue is empty.
 */
TreeNode* queue_pop(NodeQueue* q) {
    return q->data[q->head++];
}

/**
 * Prints the values of the nodes in the binary tree in level order traversal order.
 * A level order traversal visits all the nodes at each level from left to right before moving on to the next level.
 * @param root The root of the binary tree to traverse.
 */
void level_order(TreeNode* root) {
    if (root == NULL) return;
    NodeQueue q;
    queue_init(&q);
    queue_push(&q, root);

    while (!queue_is_empty(&q)) {
        TreeNode* node = queue_pop(&q);
        printf("%d\n", node->value);
        if (node->left) queue_push(&q, node->left);
        if (node->right) queue_push(&q, node->right);
    }
}

/**
 * Recursively visits all the nodes in a graph that are reachable from a given node.
 *
 * This function visits all the nodes in a graph that are reachable from a given node.
 * It uses a depth-first search approach to traverse the graph, visiting a node and then
 * recursively visiting all of its neighbors.
 *
 * @param node The node to start the search from.
 * @param n The number of nodes in the graph.
 * @param adjacency A 2D array where adjacency[u][v] is 1 if there is an edge between
 * nodes u and v, and 0 otherwise.
 * @param visited An array where visited[u] is 1 if node u has been visited, and 0
 * otherwise.
 */
void dfs_util(int node, int n, int adjacency[][64], int visited[]) {
    visited[node] = 1;
    printf("%d\n", node);
    for (int neighbor = 0; neighbor < n; neighbor++) {
        if (adjacency[node][neighbor] && !visited[neighbor]) {
            dfs_util(neighbor, n, adjacency, visited);
        }
    }
}

/**
 * Recursively visits all the nodes in a graph that are reachable from a given node.
 * This function visits all the nodes in a graph that are reachable from a given node.
 * It uses a depth-first search approach to traverse the graph, visiting a node and then
 * recursively visiting all of its neighbors.
 *
 * @param start The node to start the search from.
 * @param n The number of nodes in the graph.
 * @param adjacency A 2D array where adjacency[u][v] is 1 if there is an edge between
 * nodes u and v, and 0 otherwise.
 */
void dfs(int start, int n, int adjacency[][64]) {
    int visited[64] = {0};
    dfs_util(start, n, adjacency, visited);
}

/**
 * Computes the shortest distances from a given node to all other nodes in a graph.
 *
 * This function uses a breadth-first search approach to traverse the graph, visiting
 * all nodes reachable from the start node and computing the shortest distance from
 * the start node to each of those nodes.
 *
 * @param start The node to start the search from.
 * @param n The number of nodes in the graph.
 * @param adjacency A 2D array where adjacency[u][v] is 1 if there is an edge between
 * nodes u and v, and 0 otherwise.
 * @param dist An array where dist[u] is the shortest distance from the start node to node u.
 */
void shortest_distances(int start, int n, int adjacency[][64], int dist[]) {
    for (int i = 0; i < n; i++) {
        dist[i] = -1; // -1 means unreachable
    }

    int queue[64];
    int head = 0;
    int tail = 0;

    dist[start] = 0;
    queue[tail++] = start;

    while (head < tail) {
        int node = queue[head++];
        for (int neighbor = 0; neighbor < n; neighbor++) {
            if (adjacency[node][neighbor] && dist[neighbor] == -1) {
                dist[neighbor] = dist[node] + 1;
                queue[tail++] = neighbor;
            }
        }
    }
}

int main() {

    /* ==== TEST BINARY TREE CREATION ==== */
    printf("==== TEST create_node + preorder ====\n");

    // Build this tree:
    //
    //         1
    //       /   \
    //      2     3
    //     / \   /
    //    4   5 6
    //
    TreeNode* n4 = create_node(4);
    TreeNode* n5 = create_node(5);
    TreeNode* n6 = create_node(6);
    TreeNode* n2 = create_node(2);
    TreeNode* n3 = create_node(3);
    TreeNode* root = create_node(1);

    n2->left = n4;
    n2->right = n5;
    n3->left = n6;
    root->left = n2;
    root->right = n3;

    printf("Preorder traversal (should be 1 2 4 5 3 6):\n");
    preorder(root);

    printf("\n==== TEST level_order (tree BFS) ====\n");
    printf("Level order traversal (should be 1 2 3 4 5 6):\n");
    level_order(root);


    /* ==== TEST GRAPH DFS ====
       Graph:
           0 -- 1
           |  /
           2
           |
           3
       Adjacency matrix (undirected)
    */
    printf("\n==== TEST dfs (graph DFS) ====\n");

    int adjacency[64][64] = {0};
    int n = 4;

    // Undirected edges
    adjacency[0][1] = adjacency[1][0] = 1;
    adjacency[0][2] = adjacency[2][0] = 1;
    adjacency[1][2] = adjacency[2][1] = 1;
    adjacency[2][3] = adjacency[3][2] = 1;

    printf("DFS starting at node 0 (expect e.g., 0 1 2 3):\n");
    dfs(0, n, adjacency);


    /* ==== TEST BFS shortest distances ==== */
    printf("\n==== TEST shortest_distances (graph BFS shortest path) ====\n");

    int dist[64];
    shortest_distances(0, n, adjacency, dist);

    printf("Shortest distances from node 0:\n");
    for (int i = 0; i < n; i++) {
        printf("dist to %d = %d\n", i, dist[i]);
    }
    // Expected:
    // dist[0] = 0
    // dist[1] = 1
    // dist[2] = 1
    // dist[3] = 2


    /* ==== FREE TREE MEMORY ==== */
    free(n4);
    free(n5);
    free(n6);
    free(n2);
    free(n3);
    free(root);

    printf("\n==== ALL TESTS COMPLETE ====\n");
    return 0;
}
