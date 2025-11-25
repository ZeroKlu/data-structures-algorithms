#include <iostream>

struct Node {
    int value;
    Node* next;
    Node(int v, Node* n = nullptr) : value(v), next(n) {}
};

Node* pushFront(Node* head, int value) {
    Node* newNode = new Node(value, head);
    return newNode;
}

void printList(Node* head) {
    Node* current = head;
    std::cout << std::endl;
    while (current != nullptr) {
        std::cout << current->value;
        current = current->next;
    }
}

Node* reverseList(Node* head) {
    Node* prev = nullptr;
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        current->next = prev;
        prev = current;
        current = nextNode;
    }
    return prev;
}

void printListRecursive(Node* head) {
    if (head == nullptr) return;
    std::cout << head->value;
    printListRecursive(head->next);
}

int main() {
    Node* head = nullptr;
    head = pushFront(head, 3);
    head = pushFront(head, 2);
    head = pushFront(head, 1);
    printList(head);
    head = reverseList(head);
    printList(head);
    std::cout << std::endl;
    printListRecursive(head);
    return 0;
}
