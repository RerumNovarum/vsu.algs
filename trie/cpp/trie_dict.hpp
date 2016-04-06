#include <stdexcept>
#include <map>

template<typename V>
class trie_dict {
  private:
    std::map<char, trie_dict<V>*> next;
    bool word;
    V v;

    trie_dict<V>* find(std::string &key) {
      auto t = this;
      int i = 0;
      int end = key.size();
      while (t != nullptr && i != end) {
        auto it = t->next.find(key[i]);
        if (it == t->next.end()) break;
        t = it->second;
        i += 1;
      }
      if (i == end) return t;
      return nullptr;
    }

  public:
    bool has_key(std::string &key) {
      auto t = this->find(key);
      return t != nullptr && t->word;
    }

    V& at(std::string &key) {
      auto t = this->find(key);
      if (t == nullptr || !t->word) throw std::out_of_range("No such key");
      return t->v;
    }

    V& operator[] (std::string key) {
      auto t = this;
      int i = 0;
      int end = key.size();
      while(i != end) {
        char c = key[i];
        if (t->next.find(c) == t->next.end()) t->next[c] = new trie_dict<V>();
        t = t->next[c];
        i += 1;
      }
      t->word = true;
      return t->v;
    }
};
