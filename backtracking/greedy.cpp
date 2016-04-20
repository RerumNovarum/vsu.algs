#include <iostream>
#include <fstream>
#include <vector>

#include "points_covering.hpp"

using namespace std;

template<typename T>
void read_pts(vector< pt<T> > &pts, istream &in) {
  T a, b;
  while(in >> a >> b) {
    pts.push_back(pt<T>(a, b));
  }
}

int main(int argc, char *argv[]) {
  vector< pt<int> > pts;
  if (argc == 1) read_pts(pts, cin);
  else {
    for (int i = 0; i < argc; ++i) {
      string fname = argv[i];
      if (fname == "-") read_pts(pts, cin);
      else {
        ifstream f;
        f.open(fname);
        read_pts(pts, f);
        f.close();
      }
    }
  }
  vector< ln<int> > opts = min_cover_next(pts);
  for (auto it = opts.begin(); it != opts.end(); ++it) {
    // cout << it->a << "; " << it->b << endl;
    cout << it->a.x << " " << it->a.y\
         << "; " << it->b.x << " " << it->b.y << endl;
  }
}
