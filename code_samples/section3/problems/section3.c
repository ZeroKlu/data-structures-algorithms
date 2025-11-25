#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct ListNode {
    int val;
    struct ListNode* next;
} ListNode;

void print_list(ListNode* head) {
    ListNode* current = head;
    printf("\n");
    while (current != NULL) {
        printf("%d", current->val);
        current = current->next;
    }
}

// Insert at head: head pointer is updated via pointer-to-pointer
void insert_head(ListNode** head, int x) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->val = x;
    node->next = *head;
    *head = node;
}

// Insert after node p (if p is non-NULL)
void insert_after(ListNode* p, int x) {
    if (!p) return;
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->val = x;
    node->next = p->next;
    p->next = node;
}

// Delete node immediately after p (if it exists)
void delete_after(ListNode* p) {
    if (!p || !p->next) return;
    ListNode* doomed = p->next;
    p->next = doomed->next;
    free(doomed);
}

ListNode* reverse_list(ListNode* head) {
    ListNode* prev = NULL;
    ListNode* curr = head;

    while (curr != NULL) {
        ListNode* nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }
    return prev;
}

bool has_cycle(ListNode* head) {
    if (head == NULL) return false;
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

ListNode* middle_node(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

ListNode* merge_two_lists(ListNode* l1, ListNode* l2) {
    ListNode dummy;
    dummy.next = NULL;
    ListNode* tail = &dummy;

    ListNode* a = l1;
    ListNode* b = l2;
    while (a != NULL && b != NULL) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    if (a != NULL) tail->next = a;
    if (b != NULL) tail->next = b;

    return dummy.next;
}

ListNode* remove_nth_from_end(ListNode* head, int n) {
    ListNode dummy;
    dummy.val = 0;
    dummy.next = head;

    ListNode* fast = &dummy;
    ListNode* slow = &dummy;

    for (int i = 0; i <= n; i++) {
        if (fast == NULL) return head; // n too large
        fast = fast->next;
    }

    while (fast != NULL) {
        fast = fast->next;
        slow = slow->next;
    }

    ListNode* doomed = slow->next;
    if (doomed != NULL) {
        slow->next = doomed->next;
        free(doomed);
    }
    return dummy.next;
}

int main() {
    ListNode* head = NULL;
    insert_head(&head, 5);
    insert_head(&head, 4);
    insert_head(&head, 3);
    insert_head(&head, 2);
    insert_head(&head, 1);

    // Insert 7 at the end
    ListNode* tail = head;
    while (tail->next != NULL) {
        tail = tail->next;
    }

    insert_after(tail, 6);
    
    print_list(head);

    delete_after(tail);

    print_list(head);

    head = reverse_list(head);

    print_list(head);

    printf("\nHas cycle: %s", has_cycle(head) ? "true" : "false");

    ListNode* cycle = NULL;
    insert_head(&cycle, 4);
    insert_head(&cycle, 3);
    insert_head(&cycle, 2);
    insert_head(&cycle, 1);

    print_list(cycle);

    // Get pointers to the nodes we want
    ListNode* p = cycle->next->next;          // node 3
    ListNode* q = cycle->next->next->next;    // node 4

    // Create the cycle
    q->next = p; // 4→3

    printf("\nHas cycle: %s", has_cycle(cycle) ? "true" : "false");

    head = reverse_list(head);

    print_list(head);
    ListNode* mid = middle_node(head);

    if (mid != NULL) {
        printf("\nMiddle node value: %d", mid->val);
    }

    ListNode* more = NULL;
    insert_head(&more, 8);
    insert_head(&more, 7);
    insert_head(&more, 6);

    print_list(more);

    ListNode* merged = merge_two_lists(head, more);
    print_list(merged);

    // Remove 7
    merged = remove_nth_from_end(merged, 2);
    print_list(merged);

    return 0;
}
