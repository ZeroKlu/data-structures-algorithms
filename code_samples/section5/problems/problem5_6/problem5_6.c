// For brevity, this is a conceptual sketch using a growable string buffer.
// In real C code you would need robust string-building utilities.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ==========================
// BINARY TREE NODE DEFINITION
// ==========================

// A basic binary tree node
typedef struct TreeNode {
    int val;                 // value stored in this node
    struct TreeNode *left;   // left child (NULL if none)
    struct TreeNode *right;  // right child (NULL if none)
} TreeNode;

// ==========================
// SIMPLE STRING BUILDER (StrBuf)
// ==========================

// A minimal growable string buffer:
// - data: pointer to a heap-allocated char array
// - size: current length of the string (not including '\0')
// - capacity: allocated size of data buffer
typedef struct {
    char *data;
    size_t size;
    size_t capacity;
} StrBuf;

// Initialize buffer with a small starting capacity and empty string.
static void sb_init(StrBuf *sb) {
    sb->size = 0;
    sb->capacity = 64;
    sb->data = (char *)malloc(sb->capacity);

    // Ensure the string starts as empty
    sb->data[0] = '\0';
}

// Append a C-string s to the buffer, growing capacity as needed.
static void sb_append(StrBuf *sb, const char *s) {
    size_t len = strlen(s);

    // Ensure we have room for: existing content + new content + '\0'
    if (sb->size + len + 1 > sb->capacity) {
        // Keep doubling until large enough
        while (sb->size + len + 1 > sb->capacity) {
            sb->capacity *= 2;
        }
        sb->data = (char *)realloc(sb->data, sb->capacity);
    }

    // Copy including the terminating '\0'
    // We copy len + 1 so the resulting buffer remains a valid C string.
    memcpy(sb->data + sb->size, s, len + 1);

    // Update size (exclude the '\0')
    sb->size += len;
}

// ==========================
// SERIALIZATION (PREORDER + NULL MARKERS)
// ==========================

// Serialize using preorder DFS:
//
//   node value, then left subtree, then right subtree
//
// Use "#," to represent a NULL pointer.
// Example (single node 1):
//   "1,#,#,"
static void serialize_dfs(TreeNode *root, StrBuf *sb) {
    // If subtree is empty, write a null marker
    if (!root) {
        sb_append(sb, "#,");
        return;
    }

    // Write this node's value followed by comma
    char buf[32];
    snprintf(buf, sizeof(buf), "%d,", root->val);
    sb_append(sb, buf);

    // Recurse left and right
    serialize_dfs(root->left, sb);
    serialize_dfs(root->right, sb);
}

// Public serialize function:
// - returns a heap-allocated string containing the serialization
// - caller must free the returned char*
char *serialize(TreeNode *root) {
    StrBuf sb;
    sb_init(&sb);

    // Fill buffer with preorder serialization
    serialize_dfs(root, &sb);

    return sb.data; // caller should free()
}

// ==========================
// DESERIALIZATION
// ==========================
//
// We parse the comma-separated tokens back into a tree,
// consuming tokens in preorder. This exactly mirrors serialization.
//
// NOTE: next_token() modifies the input string in place by inserting '\0'
// at commas. That is why the test harness makes a copy before calling
// deserialize().

// Return next token from a comma-separated string.
// - *strp is advanced past the comma
// - commas are replaced with '\0' so the token becomes a standalone C-string
static char *next_token(char **strp) {
    // If cursor is NULL, no tokens remain
    if (*strp == NULL) return NULL;

    char *start = *strp;
    char *comma = strchr(start, ',');

    // If no comma found, treat the rest as one token
    if (!comma) {
        *strp = NULL;
        return start;
    }

    // Terminate the token at the comma
    *comma = '\0';

    // Advance cursor to character after the comma
    *strp = comma + 1;

    return start;
}

