#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

vector<vector<string> > groupAnagrams(const vector<string> &strs) {
    unordered_map<string, vector<string> > mp;
    for (const string &s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        mp[key].push_back(s);
    }
    vector<vector<string> > result;
    result.reserve(mp.size());
    for (auto &kv : mp) {
        result.push_back(move(kv.second));
    }
    return result;
}

int main() {
    vector<string> words = {"eat", "tea", "tan", "ate", "nat", "bat"};

    cout << "=== Test: groupAnagrams ===\n\n";
    cout << "Input words: ";
    for (const auto &w : words) cout << w << " ";
    cout << "\n\n";

    auto groups = groupAnagrams(words);

    cout << "Anagram Groups:\n";
    for (const auto &group : groups) {
        cout << "  [ ";
        for (const auto &s : group) cout << s << " ";
        cout << "]\n";
    }

    cout << "\nExpected groups (order may vary):\n"
         << "  [eat, tea, ate]\n"
         << "  [tan, nat]\n"
         << "  [bat]\n";

    return 0;
}
