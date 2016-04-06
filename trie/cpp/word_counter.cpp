#include <fstream>
#include <iostream>

#include "trie_dict.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  trie_dict<int> t;
  {
    ifstream f(argv[1]);
    if (f.is_open()) {
      string word;
      while (f >> word) {
        if (t.has_key(word))
          t[word]++;
        else
          t[word] = 1;
      }
    }
    f.close();
  }
  {
    string word;
    while (cin >> word) {
      if (t.has_key(word)) cout << t[word] << endl;
      else cout << 0 << endl;
    }
  }
  return 0;
}
