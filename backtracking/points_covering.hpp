#ifndef POINTS_COVERING
#define POINTS_COVERING

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
  pt<T>& operator+(pt<T> &p) {
    pt<T> result(this->x + p.x, this->y + p.y);
    return result;
  }
  pt<T>& operator-() {
    pt<T> result(-this->x, -this->y);
    return result;
  }
  pt<T> operator-(pt<T> p) {
    pt<T> result(this->x - p.x, this->y - p.y);
    return result;
  }
  ostream& operator<< (ostream &out) {
    return out << this->x << " " << this->y;
  }
  bool operator== (pt<T> p) {
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
  b -= a;
  c -= a;
  return sig_vol(b, c) > 0;
}

template<typename T>
struct pt_polar_lt {
  pt<T> origin;
  pt_polar_lt(pt<T> o) : origin(o) {}
  bool operator()(pt<T> a, pt<T> b) {
    // return ccw(origin, a, b);
    if (a == origin) return true;
    if (b == origin) return false;
    T v = sig_vol(a - origin, b - origin);
    if (v > 0) return true;
    if (v > 0) return false;
    if (a.y < b.y) return true;
    if (b.y < a.y) return false;
    if (a.x < b.x) return true;
    return false;
  }
};

template<typename T>
struct ln {
  pt<T> a, b;
  ln() {}
  ln(pt<T> p_1, pt<T> p_2) :
    a(p_1),
    b(p_2)
  {}
};

template<typename T>
vector< ln<T> > min_cover_next(vector< pt<T> > in_pts) {
  vector< pt<T> > pts(in_pts);
  vector< ln<T> > lines;
  int n = pts.size();
  int sup = 0;
  for (auto it = in_pts.begin(); it != in_pts.end(); ++it) {
    pt_polar_lt<T> lt(*it);
    std::sort(pts.begin(), pts.end(), lt);
    int i=0, j=0;
    while (i < n) {
      j = i + 1;
      while (j < n && collinear(pts[i], pts[j-1], pts[j])) ++j;
      --j;
      if (j - i + 1 >= sup) {
        if (j - i + 1 > sup) lines.clear();
        pt_xy_lt<T> lt;
        std::sort(pts.begin() + i, pts.begin() + j, lt);
        lines.push_back(ln<T>(pts[i], pts[i+1]));
        sup = j - i + 1;
      }
      i = j + 1;
    }
  }
  return lines;
}

template<typename T>
vector< ln<T> > min_cover(vector< pt<T> > in_pts) {
  vector< ln<T> > covering;
  if (in_pts.size() == 0) {
    return covering;
  }
  else if (in_pts.size() == 1) {
    ln<T> l;
    l.a = in_pts[0];
    covering.push_back(l);
    return covering;
  } else if (in_pts.size() == 2) {
    covering.push_back(ln<T>(in_pts[0], in_pts[1]));
    return covering;
  }
  ln<T> cur;
  int n = in_pts.size();
  int inf = n+1;
  vector< ln<T> > lines = min_cover_next(in_pts);
  for (auto it = lines.begin(); it != lines.end(); ++it) {
    ln<T> l = *it;
    vector< pt<T> > pts;
    for (auto p = in_pts.begin(); p != in_pts.end(); ++p) {
      if (!collinear(l.a, l.b, *p))
        pts.push_back(*p);
    }
//    for (auto j = pts.begin(); j != pts.end(); ++j)
//      cout << j->x << " " << j->y << ", ";
//    cout << endl;
    vector< ln<T> > opt = min_cover(pts);
    if (opt.size() < inf) {
      covering = opt;
      cur = l;
    }
  }
  covering.push_back(cur);
  return covering;
}
#endif
