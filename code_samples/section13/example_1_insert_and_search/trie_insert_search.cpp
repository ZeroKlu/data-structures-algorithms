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

    void insert(const std::string& word) {
        Node* current = root.get();
        for (unsigned char ch : word) {
            char c = static_cast<char>(std::tolower(ch));
            int idx = index(c);
            if (idx < 0) return; // reject words containing non a-z characters
            if (!current->children[idx]) {
                current->children[idx] = std::make_unique<Node>();
            }
            current = current->children[idx].get();
        }
        current->isEnd = true;
    }

    bool search(const std::string& word) const {
        const Node* node = walk(word);
        return node && node->isEnd;
    }

    bool startsWith(const std::string& prefix) const {
        return walk(prefix) != nullptr;
    }

private:
    struct Node {
        std::array<std::unique_ptr<Node>, 26> children{};
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

int main() {
    Trie trie;

    const std::string dictPath = "..\\data\\words.txt";
    int loaded = loadDictionary(trie, dictPath);
    std::cout << "Loaded " << loaded << " words from " << dictPath << "\n\n";

    const std::vector<std::string> tests = {
        "aardvark",
        "abandon",
        "zebra",
        "notaword"
    };

    for (const auto& w : tests) {
        std::cout << "search(\"" << w << "\") = " << (trie.search(w) ? "true" : "false") << "\n";
    }

    std::cout << "\nPrefix tests:\n";
    const std::vector<std::string> prefixes = {"ab", "alg", "aard", "nope"};
    for (const auto& p : prefixes) {
        std::cout << "startsWith(\"" << p << "\") = " << (trie.startsWith(p) ? "true" : "false") << "\n";
    }

    return 0;
}
