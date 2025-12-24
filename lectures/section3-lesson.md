# Section 3 — Linked Lists & Recursion

Linked lists and recursion show up constantly in interviews because they test whether you can reason about **pointers/references**, **invariants**, and **stack behavior**. Arrays are about indices; linked lists are about **wiring**.

---

## Learning goals

By the end of this section, you should be able to:

- Explain what a linked list node is and how nodes connect in memory
- Compare linked lists vs arrays and choose the right structure for a task
- Implement basic list operations: insert at head, traverse, reverse
- Explain recursion using base cases + progress
- Reason about time and space complexity, including recursion stack space

---

## 3.1 What is a linked list?

A **linked list** is a chain of nodes. Each node stores:

- A **value**
- A **reference/pointer** to the next node (and sometimes the previous node)

Key vocabulary:

- **Head**: the first node
- **Tail**: the last node (its `next` is null)
- **Singly linked list**: node points only forward
- **Doubly linked list**: node points forward and backward

### Big idea

Linked lists trade away fast indexing for structural flexibility:

- Arrays: **O(1)** access by index (random access)
- Linked lists: **O(k)** to reach the k-th element (must walk from the head)

---

## 3.2 Complexity recap (linked lists)

Important operations and their costs:

- **Traversal** over all nodes: **O(n)**
- **Insert or delete after a given node** (when you already have it): **O(1)**
- **Find by index**: **O(k)**, worst case **O(n)**

### Design tips

- Maintaining a **tail pointer** allows O(1) insertion at the end
- Using a **dummy (sentinel) head** simplifies edge cases
- Doubly linked lists make deletion easier when you already have a node pointer

---

## 3.3 Recursion and the call stack

A **recursive function** solves a problem by calling itself on a smaller subproblem.

To avoid infinite recursion, you need:

- A **base case** that stops recursion
- A **step** that moves toward the base case

### Call stack cost

For a linked list of length `n`:

- One recursive call per node
- `n` stack frames created before unwinding

So recursive list algorithms usually have:

- **Time:** O(n)
- **Extra space:** O(n) (call stack)

This is why many problems have both iterative and recursive solutions.

---

# Code Examples — What Each One Teaches

## 3.5 Example 1 — Insert at head

**Pattern:** constant-time pointer rewiring.

Steps:

1. Allocate a new node
2. Store the value
3. Point `new.next` at the old head
4. Return the new node as the new head

### Why it’s O(1)

No matter how long the list is, we do a fixed number of pointer assignments.

**Time:** O(1)  
**Extra space:** O(1) per insertion (one new node)

---

## 3.6 Example 2 — Traverse and print

**Pattern:** walk until null.

Algorithm:

- Start at `head`
- While `current != null`:
  - Use the value
  - Move to `current.next`

### Why it’s O(n)

Each node is visited exactly once.

**Time:** O(n)  
**Extra space:** O(1)

---

## 3.7 Example 3 — Reverse a linked list (iterative)

A classic interview problem.

**Core idea:** reverse links one at a time without losing access to the rest of the list.

### Three-pointer pattern

- `prev` → reversed prefix
- `current` → node being processed
- `next` → saved pointer to the remaining list

At each step:

1. Save `next = current.next`
2. Reverse pointer: `current.next = prev`
3. Move `prev` forward
4. Move `current` forward

When `current` becomes null, `prev` is the new head.

**Time:** O(n)  
**Extra space:** O(1)

---

## 3.8 Example 4 — Recursive traversal

**Pattern:** process current node, then recurse on the rest.

Base case:

- If the node is null, return

Recursive step:

- Print the value
- Recurse on `head.next`

### Trade-off

Recursion mirrors the structure of the list, but:

- **Time:** O(n)
- **Extra space:** O(n) (call stack)

---

## 3.9 Quick Practice — Reasoning

1) **Count nodes iteratively**  
Walk the list once.  
Time: **O(n)**, Extra space: **O(1)**

2) **Count nodes recursively**  
One recursive call per node.  
Time: **O(n)**, Extra space: **O(n)**

3) **Print list in reverse order recursively**  
Recurse first, print on the way back.  
Time: **O(n)**, Extra space: **O(n)**

4) **Reverse list recursively vs iteratively**  
Both are O(n) time.  
Recursive uses **O(n)** stack space; iterative uses **O(1)** extra space.

5) **Insert at tail with only a head pointer**  
Must traverse to find the last node.  
Time: **O(n)** per insertion, Extra space: **O(1)**

---

## Interview mindset

When solving linked list problems, interviewers are looking for:

- Careful pointer handling
- Clear invariants (“what stays true after each step”)
- Correct time and space analysis
- Awareness of recursion stack costs

A strong habit is to state invariants out loud, for example:

> “`prev` points to the reversed prefix, and `current` points to the remaining suffix.”

---

## Common pitfalls

- Losing a pointer before rewiring (`current.next`)
- Forgetting the base case in recursion
- Assuming linked lists allow random access
- Ignoring recursion stack space in complexity analysis
