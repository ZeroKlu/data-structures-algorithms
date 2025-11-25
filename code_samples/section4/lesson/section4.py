"""Section 4: Trees and Graphs."""

from collections import deque

class IntStack:
    """Integer stack."""
    def __init__(self) -> None:
        self._data: list[int] = []

    def push(self, value: int) -> None:
        """Push value onto stack."""
        self._data.append(value)

    def pop(self) -> int:
        """Pop value from stack."""
        if not self._data:
            raise IndexError("stack underflow")
        return self._data.pop()

    def is_empty(self) -> bool:
        """Check if stack is empty."""
        return len(self._data) == 0

class IntQueue:
    """Integer queue."""
    def __init__(self) -> None:
        self._data: deque[int] = deque()

    def enqueue(self, value: int) -> None:
        """Push value onto queue."""
        self._data.append(value)

    def dequeue(self) -> int:
        """Pop value from queue."""
        if not self._data:
            raise IndexError("queue underflow")
        return self._data.popleft()

    def is_empty(self) -> bool:
        """Check if queue is empty."""
        return len(self._data) == 0

def is_matching(open_c: str, close_c: str) -> bool:
    """Check if open and close characters match."""
    return (open_c == "(" and close_c == ")") or \
           (open_c == "[" and close_c == "]") or \
           (open_c == "{" and close_c == "}")

def is_valid_parentheses(s: str) -> bool:
    """Check if parentheses are valid."""
    stack: list[str] = []
    for c in s:
        if c in "([{":
            stack.append(c)
        elif c in ")]}":
            if not stack:
                return False
            open_c = stack.pop()
            if not is_matching(open_c, c):
                return False
    return not stack

def bfs(start: int, graph: list[list[int]]) -> None:
    """Breadth-first search."""
    n = len(graph)
    visited = [False] * n
    queue: deque[int] = deque()

    visited[start] = True
    queue.append(start)

    while queue:
        node = queue.popleft()
        print(node)
        for neighbor in graph[node]:
            if not visited[neighbor]:
                visited[neighbor] = True
                queue.append(neighbor)

def main () -> None:
    """Main function."""
    print("==== TESTING IntStack ====")
    stack = IntStack()

    print("Pushing 10, 20, 30...")
    stack.push(10)
    stack.push(20)
    stack.push(30)

    print("Popping values:")
    while not stack.is_empty():
        v = stack.pop()
        print("  popped:", v)

    print("\n==== TESTING IntQueue ====")
    q = IntQueue()

    print("Enqueuing 1, 2, 3...")
    q.enqueue(1)
    q.enqueue(2)
    q.enqueue(3)

    print("Dequeuing values:")
    while not q.is_empty():
        v = q.dequeue()
        print("  dequeued:", v)

    print("\n==== TESTING is_valid_parentheses ====")

    tests = [
        "()",
        "([])",
        "{[()]}",
        "([)]",
        "((())",
        "abc(def[ghi]{jkl})",
        "",
        "{[}]"
    ]

    for s in tests:
        print(f'Test "{s}":', "valid" if is_valid_parentheses(s) else "invalid")

    print("\n==== TESTING bfs ====")

    # Graph:
    # 0 -- 1
    # |  /
    # 2
    graph = [
        [1, 2],  # neighbors of 0
        [0, 2],  # neighbors of 1
        [0, 1]   # neighbors of 2
    ]

    print("BFS starting from node 0:")
    bfs(0, graph)

    print("\n==== ALL TESTS COMPLETE ====")

if __name__ == "__main__":
    main()
