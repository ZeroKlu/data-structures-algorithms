#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int v) : val(v), next(nullptr) {}
};

struct Cmp {
    bool operator()(ListNode *a, ListNode *b) const {
        return a->val > b->val;  // min-heap
    }
};

ListNode *mergeKLists(vector<ListNode *> &lists) {
    priority_queue<ListNode *, vector<ListNode *>, Cmp> pq;
    for (auto node : lists) {
        if (node) pq.push(node);
    }
    ListNode dummy(0);
    ListNode *tail = &dummy;

    while (!pq.empty()) {
        ListNode *node = pq.top();
        pq.pop();
        tail->next = node;
        tail = tail->next;
        if (node->next) {
            pq.push(node->next);
        }
    }
    return dummy.next;
}

// ===== utilities for testing =====

ListNode *buildList(const vector<int> &vals) {
    ListNode *head = nullptr;
    ListNode *tail = nullptr;
    for (int v : vals) {
        auto *node = new ListNode(v);
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

void printList(const string &label, ListNode *head) {
    cout << label;
    for (ListNode *p = head; p != nullptr; p = p->next) {
        cout << p->val << " ";
    }
    cout << "\n";
}

void freeList(ListNode *head) {
    while (head) {
        ListNode *next = head->next;
        delete head;
        head = next;
    }
}

// ===== test function =====

void test_mergeKLists() {
    // ---------- Test 1: three sorted lists ----------
    cout << "=== Test 1: three sorted lists ===\n";
    ListNode *l1 = buildList({1, 4, 7});
    ListNode *l2 = buildList({2, 5, 8});
    ListNode *l3 = buildList({3, 6, 9});
    vector<ListNode *> lists1 = {l1, l2, l3};

    printList("List 1: ", l1);
    printList("List 2: ", l2);
    printList("List 3: ", l3);

    ListNode *merged1 = mergeKLists(lists1);
    printList("Merged: ", merged1);
    cout << "(Expected: 1 2 3 4 5 6 7 8 9)\n\n";

    freeList(merged1); // merged list reuses original nodes

    // ---------- Test 2: mixture of empty and non-empty ----------
    cout << "=== Test 2: empty + non-empty ===\n";
    ListNode *a = buildList({1, 3, 5});
    ListNode *b = nullptr;
    ListNode *c = buildList({2, 4, 6, 8});
    ListNode *d = nullptr;

    vector<ListNode *> lists2 = {a, b, c, d};

    printList("List A: ", a);
    printList("List B: ", b);
    printList("List C: ", c);
    printList("List D: ", d);

    ListNode *merged2 = mergeKLists(lists2);
    printList("Merged: ", merged2);
    cout << "(Expected: 1 2 3 4 5 6 8)\n\n";

    freeList(merged2);

    // ---------- Test 3: all lists empty ----------
    cout << "=== Test 3: all empty ===\n";
    vector<ListNode *> lists3 = {nullptr, nullptr, nullptr};

    ListNode *merged3 = mergeKLists(lists3);
    printList("Merged: ", merged3);
    cout << "(Expected: <empty>)\n\n";

    // nothing to free since merged3 is null

    // ---------- Test 4: single list ----------
    cout << "=== Test 4: single list ===\n";
    ListNode *single = buildList({10, 20, 30});
    vector<ListNode *> lists4 = {single};

    printList("Input:  ", single);
    ListNode *merged4 = mergeKLists(lists4);
    printList("Merged: ", merged4);
    cout << "(Expected: 10 20 30)\n\n";

    freeList(merged4);

    // ---------- Test 5: no lists at all ----------
    cout << "=== Test 5: zero lists ===\n";
    vector<ListNode *> lists5;

    ListNode *merged5 = mergeKLists(lists5);
    printList("Merged: ", merged5);
    cout << "(Expected: <empty>)\n\n";
}

// ===== main =====

int main() {
    test_mergeKLists();
    return 0;
}
