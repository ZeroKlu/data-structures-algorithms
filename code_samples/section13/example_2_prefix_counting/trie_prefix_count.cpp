#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Trie {
public:
    Trie() : root(std::make_unique<Node>()) {}

    // Insert a word; increments prefixCount along the path.
    // Rejects the entire word if any char is not a-z (case-insensitive).
    void insert(const std::string& word) {
        Node* current = root.get();
        for (unsigned char ch : word) {
            char c = static_cast<char>(std::tolower(ch));
            int idx = index(c);
            if (idx < 0) return; // reject word
            if (!current->children[idx]) {
                current->children[idx] = std::make_unique<Node>();
            }
            current = current->children[idx].get();
            current->prefixCount++;
        }
        current->isEnd = true;
    }

    // Count how many words start with prefix (0 if not found / invalid).
    int prefixCount(const std::string& prefix) const {
        const Node* node = walk(prefix);
        return node ? node->prefixCount : 0;
    }

private:
    struct Node {
        std::array<std::unique_ptr<Node>, 26> children{};
        int  prefixCount{0}; // number of words sharing this prefix
        bool isEnd{false};
    };

    std::unique_ptr<Node> root;

    static int index(char c) {
        if (c < 'a' || c > 'z') return -1;
        return c - 'a';
    }

    const Node* walk(const std::string& s) const {
        const Node* current = root.get();
        for (unsigned char ch : s) {
            char c = static_cast<char>(std::tolower(ch));
            int idx = index(c);
            if (idx < 0) return nullptr;
            const auto& child = current->children[idx];
            if (!child) return nullptr;
            current = child.get();
        }
        return current;
    }
};

static int loadDictionary(Trie& trie, const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Failed to open dictionary: " << path << "\n";
        return 0;
    }

    int count = 0;
    std::string line;
    while (std::getline(in, line)) {
        // Trim Windows CR if present
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        trie.insert(line);
        count++;
    }
    return count;
}

int main(int argc, char** argv) {
    Trie trie;

    // Default path matches the lesson convention; can be overridden by CLI arg.
    const std::string dictPath = (argc > 1) ? argv[1] : "..\\data\\words.txt";
    int loaded = loadDictionary(trie, dictPath);

    std::cout << "Loaded " << loaded << " words from " << dictPath << "\n\n";

    const std::vector<std::string> prefixes = {
        "a",
        "ab",
        "alg",
        "aard",
        "z",
        "nope"
    };

    for (const auto& p : prefixes) {
        std::cout << "prefixCount(\"" << p << "\") = " << trie.prefixCount(p) << "\n";
    }

    return 0;
}
