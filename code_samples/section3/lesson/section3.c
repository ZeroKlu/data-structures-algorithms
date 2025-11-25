#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;

Node* push_front(Node* head, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return head;  // allocation failed, return old head
    new_node->value = value;
    new_node->next = head;
    return new_node;
}

void print_list(Node* head) {
    Node* current = head;
    printf("\n");
    while (current != NULL) {
        printf("%d", current->value);
        current = current->next;
    }
}

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
