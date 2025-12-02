// For brevity, this is a conceptual sketch using a growable string buffer.
// In real C code you would need robust string-building utilities.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct {
    char *data;
    size_t size;
    size_t capacity;
} StrBuf;

static void sb_init(StrBuf *sb) {
    sb->size = 0;
    sb->capacity = 64;
    sb->data = (char *)malloc(sb->capacity);
    sb->data[0] = '\0';
}

static void sb_append(StrBuf *sb, const char *s) {
    size_t len = strlen(s);
    if (sb->size + len + 1 > sb->capacity) {
        while (sb->size + len + 1 > sb->capacity) {
            sb->capacity *= 2;
        }
        sb->data = (char *)realloc(sb->data, sb->capacity);
    }
    memcpy(sb->data + sb->size, s, len + 1);
    sb->size += len;
}

static void serialize_dfs(TreeNode *root, StrBuf *sb) {
    if (!root) {
        sb_append(sb, "#,");
        return;
    }
    char buf[32];
    snprintf(buf, sizeof(buf), "%d,", root->val);
    sb_append(sb, buf);
    serialize_dfs(root->left, sb);
    serialize_dfs(root->right, sb);
}

char *serialize(TreeNode *root) {
    StrBuf sb;
    sb_init(&sb);
    serialize_dfs(root, &sb);
    return sb.data; // caller should free
}

// Deserialization helpers.
static char *next_token(char **strp) {
    if (*strp == NULL) return NULL;
    char *start = *strp;
    char *comma = strchr(start, ',');
    if (!comma) {
        *strp = NULL;
        return start;
    }
    *comma = '\0';
    *strp = comma + 1;
    return start;
}

static TreeNode *deserialize_dfs(char **strp) {
    char *tok = next_token(strp);
    if (!tok) return NULL;
    if (strcmp(tok, "#") == 0) {
        return NULL;
    }
    int val = atoi(tok);
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = deserialize_dfs(strp);
    node->right = deserialize_dfs(strp);
    return node;
}

TreeNode *deserialize(char *data) {
    char *cursor = data;
    return deserialize_dfs(&cursor);
}

/* ----------------------------
   Extra helpers for testing
   ---------------------------- */

static TreeNode *node(int v) {
    TreeNode *n = (TreeNode *)malloc(sizeof(TreeNode));
    n->val = v;
    n->left = n->right = NULL;
    return n;
}

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

static void print_tree_impl(TreeNode *root, int indent) {
    if (!root) return;
    const int STEP = 4;

    print_tree_impl(root->right, indent + STEP);

    for (int i = 0; i < indent; ++i) putchar(' ');
    printf("%d\n", root->val);

    print_tree_impl(root->left, indent + STEP);
}

static void print_tree(TreeNode *root) {
    printf("Tree (sideways, right is up):\n");
    print_tree_impl(root, 0);
    printf("\n");
}

/* One test helper: serialize -> deserialize -> serialize, compare */

static void roundtrip_test(const char *label, TreeNode *root) {
    printf("==== %s ====\n", label);
    print_tree(root);

    char *s1 = serialize(root);
    printf("Serialized: %s\n", s1);

    // deserialize modifies the buffer in-place, so work on a copy
    char *buf = my_strdup(s1);
    TreeNode *copy_root = deserialize(buf);

    printf("Deserialized tree:\n");
    print_tree(copy_root);

    char *s2 = serialize(copy_root);
    printf("Re-serialized: %s\n", s2);

    if (strcmp(s1, s2) == 0) {
        printf("Round-trip OK (strings match)\n");
    } else {
        printf("Round-trip MISMATCH!\n");
    }
    printf("\n");

    free(s1);
    free(s2);
    free(buf);
    free_tree(copy_root);
}

/* ----------------------------
   Main test harness
   ---------------------------- */

int main(void) {
    // 1. Empty tree
    TreeNode *empty = NULL;

    // 2. Single node
    TreeNode *single = node(42);

    // 3. Larger tree:
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

    roundtrip_test("Empty tree", empty);
    roundtrip_test("Single node tree", single);
    roundtrip_test("Larger example tree", root);

    free_tree(single);
    free_tree(root);
    // empty is NULL, nothing to free

    return 0;
}
