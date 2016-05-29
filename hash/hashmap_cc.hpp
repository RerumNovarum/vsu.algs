#ifndef HASHMAP_CC
#define HASHMAP_CC

#include <functional>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <list>

using std::vector;
using std::list;
using std::pair;
using std::make_pair;
using std::find_if;
using std::out_of_range;

template<
  typename K,
  typename V,
  typename hash_t=std::hash<K> >
class hashmap_cc {
  public:
    typedef const K& key_const_ref;
    typedef const V& val_const_ref;
    typedef K& key_ref;
    typedef V& val_ref;
    typedef list<pair<K, V> > chain_t;
    typedef chain_t& chain_ref;
    typedef const chain_t& const_chain_ref;
    class const_iterator;

    hashmap_cc(size_t N=1);
    void put(key_const_ref k, V v);
    bool has(key_const_ref k);
    V get(key_const_ref k);
    val_const_ref operator[](key_const_ref k) const;
    val_ref operator[](key_const_ref k);
    const_iterator begin();
    const_iterator end();
  private:
    int n,N;
    vector<chain_t> table;
    hash_t hash;
    const_iterator _end;
    void resize(size_t N);
    chain_ref get_chain(key_const_ref k);
};

#define HASH_T_DEC \
  template< \
    typename K, \
    typename V, \
    typename hash_t>
#define HASH_C_DEC \
  hashmap_cc<K, V, hash_t>
#define HASH_C \
  typename HASH_C_DEC

HASH_T_DEC
void HASH_C_DEC::resize(size_t N) {
  auto old = this->table;
  this->table.clear();
  this->table.resize(N);
  for (auto cit = old.begin(); cit != old.end(); ++cit)
    for (auto it = cit->begin(); it != cit->end(); ++it)
      this->put(it->first, it->second);
  this->N = N;
}

HASH_T_DEC
HASH_C_DEC::hashmap_cc(size_t N) {
  this->n=0;
  this->resize(N);
}

HASH_T_DEC
HASH_C::chain_ref HASH_C_DEC::get_chain(HASH_C::key_const_ref k) {
  size_t h = this->hash(k);
  int i = h % this->N;
  return this->table[i];
}

HASH_T_DEC
bool HASH_C_DEC::has(HASH_C::key_const_ref k) {
  auto& chain = get_chain(k);
  auto it = find_if(chain.begin(), chain.end(), [k](pair<K, V> p) { return p.first == k; });
  return it != chain.end();
}

HASH_T_DEC
HASH_C::val_const_ref HASH_C_DEC::operator[](HASH_C::key_const_ref k) const {
  auto& chain = get_chain(k);
  auto it = find_if(chain.begin(), chain.end(), [k](pair<K, V> p) { return p.first == k; });
  if (it == chain.end()) throw out_of_range();
  return it->second;
}

HASH_T_DEC
V HASH_C_DEC::get(HASH_C::key_const_ref k) {
  return (*this)[k];
}

HASH_T_DEC
HASH_C::val_ref HASH_C_DEC::operator[](HASH_C::key_const_ref k) { 
  auto& chain = get_chain(k);
  auto it = find_if(chain.begin(), chain.end(), [k](pair<K, V> p) { return p.first == k; });
  if (it == chain.end()) {
    if (2*n > N) {
      resize(1 + 4*n);
    }
    auto& chain = get_chain(k);
    V zero;
    chain.push_back(make_pair(k, zero));
    n += 1;
    return chain.rbegin()->second;
  }
  return it->second;
}

HASH_T_DEC
void HASH_C_DEC::put(HASH_C::key_const_ref k, V v) {
  (*this)[k] = v;
}

HASH_T_DEC
struct HASH_C_DEC::const_iterator { 
  typedef pair<K, V> value_type;
  typedef typename vector<HASH_C::chain_t>::const_iterator chain_iter;
  typedef HASH_C::chain_t::const_iterator item_iter;
  
  bool exhausted;
  chain_iter cit;
  chain_iter cend;
  item_iter iit;
  item_iter iend;

  const_iterator() : exhausted(true) { }
  const_iterator(chain_iter begin, chain_iter end) : exhausted(false) {
    cit = begin;
    cend = end;
    ++(*this);
    if (cit == end) exhausted = true;
    else {
      iit = cit->begin();
      iend = cit->end();
      if (iit == iend) exhausted = true;
    }
  }

  const_iterator& operator++() {
    if (exhausted) throw out_of_range("iterator underflow");
    if(iit != iend)
      ++iit;
    while (!exhausted && iit == iend) {
      ++cit;
      if (cit == cend) exhausted = true;
      iit = cit->begin();
      iend = cit->end();
    }
    if (cit == cend) exhausted = true;
    return *this;
  }
  value_type operator*() {
    if (exhausted) throw out_of_range("iterator underflow");
    return *iit;
  }

  bool operator==(const const_iterator& rhs) const {
    bool e1 = this->exhausted;
    bool e2 = rhs.exhausted;
    if (e1 | e2) return e1 & e2;
    return this->cit == rhs.cit && this->iit == rhs.iit;
  }

  bool operator!=(const const_iterator& rhs) const {
    return !((*this) == rhs);
  }
};

HASH_T_DEC
HASH_C::const_iterator HASH_C_DEC::begin() {
  return const_iterator(this->table.begin(), this->table.end());
}

HASH_T_DEC
HASH_C::const_iterator HASH_C_DEC::end() {
  return this->_end;
}

#undef HASH_C_DEC
#undef HASH_T_DEC
#undef HASH_C

#endif
