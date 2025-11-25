#include <stack>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <iostream>

using namespace std;

bool isValid(const string& s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else {
            if (st.empty()) return false;
            char open = st.top();
            st.pop();
            if ((c == ')' && open != '(') ||
                (c == ']' && open != '[') ||
                (c == '}' && open != '{')) {
                return false;
            }
        }
    }
    return st.empty();
}

class MyQueue {
    stack<int> in, out;

    void moveInToOut() {
        if (out.empty()) {
            while (!in.empty()) {
                out.push(in.top());
                in.pop();
            }
        }
    }

public:
    void enqueue(int x) {
        in.push(x);
    }

    int dequeue() {
        moveInToOut();
        int v = out.top();
        out.pop();
        return v;
    }

    int front() {
        moveInToOut();
        return out.top();
    }

    bool empty() const {
        return in.empty() && out.empty();
    }
};

vector<int> nextGreaterElements(const vector<int>& nums) {
    int n = (int)nums.size();
    vector<int> ans(n, -1);
    stack<int> st; // will store values, decreasing

    for (int i = n - 1; i >= 0; i--) {
        int x = nums[i];
        while (!st.empty() && st.top() <= x) {
            st.pop();
        }
        ans[i] = st.empty() ? -1 : st.top();
        st.push(x);
    }
    return ans;
}

vector<int> dailyTemperatures(const vector<int>& temps) {
    int n = (int)temps.size();
    vector<int> ans(n, 0);
    stack<int> st; // indices

    for (int i = 0; i < n; i++) {
        while (!st.empty() && temps[i] > temps[st.top()]) {
            int idx = st.top();
            st.pop();
            ans[idx] = i - idx;
        }
        st.push(i);
    }
    return ans;
}

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

vector<vector<int>> levelOrder(TreeNode *root) {
    vector<vector<int>> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = (int)q.size();
        vector<int> level;
        level.reserve(levelSize);
        for (int i = 0; i < levelSize; i++) {
            TreeNode *node = q.front();
            q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(move(level));
    }
    return result;
}

vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
    int n = (int)nums.size();
    vector<int> result;
    if (n == 0 || k == 0) return result;

    deque<int> dq; // indices
    for (int i = 0; i < n; i++) {
        while (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
        dq.push_back(i);
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    return result;
}

int main() {
    using std::cout;
    using std::endl;

    // ==== TEST isValid ====
    cout << "==== TEST isValid ====\n";
    vector<string> parenTests = {
        "()",
        "([])",
        "{[()]}",
        "([)]",
        "((())",
        "abc(def[ghi]{jkl})",
        "",
        "{[}]"
    };

    for (const auto& s : parenTests) {
        cout << "Test \"" << s << "\": "
             << (isValid(s) ? "valid" : "invalid") << '\n';
    }

    // ==== TEST MyQueue ====
    cout << "\n==== TEST MyQueue ====\n";
    MyQueue q;
    cout << "Enqueue: 1, 2, 3\n";
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    cout << "Front: " << q.front() << '\n';
    cout << "Dequeue: " << q.dequeue() << '\n';
    cout << "Dequeue: " << q.dequeue() << '\n';
    cout << "Empty? " << (q.empty() ? "true" : "false") << '\n';
    cout << "Dequeue: " << q.dequeue() << '\n';
    cout << "Empty? " << (q.empty() ? "true" : "false") << '\n';

    // ==== TEST nextGreaterElements ====
    cout << "\n==== TEST nextGreaterElements ====\n";
    vector<int> nums = {2, 1, 2, 4, 3};
    auto nge = nextGreaterElements(nums);

    cout << "nums: ";
    for (int x : nums) cout << x << ' ';
    cout << "\nNGE : ";
    for (int x : nge) cout << x << ' ';
    cout << '\n';

    // ==== TEST dailyTemperatures ====
    cout << "\n==== TEST dailyTemperatures ====\n";
    vector<int> temps = {73, 74, 75, 71, 69, 72, 76, 73};
    auto days = dailyTemperatures(temps);

    cout << "temps: ";
    for (int t : temps) cout << t << ' ';
    cout << "\nwait : ";
    for (int d : days) cout << d << ' ';
    cout << '\n';

    // ==== TEST levelOrder ====
    cout << "\n==== TEST levelOrder ====\n";

    // Build this tree:
    //
    //        1
    //       / \
    //      2   3
    //     /   / \
    //    4   5   6
    //
    TreeNode* node4 = new TreeNode(4);
    TreeNode* node5 = new TreeNode(5);
    TreeNode* node6 = new TreeNode(6);
    TreeNode* node2 = new TreeNode(2);
    TreeNode* node3 = new TreeNode(3);
    TreeNode* root  = new TreeNode(1);

    node2->left = node4;
    node3->left = node5;
    node3->right = node6;
    root->left = node2;
    root->right = node3;

    auto levels = levelOrder(root);
    for (size_t i = 0; i < levels.size(); ++i) {
        cout << "level " << i << ": ";
        for (int v : levels[i]) {
            cout << v << ' ';
        }
        cout << '\n';
    }

    // Clean up tree nodes (not strictly necessary at program end,
    // but good practice)
    delete node4;
    delete node5;
    delete node6;
    delete node2;
    delete node3;
    delete root;

    // ==== TEST maxSlidingWindow ====
    cout << "\n==== TEST maxSlidingWindow ====\n";
    vector<int> arr = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;

    auto msw = maxSlidingWindow(arr, k);

    cout << "nums: ";
    for (int x : arr) cout << x << ' ';
    cout << "\nmax window size " << k << ": ";
    for (int x : msw) cout << x << ' ';
    cout << '\n';

    cout << "\n==== ALL TESTS COMPLETE ====\n";
    return 0;
}
