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
            if (idx < 0) return;
            if (!current->children[idx]) current->children[idx] = std::make_unique<Node>();
            current = current->children[idx].get();
        }
        current->isEnd = true;
    }

    std::vector<std::string> autocomplete(const std::string& prefix, std::size_t limit) const {
        const Node* start = walk(prefix);
        if (!start) return {};

        std::string seed;
        seed.reserve(prefix.size());
        for (unsigned char ch : prefix) seed.push_back(static_cast<char>(std::tolower(ch)));

        std::vector<std::string> out;
        out.reserve(limit);
        dfsCollect(start, seed, out, limit);
        return out;
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

    static void dfsCollect(const Node* node,
                           std::string& buffer,
                           std::vector<std::string>& out,
                           std::size_t limit) {
        if (!node || out.size() >= limit) return;

        if (node->isEnd) {
            out.push_back(buffer);
            if (out.size() >= limit) return;
        }

        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                buffer.push_back(static_cast<char>('a' + i));
                dfsCollect(node->children[i].get(), buffer, out, limit);
                buffer.pop_back();
                if (out.size() >= limit) return;
            }
        }
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
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        trie.insert(line);
        count++;
    }
    return count;
}

static std::size_t parseLimit(const char* s, std::size_t def) {
    if (!s || !*s) return def;
    try {
        std::size_t v = static_cast<std::size_t>(std::stoul(s));
        return v == 0 ? def : v;
    } catch (...) {
        return def;
    }
}

int main(int argc, char** argv) {
    Trie trie;

    const std::string dictPath = (argc > 1) ? argv[1] : "..\\data\\words.txt";
    const std::string prefix   = (argc > 2) ? argv[2] : "ab";
    const std::size_t limit    = (argc > 3) ? parseLimit(argv[3], 20) : 20;

    int loaded = loadDictionary(trie, dictPath);
    std::cout << "Loaded " << loaded << " words from " << dictPath << "\n\n";

    std::cout << "Autocomplete(\"" << prefix << "\") [limit=" << limit << "]\n";
    auto results = trie.autocomplete(prefix, limit);
    for (const auto& w : results) std::cout << w << "\n";

    return 0;
}
