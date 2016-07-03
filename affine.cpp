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
  void traceback(string &, string &);
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
	      cout << setw(5) << DP[i][j][k];
	      cout << " ";
	    }
	  cout << endl;
	}
      cout << endl;
    }
  cout << endl;

  
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
	      cout << setw(5) << TRACE[i][j][k];
	      cout << " ";
	    }
	  cout << endl;
	}
      cout << endl;
    }
  
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
  DP.resize(3, vector < vector <int> > (N, vector <int> (M, -100) ) );
  TRACE.resize(3, vector < vector <int> > (N, vector <int> (M, -1) ) );
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
      TRACE[1][j][0] = 1;
    }
  for(int k= 1; k < M; k++)
    {
      DP[2][0][k] = o + (k - 1) * e;
      TRACE[2][0][k] = 2;
    }
  //initialization has been completed

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
  int tmp, dir;
  if(DP[1][j-1][k-1] > DP[0][j-1][k-1] )
    {
      dir = 1;
      tmp = DP[1][j-1][k-1]; 
    }
  else
    {
      dir = 0;
      tmp =  DP[0][j-1][k-1];
    }
  if( DP[2][j-1][k-1] > tmp) 
    {
      dir = 2;
      tmp = DP[2][j-1][k-1];
    }
 

  if( S1[j - 1] == S2[k - 1] )
    {
      DP[0][j][k] = tmp + m;
      TRACE[0][j][k] = dir;
    }
  else
    {
      DP[0][j][k] = tmp + x;
	TRACE[0][j][k] = dir;
    }
  // calculation for diagonal

  if( DP[1][j-1][k] + e > DP[0][j-1][k] + o )
    {
      tmp = DP[1][j-1][k] + e ;
      dir = 1;
    }
  else
    {
      dir = 0;
      tmp = DP[0][j-1][k] +o;
    }
  if(DP[2][j-1][k] + o > tmp)
    {
      DP[1][j][k] = DP[2][j-1][k] + o;
      TRACE[1][j][k] = 2;
    }
  else
    {
      DP[1][j][k] = tmp;
      TRACE[1][j][k] = dir;
    }
  // calculation for vertical

  if( DP[2][j][k-1] + e > DP[0][j][k-1] + o)
    {
      tmp = DP[2][j][k-1] + e ;
      dir = 2;
    }
  else
    {
      tmp = DP[0][j][k-1] + o;
      dir = 0;
    }

  if( DP[1][j][k-1] + o > tmp )
    {
      DP[2][j][k] = DP[1][j][k-1] + o;
      TRACE[2][j][k] = 1;
    }
  else
    {
      DP[2][j][k] = tmp;
      TRACE[2][j][k] = dir;
    }
  // calculation for horizontal
}


void affine_gap::traceback(string &A1, string &A2)
{
  int p, q, r;
  p = 0;
  q = N -1;
  r = M -1;
  while(q > 0 && r > 0)
    {
      switch(p){
      case 0:
	{
	  p = TRACE[p][q][r];
	  q--;
	  r--;
	  A1 += S1[q];
	  A2 += S2[r];
	  break;
	}
      case 1:
	{
	  p = TRACE[p][q][r];
	  q--;
	  A1 += S1[q];
	  A2 += '-';
	  break;
	}
      case 2:
	{
	  p = TRACE[p][q][r];
	  r--;
	  A1 += '-';
	  A2 += S2[r];
	  break;
	}
      default: cerr << "An error has occurred" << endl;
      }
    }
  reverse(A1.begin(), A1.end() );
  reverse(A2.begin(), A2.end() );
  
}

int main(){

  string S1, S2;
  S1 = "GATTA";
  S2 = "GAATTC";
  cout << S1 << endl;
  cout << S2 << endl;
  affine_gap aln(S1, S2, 2, -1, -2, -2);
  aln.calculate_matrix();
  aln.show();
  string A1, A2, A3;
  aln.traceback(A1, A2);
  cout << A1 << endl;
  cout << A2 << endl;
  for(int i = 0; i < A1.size(); i++)
    {
      if(A1[i] == A2[i] )
	cout << '*';
      else
	cout << ' ';
    }
  cout << endl;

  return 0;
}
