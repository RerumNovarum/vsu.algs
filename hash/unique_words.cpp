#include <fstream>
#include <iostream>
#include <algorithm>

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
  // for (auto it = t.begin(); it != t.end(); ++it) {
  //   cout << (*it).first << ": " << (*it).second << endl;
  // }
  //
  // instead, make sure simplest standard STL algorithms
  // can work with data structure:
  auto print_word = [](pair<string, int> p) { cout << p.first << ": " << p.second << endl; };
  for_each(t.begin(), t.end(), print_word);
  return 0;
}
