class TreeNode {
    constructor(val, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}

function maxRootToLeaf(root) {
    if (!root) {
        return Number.NEGATIVE_INFINITY;
    }
    if (!root.left && !root.right) {
        return root.val;
    }
    const leftBest = maxRootToLeaf(root.left);
    const rightBest = maxRootToLeaf(root.right);
    return root.val + Math.max(leftBest, rightBest);
}

function tspBitmask(n, dist) {
    const maxMask = 1 << n;
    const INF = Number.MAX_SAFE_INTEGER;
    const dp = Array.from({ length: maxMask }, () =>
        new Array(n).fill(INF)
    );
    dp[1 << 0][0] = 0;

    for (let mask = 0; mask < maxMask; mask++) {
        for (let j = 0; j < n; j++) {
            if (!(mask & (1 << j))) continue;
            const curr = dp[mask][j];
            if (curr === INF) continue;
            for (let k = 0; k < n; k++) {
                if (mask & (1 << k)) continue;
                const nextMask = mask | (1 << k);
                const cand = curr + dist[j][k];
                if (cand < dp[nextMask][k]) {
                    dp[nextMask][k] = cand;
                }
            }
        }
    }

    const fullMask = maxMask - 1;
    let best = INF;
    for (let j = 0; j < n; j++) {
        best = Math.min(best, dp[fullMask][j]);
    }
    return best;
}

function matrixChainOrder(p) {
    const n = p.length - 1;
    const INF = Number.MAX_SAFE_INTEGER;
    const dp = Array.from({ length: n + 1 }, () =>
        new Array(n + 1).fill(0)
    );

    for (let len = 2; len <= n; len++) {
        for (let i = 1; i <= n - len + 1; i++) {
            const j = i + len - 1;
            dp[i][j] = INF;
            for (let k = i; k < j; k++) {
                const cost =
                    dp[i][k] +
                    dp[k + 1][j] +
                    p[i - 1] * p[k] * p[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                }
            }
        }
    }
    return dp[1][n];
}

// ======================================================
// Test Harness
// ======================================================

console.log("=== Test 1: maxRootToLeaf ===\n");

// Build test tree:
//       10
//      /  \
//     5    20
//    / \     \
//   3   7     25
//
// Best path sum = 10 + 20 + 25 = 55

const root = new TreeNode(
    10,
    new TreeNode(
        5,
        new TreeNode(3),
        new TreeNode(7)
    ),
    new TreeNode(
        20,
        null,
        new TreeNode(25)
    )
);

const result1 = maxRootToLeaf(root);
console.log("maxRootToLeaf(tree) =", result1, "(expected 55)");

console.log("\n=== Test 2: tspBitmask ===\n");

const dist = [
    [0, 10, 15, 20],
    [10, 0, 35, 25],
    [15, 35, 0, 30],
    [20, 25, 30, 0]
];

const tspCost = tspBitmask(4, dist);
console.log("tspBitmask(4 nodes) =", tspCost, "(expected 65)");

console.log("\n=== Test 3: matrixChainOrder ===\n");

// CLRS example → expected 15125
const p1 = [30, 35, 15, 5, 10, 20, 25];
console.log(
    "matrixChainOrder(CLRS) =",
    matrixChainOrder(p1),
    "(expected 15125)"
);

// Simple 2-matrix → expected 6000
const p2 = [10, 20, 30];
console.log(
    "matrixChainOrder([10,20,30]) =",
    matrixChainOrder(p2),
    "(expected 6000)"
);

// Single matrix → expected 0
const p3 = [5, 10];
console.log(
    "matrixChainOrder(single) =",
    matrixChainOrder(p3),
    "(expected 0)"
);
