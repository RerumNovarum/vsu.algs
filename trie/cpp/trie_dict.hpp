#ifndef _TRIE_DICT
#define _TRIE_DICT

#include <stdexcept>
#include <cassert>
#include <map>

#define TRIE_DEC \
  template< \
    typename K = std::string, \
    typename V = null_type>

struct null_type {};

TRIE_DEC
struct _trie_dict_node {
  typedef std::iterator_traits<typename K::iterator> atraits;
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
    typedef std::iterator_traits<typename K::iterator> atraits;
    typedef typename atraits::value_type char_type;
    typedef typename K::iterator   char_iterator;
    typedef typename K::const_iterator char_const_iterator;

    typedef V& val_ref;
    typedef const val_ref val_const_ref;
    typedef K& key_ref;
    typedef const key_ref key_const_ref;

    typedef _trie_dict_node<K,V> node;
    typedef node*                node_ptr;

    class iterator;
    class const_iterator;
    
    int size();
    bool has(key_const_ref k);
    void put(key_const_ref k, V v);
    val_ref at(key_const_ref k);

    val_const_ref operator[](key_const_ref k) const;
    val_ref       operator[](key_const_ref k);

    iterator begin();
    const_iterator end();

  private:
    int count;
    node_ptr root = nullptr;

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
  int j = 0;
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
struct TRIE_C_DEC::iterator {
  TRIE_C::node_ptr current_node;
  K key;

  // (this->current_node == nullptr) \iff (*this == end)
  iterator(TRIE_C::node_ptr root = nullptr) : current_node(root) {}

  std::pair<K, V> operator*() {
    return std::make_pair<K, V>(key, current_node->v);
  }
  
  TRIE_C::iterator& operator++() {
    TRIE_C::node_ptr t = current_node;
    if (t == nullptr) throw std::out_of_range("underflow while iterating");
    if (key.begin() == key.end()) {
      t = t->next.begin()->second;
    } else {
      do {
        TRIE_C::char_type c = key.back;
        auto succ = t->next.upper_bound(c);
        if (succ == t->next.end()) {
          t = t->prev; 
          key.pop_back();
        } else {
          t = succ->second;
          key.push_back(succ->first);
          if (t->word) break;
        }
      } while(t != nullptr);
    }
    current_node = t;
  }
};

#undef TRIE_T_DEC
#undef TRIE_C_DEC
#undef TRIE_C
#endif
