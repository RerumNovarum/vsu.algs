#ifndef _TRIE_DICT
#define _TRIE_DICT

#include <stdexcept>
#include <cassert>
#include <map>
#include <algorithm>

#define TRIE_DEC \
  template< \
    typename K = std::string, \
    typename V = null_type>

struct null_type {};

TRIE_DEC
struct _trie_dict_node {
  typedef std::iterator_traits<typename K::const_iterator> atraits;
  typedef typename atraits::value_type char_type;
  typedef _trie_dict_node<K,V>*     node_ptr;

  std::map<char_type, node_ptr> next;
  node_ptr prev;
  bool word;
  V v;

  _trie_dict_node() : prev(nullptr), word(false) {}
  _trie_dict_node(node_ptr previous, bool is_word = false) :
    word(is_word),
    prev(previous) {}
};

TRIE_DEC
class trie_dict {
  public:
    typedef std::iterator_traits<typename K::const_iterator> atraits;
    typedef typename atraits::value_type char_type;
    typedef typename K::iterator   char_iterator;
    typedef typename K::const_iterator char_const_iterator;

    typedef V& val_ref;
    typedef const val_ref val_const_ref;
    typedef K& key_ref;
    typedef const key_ref key_const_ref;

    typedef _trie_dict_node<K,V> node;
    typedef node*                node_ptr;

    class const_iterator;
    
    trie_dict() : root(nullptr), count(0) {}
    int size();
    bool has(key_const_ref k);
    void put(key_const_ref k, V v);
    val_ref at(key_const_ref k);

    val_const_ref operator[](key_const_ref k) const;
    val_ref       operator[](key_const_ref k);

    const_iterator begin();
    const_iterator end();

  private:
    int count;
    const_iterator _end;
    node_ptr root;

    node_ptr find_node(key_const_ref k);
    node_ptr push_node(key_const_ref k);
};

#define TRIE_T_DEC template<typename K, typename V>
#define TRIE_C_DEC trie_dict<K,V>
#define TRIE_C typename trie_dict<K, V>

TRIE_T_DEC
int TRIE_C_DEC::size() { return this->count; }

TRIE_T_DEC
TRIE_C::node_ptr TRIE_C_DEC::find_node(TRIE_C::key_const_ref k) {
  TRIE_C::node_ptr t = this->root;
  TRIE_C::char_iterator it = k.begin();
  TRIE_C::char_const_iterator e_it = k.end();
  while (root != nullptr && it != e_it) {
    char_type c = *it;
    auto next = t->next.find(c);
    if (next == t->next.end()) return nullptr;
    t = next->second;
    ++it;
  }
  if (it != e_it) return nullptr;
  return t;
}


TRIE_T_DEC
TRIE_C::node_ptr TRIE_C_DEC::push_node(TRIE_C::key_const_ref k) {
  if (this->root == nullptr) this->root = new node;
  TRIE_C::node_ptr t = this->root;
  TRIE_C::char_iterator it = k.begin();
  TRIE_C::char_const_iterator e_it = k.end();
  while (it != e_it) {
    TRIE_C_DEC::char_type c = *it;
    auto next = t->next.find(c);
    if (next == t->next.end()) {
      auto next_t = new node(t);
      t->next[c] = next_t;
      t = next_t;
    } else {
      t = next->second;
    }
    ++it;
  }
  assert(t != nullptr);
  return t;
}

TRIE_T_DEC
bool TRIE_C_DEC::has(TRIE_C_DEC::key_const_ref k) {
  TRIE_C_DEC::node_ptr p = this->find_node(k);
  return (p != nullptr && p->word);
}

TRIE_T_DEC
V& TRIE_C_DEC::at(TRIE_C_DEC::key_const_ref k) {
  TRIE_C::node_ptr p = this->find_node(k);
  if (p == nullptr || !p->word) throw std::out_of_range("no such key");
  return p->v;
}

TRIE_T_DEC
void TRIE_C_DEC::put(TRIE_C::key_const_ref k, V v) {
  TRIE_C::node_ptr p = this->push_node(k);
  if (!p->word) ++this->count;
  p->word = true;
  p->v = v;
}

TRIE_T_DEC
TRIE_C::val_const_ref TRIE_C_DEC::operator[] (TRIE_C::key_const_ref k) const {
  TRIE_C::node_ptr p = this->push_node(k);
  return p->v;
}

TRIE_T_DEC
TRIE_C::val_ref TRIE_C_DEC::operator[] (TRIE_C::key_const_ref k) {
  TRIE_C::node_ptr p = this->push_node(k);
  if (!p->word) ++this->count;
  p->word = true;
  return p->v;
}

TRIE_T_DEC
struct TRIE_C_DEC::const_iterator {
  typedef std::pair<K, V> value_type;
  TRIE_C::node_ptr current_node;
  K key;
  int el_num;

  // (this->current_node == nullptr) \iff (*this == end)
  const_iterator(TRIE_C::node_ptr root = nullptr) {
    this->el_num = -1;
    current_node = root;
  }

  bool operator==(const const_iterator& rhs) const {
    return this->current_node == rhs.current_node;
  }
  bool operator!=(const const_iterator& rhs) const {
    return this->current_node != rhs.current_node;
  }
  value_type operator*() {
    if (this->current_node == nullptr) throw std::out_of_range("underflow while dereferencing iterator");
    if (el_num == -1) ++(*this);
    return std::make_pair(key, current_node->v);
  }
  
  TRIE_C::const_iterator& operator++() {
    ++el_num;
    TRIE_C::node_ptr t = current_node;
    if (t == nullptr && el_num != 0) throw std::out_of_range("underflow while iterating");

    if (t != nullptr) {
      if (key.begin() == key.end()) {
        auto n = t->next.begin();
        key.push_back(n->first);
        t = n->second;
        if (t->word) {
          current_node = t;
          return *this;
        }
      }
      bool down = true;
      do {
        typename std::map<TRIE_C::char_type, TRIE_C::node_ptr>::iterator next;
        if (down) {
          next = t->next.begin();
        } else {
          t = t->prev;
          if (t == nullptr) break;
          TRIE_C::char_type c = key.back();
          next = t->next.upper_bound(c);
          key.pop_back();
        }
        if (next == t->next.end()) {
          down = false;
        } else {
          t = next->second;
          key.push_back(next->first);
          down = true;
          if (t->word) {
            break;
          }
        }
      } while(t != nullptr);
    }
    current_node = t;
    return *this;
  }
};

TRIE_T_DEC
TRIE_C::const_iterator TRIE_C_DEC::begin() {
  return TRIE_C::const_iterator(this->root);
}
TRIE_T_DEC
TRIE_C::const_iterator TRIE_C_DEC::end() {
  return this->_end;
}

#undef TRIE_T_DEC
#undef TRIE_C_DEC
#undef TRIE_C
#endif
