#ifndef _TRIE_DICT
#define _TRIE_DICT

#include <stdexcept>
#include <cassert>
#include <map>
#include <stack>

template <typename V>
class _Trie_dict_iterator;

template <typename V>
struct trie_dict_node {
  std::map<char, trie_dict_node<V>*> next;
  bool word;
  V v;
};

template <typename V>
class trie_dict {
  typedef _Trie_dict_iterator<V> iterator;
  typedef trie_dict_node<V> _Node;
  friend class _Trie_dict_iterator<V>;

  private:
    _Node head;
    static iterator _end_iterator;

    trie_dict<V>* find(std::string &key) {
      auto t = head;
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
      auto t = head;
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

    iterator begin() {
      return iterator(this);
    }
    iterator end() {
      return _end_iterator;
    }
};

template <typename V>
_Trie_dict_iterator<V> trie_dict<V>::_end_iterator;

template <typename V>
struct _Trie_dict_iterator {
  typedef _Trie_dict_iterator<V> _Self;
  typedef trie_dict_node<V> _Node;
  typedef V   value_type;
  typedef V&  reference;
  typedef const V&  const_reference;

  std::stack<_Node*> _nodes;
  std::stack<int> _len;
  std::string _s;
  int n;

  // creates pass-the-end iterator
  _Trie_dict_iterator() : n(-1) {} 
  _Trie_dict_iterator(_Node *root) {
    n = -1;
    if (root != nullptr) {
      _nodes.push(root);
      _len.push(0);
      ++(*this);
    }
  }

  bool operator== (const _Self& that) {
    return
      this->n == that.n
      || (this->_nodes.size() == 0 && that.n==-1)
      || (this->n == -1 && that._nodes.size() == 0);
  }
  bool operator!= (const _Self& that) {
    return !(*this == that);
  }

  _Self& operator++ () {
    assert(_nodes.size() > 0);
    auto old = _nodes.top();
    while (_nodes.size() > 0 && (!_nodes.top()->word || _nodes.top() == old)) {
      auto t = _nodes.top();
      int len = _len.top();
      _nodes.pop();
      _len.pop();
      for (auto it = t->next.rbegin(); it != t->next.rend(); ++it) {
        _nodes.push(it->second);
        _len.push(len+1);
      }
    }
    if (_nodes.size() > 0 && _nodes.top()->word) {
      ++n;
      int len = _len.top();
//      while(_s.size() != )
    }
    return *this;
  }

  std::pair<std::string, const_reference> operator* () {
    return _nodes.top()->v;
  }
};

#endif
