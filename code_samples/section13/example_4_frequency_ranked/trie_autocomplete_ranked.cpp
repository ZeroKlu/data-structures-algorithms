// trie_autocomplete_ranked.cpp
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include <algorithm>

class Trie {
public:
    Trie() : root(std::make_unique<Node>()) {}

    void insert(const std::string& word) {
        Node* cur = root.get();
        for (unsigned char ch : word) {
            char c = static_cast<char>(std::tolower(ch));
            int i = idx(c);
            if (i < 0) return;
            if (!cur->children[i]) cur->children[i] = std::make_unique<Node>();
            cur = cur->children[i].get();
        }
        cur->isEnd = true;
        cur->frequency++;
    }

    std::vector<std::pair<std::string,int>>
    autocompleteRanked(const std::string& prefix, size_t limit) const {
        const Node* start = walk(prefix);
        if (!start) return {};

        std::vector<std::pair<std::string,int>> all;
        std::string buf = prefix;
        for (char& c : buf) c = std::tolower(c);

        dfs(start, buf, all);
        std::sort(all.begin(), all.end(),
            [](auto& a, auto& b) {
                if (a.second != b.second) return a.second > b.second;
                return a.first < b.first;
            });
        if (all.size() > limit) all.resize(limit);
        return all;
    }

private:
    struct Node {
        std::array<std::unique_ptr<Node>,26> children{};
        bool isEnd{false};
        int frequency{0};
    };

    std::unique_ptr<Node> root;

    static int idx(char c) { return (c >= 'a' && c <= 'z') ? c - 'a' : -1; }

    const Node* walk(const std::string& s) const {
        const Node* cur = root.get();
        for (unsigned char ch : s) {
            char c = static_cast<char>(std::tolower(ch));
            int i = idx(c);
            if (i < 0 || !cur->children[i]) return nullptr;
            cur = cur->children[i].get();
        }
        return cur;
    }

    static void dfs(const Node* node, std::string& buf,
                    std::vector<std::pair<std::string,int>>& out) {
        if (node->isEnd) out.emplace_back(buf, node->frequency);
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                buf.push_back(char('a' + i));
                dfs(node->children[i].get(), buf, out);
                buf.pop_back();
            }
        }
    }
};

static void loadFile(Trie& t, const std::string& path) {
    std::ifstream in(path);
    std::string w;
    while (std::getline(in, w)) if (!w.empty()) t.insert(w);
}

int main(int argc, char** argv) {
    Trie trie;
    std::string dict = argc > 1 ? argv[1] : "..\\data\\words.txt";
    std::string freq = argc > 2 ? argv[2] : "..\\data\\frequency.txt";
    std::string prefix = argc > 3 ? argv[3] : "th";

    loadFile(trie, dict);
    loadFile(trie, freq);

    for (auto& [w,f] : trie.autocompleteRanked(prefix, 20))
        std::cout << w << "\t(freq=" << f << ")\n";
}
