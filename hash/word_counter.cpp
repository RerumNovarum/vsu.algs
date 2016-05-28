#include <fstream>
#include <iostream>

#include "hashmap_cc.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  hashmap_cc<string, int> t;
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
  {
    string word;
    while (cin >> word) {
      if (t.has(word)) cout << t[word] << endl;
      else cout << 0 << endl;
    }
  }
  return 0;
}