// Recursive DFS rebuild:
// - read next token
// - if token is "#", return NULL
// - else create node, recursively build left then right
static TreeNode *deserialize_dfs(char **strp) {
    // Get next token
    char *tok = next_token(strp);
    if (!tok) return NULL; // no more input

    // "#" represents null pointer
    if (strcmp(tok, "#") == 0) {
        return NULL;
    }

    // Otherwise, parse the integer value
    int val = atoi(tok);

    // Create a new node
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->val = val;

    // Recurse in the same order used by serialization
    node->left = deserialize_dfs(strp);
    node->right = deserialize_dfs(strp);

    return node;
}

// Public deserialize function:
// - takes a mutable string buffer (because we modify it in-place)
// - returns the rebuilt tree root
TreeNode *deserialize(char *data) {
    char *cursor = data;            // current parse position
    return deserialize_dfs(&cursor);
}

/* ----------------------------
   Extra helpers for testing
   ---------------------------- */

// Convenience: allocate a new node
static TreeNode *node(int v) {
    TreeNode *n = (TreeNode *)malloc(sizeof(TreeNode));
    n->val = v;
    n->left = n->right = NULL;
    return n;
}

// Free an entire tree using postorder cleanup
static void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

// Simple strdup (portable, in case strdup isn't available)
static char *my_strdup(const char *s) {
    size_t len = strlen(s);
    char *copy = (char *)malloc(len + 1);
    if (!copy) return NULL;
    memcpy(copy, s, len + 1);
    return copy;
}

/* Sideways tree printer */

// Print the tree sideways:
// - right subtree appears above
// - left subtree appears below
static void print_tree_impl(TreeNode *root, int indent) {
    if (!root) return;
    const int STEP = 4;

    // Print right subtree first
    print_tree_impl(root->right, indent + STEP);

    // Print current node with indentation
    for (int i = 0; i < indent; ++i) putchar(' ');
    printf("%d\n", root->val);

    // Print left subtree last
    print_tree_impl(root->left, indent + STEP);
}

// Public wrapper for printing
static void print_tree(TreeNode *root) {
    printf("Tree (sideways, right is up):\n");
    print_tree_impl(root, 0);
    printf("\n");
}

/* One test helper: serialize -> deserialize -> serialize, compare */

// Runs a round-trip test:
// 1) serialize original
// 2) deserialize into a new tree
// 3) serialize the new tree
// 4) compare the two strings (should match)
static void roundtrip_test(const char *label, TreeNode *root) {
    printf("==== %s ====\n", label);

    // Print original tree
    print_tree(root);

    // Serialize original
    char *s1 = serialize(root);
    printf("Serialized: %s\n", s1);

    // IMPORTANT: deserialize modifies the buffer in-place (replaces commas with '\0')
    // so we make a copy of the serialized string before deserializing.
    char *buf = my_strdup(s1);
    TreeNode *copy_root = deserialize(buf);

    // Print deserialized tree
    printf("Deserialized tree:\n");
    print_tree(copy_root);

    // Serialize again
    char *s2 = serialize(copy_root);
    printf("Re-serialized: %s\n", s2);

    // Compare strings to confirm the round trip preserved structure
    if (strcmp(s1, s2) == 0) {
        printf("Round-trip OK (strings match)\n");
    } else {
        printf("Round-trip MISMATCH!\n");
    }
    printf("\n");

    // Cleanup allocations made during the test
    free(s1);
    free(s2);
    free(buf);
    free_tree(copy_root);
}

/* ----------------------------
   Main test harness
   ---------------------------- */

int main(void) {
    // 1) Empty tree
    TreeNode *empty = NULL;

    // 2) Single node tree
    TreeNode *single = node(42);

    // 3) Larger tree:
    //
    //         1
    //       /   \
    //      2     3
    //     / \   /
    //    4   5 6
    TreeNode *root = node(1);
    root->left = node(2);
    root->right = node(3);
    root->left->left = node(4);
    root->left->right = node(5);
    root->right->left = node(6);

    // Run round-trip tests
    roundtrip_test("Empty tree", empty);
    roundtrip_test("Single node tree", single);
    roundtrip_test("Larger example tree", root);

    // Cleanup original trees
    free_tree(single);
    free_tree(root);
    // empty is NULL, nothing to free

    return 0;
}
