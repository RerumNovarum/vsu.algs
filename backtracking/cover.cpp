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
  vector<pt<int> > pts;
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
  vector<ln<int> > *covering = min_cover(pts);
  for (auto it = covering->begin(); it != covering->end(); ++it) {
    cout << *it << endl;
  }
}
