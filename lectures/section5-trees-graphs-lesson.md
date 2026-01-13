# Section 5 Lecture Script — Trees & Graphs (Lesson + Examples)

## 0 - Opening (30–60 seconds)

Today is about **trees and graphs** — the data structures that show up whenever you need to represent:
- **hierarchies** (folders, org charts, DOM trees, decision trees),
- **networks** (maps, social graphs, dependency graphs),
- and **search spaces** (game states, puzzles, shortest paths).

Most interview problems here reduce to:  
**“How do I traverse this structure efficiently and safely?”**  
Your two core tools are:
- **DFS** (depth-first search) — recursion or explicit stack
- **BFS** (breadth-first search) — queue

---

## 1 - Trees vs. Graphs (5.1)

### What is a tree?
A **tree** is a *special graph* with strong guarantees:
- **connected** (everything reachable from everything)
- **acyclic** (no cycles)
- with **n nodes, n − 1 edges**

In a **rooted tree**:
- one node is the **root**
- every other node has **exactly one parent**
- nodes can have **children** (binary trees have up to 2)

**Key interview advantage:**  
Trees are “easy mode graphs” because the no-cycle guarantee removes a lot of complexity.

### What is a graph?
A **graph** is more general:
- may be **directed** or **undirected**
- may be **weighted** or **unweighted**
- may be **cyclic** or **acyclic**
- may be **disconnected** (multiple components)

**Key interview danger:**  
Graphs can revisit nodes through cycles. That’s why **visited tracking** is a big deal.

---

## 2 - Traversal Patterns (5.2)

### Tree traversals (binary tree)
You usually see these:

- **Preorder:** node → left → right  
  Great when you want to process a node before its children.
- **Inorder:** left → node → right  
  In a BST, this yields **sorted order**.
- **Postorder:** left → right → node  
  Great when children must be handled before parent (cleanup, subtree computation).
- **Level-order:** BFS using a queue  
  Great when you care about **depth**, **layers**, or “closest” by edges.

### Graph traversals
- **DFS:** stack/recursion; explore deep then backtrack
- **BFS:** queue; explore in expanding layers

**Mental cue:**
- “layers”, “minimum steps”, “nearest” → **BFS**
- “explore”, “paths”, “backtracking”, “cycle detection”, “topological” → **DFS**

---

## 3 - Example 1 — Binary Tree Node + Preorder Traversal (5.4)

### What we’re building
A binary tree node has:
- a value
- pointer/reference to left child
- pointer/reference to right child

### Preorder traversal
Preorder means:
1. Visit current node
2. Traverse left subtree
3. Traverse right subtree

### Why recursion works
The tree is naturally recursive:
- a tree is a node plus subtrees
- so the traversal becomes a simple recursive pattern

### Complexity
- **Time:** O(n) (visit each node once)
- **Extra space:** O(h) recursion stack (h = height)

#### Important warning: height matters
- Balanced tree: h ≈ log n (usually safe)
- Degenerate tree (like a linked list): h ≈ n (risk stack overflow)

**Interview note:** If recursion depth might be large, convert to an explicit stack.

---

## 4 - Example 2 — Level-Order Traversal (BFS) on a Tree (5.5)

### What BFS is doing on a tree
Level-order traversal is **BFS**:
- start at root (depth 0)
- visit all children (depth 1)
- then grandchildren (depth 2), etc.

### Why a queue
A queue enforces **FIFO**:
- nodes discovered earlier are processed earlier
- that naturally groups nodes by levels

### Two common BFS output styles
1. **Print nodes in BFS order** (simple queue loop)
2. **Group by level** (use queue size at start of each level)

### Complexity
- **Time:** O(n)
- **Extra space:** O(n) worst-case queue size (wide level)

**Interview note:**  
If the problem says “level”, “depth”, “minimum depth”, “closest leaf”, think BFS.

---

## 5 - Example 3 — DFS on a Graph (5.6)

### The graph representation
We store a graph as an **adjacency list**:
- graph[u] = list of neighbors of u

This is usually best for sparse graphs:
- space is O(n + m) (nodes + edges)

### Why visited matters
Graphs can contain cycles:
- if you DFS without visited, you can loop forever

So DFS includes:
- `visited[node] = true`
- recurse to each unvisited neighbor

### What DFS is useful for
With DFS + visited you can:
- find all nodes in a connected component
- detect cycles (especially with extra state in directed graphs)
- do topological sort in DAGs (postorder push)

### Complexity
- **Time:** O(n + m)
- **Extra space:** O(n) visited + recursion depth

**Interview note:**  
If recursion is risky, use an explicit stack and push neighbors manually.

---

## 6 - Example 4 — Shortest Path in an Unweighted Graph (BFS) (5.7)

### Why BFS finds shortest paths (unweighted)
In an unweighted graph, every edge costs the same (1 step).  
BFS explores by increasing distance:
- first all nodes at distance 1
- then distance 2
- etc.

So the **first time** you reach a node is guaranteed to be via the fewest edges.

### Core idea
Maintain:
- `dist[]` initialized to -1 (unreached)
- queue seeded with `start`
- dist[start] = 0

Then:
- pop node
- for each neighbor:
  - if dist[neighbor] == -1:
    - dist[neighbor] = dist[node] + 1
    - enqueue neighbor

### Enhancements you’ll often add
- **parent[]** to reconstruct path
- **multi-source BFS** (push multiple starts with dist 0)
- **state-space BFS** (nodes are “states” like puzzles)

### When BFS is NOT enough
If edges have different positive weights:
- BFS can be wrong
- you need **Dijkstra** (priority queue)

### Complexity
- **Time:** O(n + m)
- **Extra space:** O(n)

---

## 7 - Quick Practice Wrap (5.8)

When asked for time/space:
- Tree traversals: **O(n)** time, **O(h)** recursion (or stack)
- Graph DFS/BFS: **O(n + m)** time, **O(n)** for visited + queue/stack

A quick checklist:
- Do we have cycles? → use **visited**
- Do we care about shortest steps in unweighted graph? → **BFS**
- Do we need to explore/validate/compute components or order? → **DFS**

---

## 8 - Closing (15–30 seconds)

Trees and graphs look intimidating, but they’re mostly traversal patterns:
- **DFS** = stack/recursion + visited
- **BFS** = queue + visited + optional distance/parent

If you can recognize which pattern matches the problem statement,
you’re 80% of the way to the solution.
