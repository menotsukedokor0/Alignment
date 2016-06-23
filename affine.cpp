#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <climits>
using namespace std;

class affine_gap
{
  int N,M;
  int m, x, o, e;
  string S1, S2;
  vector < vector < vector <int> > > DP;
  vector < vector < vector <int> > > TRACE;
public:
  affine_gap(const string &, const string &, const int m1, const int x1, const int o1, const int e1);
  void calculate_matrix();
  void traceback(string &, string &, string &);
  void max(const int j, const int k);
  void show();
};

void affine_gap::show()
{
  for(int i = 0; i < 3; ++i)
    {
      switch(i)
	{
	case 0: cout << "diagonal" << endl; break;
	case 1: cout << "vertical" << endl; break;
	case 2: cout << "horizontal" << endl; break;
	}
      for(int j = 0; j <N; ++j)
	{
	  for(int k = 0; k < M; ++k)
	    {
	      cout << setw(3) << DP[i][j][k];
	      cout << " ";
	    }
	  cout << endl;
	}
      cout << endl;
    }
  cout << endl;

  /*
  for(int i = 0; i < 3; ++i)
    {
      switch(i)
	{
	case 0: cout << "diagonal" << endl; break;
	case 1: cout << "vertical" << endl; break;
	case 2: cout << "horizontal" << endl; break;
	}
      for(int j = 0; j <N; ++j)
	{
	  for(int k = 0; k < M; k++)
	    {
	      cout << setw(3) << TRACE[i][j][k];
	      cout << " ";
	    }
	  cout << endl;
	}
      cout << endl;
    }
  */
}

affine_gap::affine_gap(const string &T1, const string &T2,const int m1,const int x1, const int o1, const int e1)
{
  S1 = T1;
  S2 = T2;
  N = S1.size() + 1;
  M = S2.size() + 1;
  m = m1;
  x = x1;
  o = o1;
  e = e1;
  DP.resize(3, vector < vector <int> > (N, vector <int> (M, 0) ) );
  TRACE.resize(3, vector < vector <int> > (N, vector <int> (M,-1) ) );
}

void affine_gap::calculate_matrix()
{
  for(int i = 0; i <3; i++)
    {
      DP[i][0][0] = 0;
    }
  for(int j = 1; j < N; j++)
    {
      DP[1][j][0] = o + (j - 1) * e;
    }
  for(int k= 1; k < M; k++)
    {
      DP[2][0][k] = o + (k - 1) * e;
    }

  for(int j = 1; j < N; j++)
    {
      for(int k = 1; k < M; k++)
	{
	  max(j, k);
	}
    }
}

void affine_gap::max(int j, int k)
{
  int tmp = DP[1][j-1][k-1] > DP[0][j-1][k-1] ? DP[1][j-1][k-1] : DP[0][j-1][k-1];
  tmp = DP[2][j-1][k-1] > tmp ?  DP[2][j-1][k-1] : tmp;

  if( S1[j - 1] == S2[k - 1] )
    {
      DP[0][j][k] = tmp + m;
    }
  else
    {
      DP[0][j][k] = tmp + x;
    }

  DP[1][j][k] = DP[1][j-1][k] + e > DP[0][j-1][k] + o ?  DP[1][j-1][k] + e : DP[0][j-1][k] +o;

  DP[2][j][k] =  DP[2][j][k-1] + e > DP[0][j][k-1] + o ?  DP[2][j][k-1] + e : DP[0][j][k-1] + o;

}


int main(){

  string S1, S2;
  S1 = "GATTA";
  S2 = "GAATTC";
  cout << S1 << endl;
  cout << S2 << endl;
  affine_gap aln(S1, S2, 2, -1, -6, -2);
  aln.calculate_matrix();
  aln.show();

  return 0;
}
