#include <stdlib.h>
#include <stdio.h>

// Linked list node
typedef struct Node {
    // Node value
    int value;

    // Pointer to next node
    struct Node* next;
} Node;

// Pushes a new node to the front of the list
Node* push_front(Node* head, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return head;  // allocation failed, return old head
    new_node->value = value;
    new_node->next = head;
    return new_node;
}

// Prints a linked list
void print_list(Node* head) {
    Node* current = head;
    printf("\n");
    while (current != NULL) {
        printf("%d", current->value);
        current = current->next;
    }
}

// Reverses a linked list
Node* reverse_list(Node* head) {
    Node* prev = NULL;
    Node* current = head;
    while (current != NULL) {
        Node* next_node = current->next;
        current->next = prev;
        prev = current;
        current = next_node;
    }
    return prev;
}

// Prints a linked list recursively
void print_list_recursive(Node* head) {
    if (head == NULL) {
        return;  // base case: empty list
    }
    printf("%d", head->value);
    print_list_recursive(head->next);
}

int main() {
    Node* head = NULL;
    head = push_front(head, 3);
    head = push_front(head, 2);
    head = push_front(head, 1);
    print_list(head);
    head = reverse_list(head);
    print_list(head);
    printf("\n");
    print_list_recursive(head);
    return 0;
}
