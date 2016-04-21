#ifndef POINTS_COVERING
#define POINTS_COVERING

#include <set>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

template<typename T>
struct pt {
  T x, y;

  pt() : x(0), y(0) {}
  pt(T x, T y) {
    this->x = x;
    this->y = y;
  }

  pt<T>& operator+=(const pt<T> &p) {
    this->x += p.x;
    this->y += p.y;
    return *this;
  }
  pt<T>& operator-=(const pt<T> &p) {
    this->x -= p.x;
    this->y -= p.y;
    return *this;
  }
  pt<T>& operator+(pt<T> &p) const {
    pt<T> result(this->x + p.x, this->y + p.y);
    return result;
  }
  pt<T>& operator-() const {
    pt<T> result(-this->x, -this->y);
    return result;
  }
  pt<T> operator-(const pt<T> p) const {
    pt<T> result(this->x - p.x, this->y - p.y);
    return result;
  }
  friend ostream& operator<< (ostream &out, pt<T> p) {
    return out << "(" << p.x << "," << p.y << ")";
  }
  bool operator== (const pt<T> &p) const {
    return this->x == p.x && this->y == p.y;
  }
};

template<typename T>
struct pt_xy_lt {
  bool operator() (pt<T> a, pt<T> b) {
    if (a.x == b.x) return a.y < b.y;
    return a.x < b.x;
  }
};

template<typename T>
T sig_vol(pt<T> a, pt<T> b) {
  return a.x*b.y - a.y*b.x;
}

template<typename T>
bool collinear(pt<T> a, pt<T> b, pt<T> c) {
  return sig_vol(b-a, c-a) == 0;
}

template<typename T>
bool ccw(pt<T> a, pt<T> b, pt<T> c) {
  return 0 < sig_vol(b - a, c - a);
}

template<typename T>
struct pt_polar_lt {
  pt<T> origin;
  pt_polar_lt(pt<T> o) : origin(o) {}
  
  bool lt(const pt<T>& a, const pt<T>& b) const {
    if (a == origin) return true;
    if (b == origin) return false;
    T v = sig_vol(a - origin, b - origin);
    if (v != 0) return v > 0;
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
  }
  bool operator()(const pt<T>& a, const pt<T>& b) const {
    bool l = this->lt(a, b);
    // cout << a << (l ? "<" : ">") << b << endl;
    return l;

    if (a == origin) return true;
    if (b == origin) return false;
    T v = sig_vol(a - origin, b - origin);
    if (v != 0) return v > 0;
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
  }
};

template<typename T>
struct ln {
  pt<T> a, b;
  ln() : a(0, 0), b(0, 0) {}
  ln(pt<T> p_1, pt<T> p_2) :
    a(p_1),
    b(p_2)
  {}
  bool operator< (const ln<T> &other) const {
    if (this==nullptr) return false;
    if (this->a.x != other.a.x)
      return this->a.x < other.a.x;
    if (this->b.x != other.b.x)
      return this->b.x < other.b.x;
    if (this->a.y != other.a.y)
      return this->a.y < other.a.y;
    return this->b.y < other.b.y;
  }
  friend ostream& operator<<(ostream& out, const ln<T> &l) {
    out << "(" << l.a << "," << l.b <<")";
  }
};

// greedily choose lines
// that cover number of points
// equal to largest possible
//
// algorithm:
// let P be set of input points
//
//  $\forall p\in P$
//    sort $P$ by polar angle regarding $p$
//    now collinear points are located sequantially
//    find the max in linear time
//
// time: \Theta(n*(n*log(n) + 2*n))
// space: O(n)
template<typename T>
set< ln<T> > min_cover_next(const vector<pt<T> >& in_pts) {
  vector<pt<T> > pts(in_pts.size());
  copy(in_pts.begin(), in_pts.end(), pts.begin());
  set<ln<T> > lines;
  int n = pts.size();
  int sup = 0;
  for (auto it = in_pts.begin(); it != in_pts.end(); ++it) {
    pt_polar_lt<T> lt(*it);
    // cout << "pv=" << *it << endl;
    // for_each(pts.begin(), pts.end(), [](pt<T> p){ cout << p << endl; });
    std::sort(pts.begin(), pts.end(), lt);
    int i=0, j=0;
    while (i < n) {
      j = i + 1;
      // cout << "j=" << j << endl;
      while (j < n && collinear(pts[i], pts[j-1], pts[j])) ++j;
      --j;
      if (j - i + 1 >= sup) {
        if (j - i + 1 > sup) lines.clear();
        pt_xy_lt<T> lt;
        std::sort(pts.begin() + i, pts.begin() + j, lt);
        lines.insert(ln<T>(pts[i], pts[i+1]));
        sup = j - i + 1;
      }
      i = j + 1;
    }
  }
  return lines;
}

// finds minimal line-covering of in_pts
// algorithm: bruteforce with a few cut-offs
template<typename T>
vector< ln<T> >* min_cover(const vector< pt<T> > &in_pts) {
  vector<ln<T> >* covering = nullptr;
  // boundary cases
  if (in_pts.size() <= 2) {
    covering = new vector<ln<T> >;
    if (in_pts.size() == 0) {
      return covering;
    }
    else if (in_pts.size() == 1) {
      ln<T> l;
      l.a = in_pts[0];
      covering->push_back(l);
    } else if (in_pts.size() == 2) {
      covering->push_back(ln<T>(in_pts[0], in_pts[1]));
    }
    return covering;
  }
  ln<T> cur;
  int n = in_pts.size();
  int inf = n+1;
  set< ln<T> > lines = min_cover_next(in_pts);
  if (lines.size() >= n/2) {
    // i.e. there's no collinear points
    // so it doesn't matter
    // which line to choose
    // TODO: cut off recursion in this case
    auto l = *lines.begin();
    lines.clear();
    lines.insert(l);
  }
  for (auto it = lines.begin(); it != lines.end(); ++it) {
    ln<T> l = *it;
    vector<pt<T> > pts;
    for (auto p = in_pts.begin(); p != in_pts.end(); ++p) {
      if (!collinear(l.a, l.b, *p))
        pts.push_back(*p);
    }
    vector< ln<T> > *opt = min_cover(pts);
    if (opt->size() < inf) {
      if (covering != nullptr) delete covering;
      covering = opt;
      cur = l;
    }
  }
  if (covering == nullptr) covering = new vector<ln<T> >();
  covering->push_back(cur);
  return covering;
}
#endif
