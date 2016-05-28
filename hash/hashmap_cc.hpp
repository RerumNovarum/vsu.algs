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
  typename hash_t=std::hash<K>>
class hashmap_cc {
  private:
    int n,N;
    vector<list<pair<K, V> > > table;
    hash_t hash;
  public:
    typedef const K& key_const_ref;
    typedef const V& val_const_ref;
    typedef K& key_ref;
    typedef V& val_ref;
    typedef list<pair<K, V> >& chain_ref;
    typedef const list<pair<K, V> >& const_chain_ref;

    hashmap_cc(size_t N=1);
    void put(key_const_ref k, V v);
    bool has(key_const_ref k);
    V get(key_const_ref k);
    val_const_ref operator[](key_const_ref k) const;
    val_ref operator[](key_const_ref k);
  private:
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

#undef HASH_C_DEC
#undef HASH_T_DEC

#endif
