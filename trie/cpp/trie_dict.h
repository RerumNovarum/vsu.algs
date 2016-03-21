template<typename V>
struct Trie {
    std::map<std::string, Trie<V>*> next;
    bool word;
    V v;

    bool has(std::string key, int i) {
      auto t = this;
      while(t != nullptr) {
        if ()
      }
    }
}
