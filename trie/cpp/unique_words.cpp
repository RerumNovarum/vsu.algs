#include <fstream>
#include <iostream>

#include "trie_dict.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  trie_dict<string, int> t;
  {
    ifstream f(argv[1]);
    if (f.is_open()) {
      string word;
      while (f >> word) {
        if (t.has(word))
          t[word]++;
        else
          t[word] = 1;
      }
    }
    f.close();
  }
  for (auto it = t.begin(); it != t.end(); ++it) {
    cout << (*it).first << ": " << (*it).second << endl;
  }
  return 0;
}
