"""Section 4 - Stacks and Queues"""

from collections import deque
from typing import Optional

def is_valid(s: str) -> bool:
    """Check if parentheses are valid."""
    stack: list[str] = []
    match: dict[str, str] = {')': '(', ']': '[', '}': '{'}
    for ch in s:
        if ch in '([{':
            stack.append(ch)
        else:
            if not stack:
                return False
            open_ch = stack.pop()
            if match.get(ch) != open_ch:
                return False
    return not stack

class MyQueue:
    """Queue"""
    def __init__(self) -> None:
        self._in: list[int] = []
        self._out: list[int] = []

    def _move_in_to_out(self) -> None:
        if not self._out:
            while self._in:
                self._out.append(self._in.pop())

    def enqueue(self, x: int) -> None:
        """Push value onto queue."""
        self._in.append(x)

    def dequeue(self) -> int:
        """Pop value from queue."""
        self._move_in_to_out()
        return self._out.pop()

    def front(self) -> int:
        """Return value at front of queue."""
        self._move_in_to_out()
        return self._out[-1]

    def empty(self) -> bool:
        """Check if queue is empty."""
        return not self._in and not self._out

def next_greater_elements(nums: list[int]) -> list[int]:
    """Next greater element."""
    n = len(nums)
    ans = [-1] * n
    stack: list[int] = []  # values

    for i in range(n - 1, -1, -1):
        x = nums[i]
        while stack and stack[-1] <= x:
            stack.pop()
        ans[i] = stack[-1] if stack else -1
        stack.append(x)
    return ans

def daily_temperatures(temps: list[int]) -> list[int]:
    """Daily temperatures."""
    n = len(temps)
    ans = [0] * n
    stack: list[int] = []  # indices

    for i, t in enumerate(temps):
        while stack and t > temps[stack[-1]]:
            idx = stack.pop()
            ans[idx] = i - idx
        stack.append(i)
    return ans

class TreeNode:
    """Tree node."""
    def __init__(self, val: int, left: "TreeNode | None" = None,
                 right: "TreeNode | None" = None) -> None:
        self.val = val
        self.left = left
        self.right = right

def level_order(root: Optional[TreeNode]) -> list[list[int]]:
    """Level order traversal."""
    result: list[list[int]] = []
    if root is None:
        return result

    q: deque[TreeNode] = deque([root])
    while q:
        level_size = len(q)
        level: list[int] = []
        for _ in range(level_size):
            node = q.popleft()
            level.append(node.val)
            if node.left:
                q.append(node.left)
            if node.right:
                q.append(node.right)
        result.append(level)
    return result

def max_sliding_window(nums: list[int], k: int) -> list[int]:
    """Max sliding window."""
    n = len(nums)
    if n == 0 or k == 0:
        return []

    dq: deque[int] = deque()  # indices
    result: list[int] = []

    for i, x in enumerate(nums):
        while dq and dq[0] <= i - k:
            dq.popleft()
        while dq and nums[dq[-1]] <= x:
            dq.pop()
        dq.append(i)
        if i >= k - 1:
            result.append(nums[dq[0]])
    return result


def main() -> None:
    """Main function."""
    print("==== TEST is_valid ====")
    paren_tests = [
        "()",
        "([])",
        "{[()]}",
        "([)]",
        "((())",
        "abc(def[ghi]{jkl})",
        "",
        "{[}]",
    ]

    for s in paren_tests:
        print(f'Test "{s}":', "valid" if is_valid(s) else "invalid")

    print("\n==== TEST MyQueue ====")
    q = MyQueue()
    print("Enqueue: 1, 2, 3")
    q.enqueue(1)
    q.enqueue(2)
    q.enqueue(3)

    print("Front:", q.front())
    print("Dequeue:", q.dequeue())
    print("Dequeue:", q.dequeue())
    print("Empty?", "true" if q.empty() else "false")
    print("Dequeue:", q.dequeue())
    print("Empty?", "true" if q.empty() else "false")

    print("\n==== TEST next_greater_elements ====")
    nums = [2, 1, 2, 4, 3]
    nge = next_greater_elements(nums)
    print("nums:", nums)
    print("NGE :", nge)

    print("\n==== TEST daily_temperatures ====")
    temps = [73, 74, 75, 71, 69, 72, 76, 73]
    waits = daily_temperatures(temps)
    print("temps:", temps)
    print("wait :", waits)

    print("\n==== TEST level_order ====")

    # Build this tree:
    #
    #        1
    #       / \
    #      2   3
    #     /   / \
    #    4   5   6
    #
    node4 = TreeNode(4)
    node5 = TreeNode(5)
    node6 = TreeNode(6)
    node2 = TreeNode(2, left=node4)
    node3 = TreeNode(3, left=node5, right=node6)
    root = TreeNode(1, left=node2, right=node3)

    levels = level_order(root)
    for i, level in enumerate(levels):
        print(f"level {i}:", level)

    print("\n==== TEST max_sliding_window ====")
    arr = [1, 3, -1, -3, 5, 3, 6, 7]
    k = 3
    msw = max_sliding_window(arr, k)
    print("nums:", arr)
    print(f"max window size {k}:", msw)

    print("\n==== ALL TESTS COMPLETE ====")

if __name__ == "__main__":
    main()
