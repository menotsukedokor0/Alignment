#ifndef ___ALIGNMNET___
#define ___ALIGNMENT___

#include <vector>
#include <string>
using namespace std;

class Alignment{
 protected:
  string S1, S2;
  int N, M;
  int m;
  int x;
  int o;
 public:
  Alignment(const string &, const string &, const int m1, const int x1, const int o1);
  virtual void calculate_DP_matrix() = 0;
  virtual void traceback(string&, string&) = 0;
  virtual void max(const int, const int) = 0;
  virtual void show() = 0; //デバック用関数
};

class NW_linear: public Alignment{
 protected:
  vector < vector <int> > DP, TRACE;
 public:
  NW_linear(const string&, const string&, const int m1, const int x1, const int o1);
  void calculate_DP_matrix();
  void traceback(string&, string&);
  void max(const int, const int);
  void show();
};

class NW_affine: public Alignment{
 protected:
  int e;
  vector < vector <vector <int> > > DP, TRACE;
 public:
  NW_affine(const string&, const string&, const int m1, const int x1, const int o1, const int e1);
  void calculate_DP_matrix();
  void traceback(string&, string&);
  void max(const int, const int);
  void show();
};

class SW_linear: public NW_linear{
 public:
  SW_linear(const string&, const string&, const int m1, const int x1, const int o1);
  void calculate_DP_matrix();
  void traceback(string&, string&);
  void max(const int, const int);
  void show();
};

#endif
