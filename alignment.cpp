#include "alignment.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
using namespace std;

Alignment::Alignment(const string &T1, const string &T2, const int m1, const int x1, const int o1)
{
  S1 = T1;
  S2 = T2;
  N = S1.size() + 1;
  M = S2.size() + 1;
  m = m1;
  x = x1;
  o = o1;
}

NW_linear::NW_linear(const string &T1, const string &T2, const int m1,const int x1,const int o1): Alignment(T1, T2, m1, x1, o1)
{
  DP.resize(N, vector <int> (M,0));
  TRACE.resize(N, vector <int> (M,-1));
}

void  NW_linear::calculate_DP_matrix()
{
  DP[0][0] = 0;
  for(int i = 1; i < N; i++)
    {
      DP[i][0] = i * o;
      TRACE[i][0] = 1;
    }
  
  for(int j = 1; j < M; ++j)
    {
      DP[0][j] = j * o;
      TRACE[0][j] = 2;
    }

  for(int i = 1; i < N; i++)
    {
      for(int j = 1; j < M; j++)
	{
	  max(i, j);
	}
    }
}

void NW_linear::traceback(string &T1, string &T2)
{
  if( ! T1.empty() )
    {
      T1.erase(T1.begin(), T1.end() );
    }
  if( ! T2.empty())
    {
      T2.erase(T2.begin(), T2.end() );
    }//与えられた配列が空であることの確認。空でないなら空にする。
  //  cout << "input strings are empty" <<endl;
  int i,j;
  i = 1;
  j = 1;
  while(TRACE[N-i][M-j] >= 0)
    {
      switch(TRACE[N-i][M-j])
	{
	case 0:
	  {
	    T1 += S1[N-i-1];
	    T2 += S2[M-j-1];
	    i++; 
	    j++;
	    break;
	  }
	case 1: 
	  {
	    T1 += S1[N-i-1];
	    T2 += "-"; 
	    i++;
	    break;
	  }
	case 2: 
	  {
	    T1 += "-";
	    T2 += S2[M-j-1];
	    j++;
	    break;
	  }
	default: cerr << "error" << endl; break;
	}
    } 
  reverse(T1.begin(), T1.end() );
  reverse(T2.begin(), T2.end() );
}

void NW_linear::max(const int i,const int j)
{
  int sco = DP[i-1][j-1];
  int dir = 0; 
  if( S1[i-1] == S2[j-1])
    {
      sco += m;
    }
  else
    {
      sco += x;
    }

  if( DP[i-1][j] + o > sco)
    {
      sco = DP[i-1][j] + o;
      dir = 1;
    }

  if( DP[i][j-1] + o > sco)
    {
      sco = DP[i][j-1] + o;
      dir = 2;
    }

  TRACE[i][j] = dir;
  DP[i][j] = sco;
}

void NW_linear::show()
{
  for(int i = 0; i < N; ++i)
    {
      for(int j = 0; j <M; ++j)
	{
	  cout << setw(3) << DP[i][j];
	  cout << " ";
	}
      cout << endl;
    }
  cout << endl;
  for(int i = 0; i < N; ++i)
    {
      for(int j = 0; j <M; ++j)
	{
	  cout << setw(3) << TRACE[i][j];
	  cout << " ";
	}
      cout << endl;
    }
}

NW_affine::NW_affine(const string &T1, const string &T2,const int m1,const int x1, const int o1, const int e1): Alignment(T1, T2, m1, x1, o1)
{
  e = e1;
  DP.resize(3, vector < vector <int> > (N, vector <int> (M, -100) ) );
  TRACE.resize(3, vector < vector <int> > (N, vector <int> (M, -1) ) );
}

void NW_affine::calculate_DP_matrix()
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

void NW_affine::traceback(string &A1, string &A2)
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

void NW_affine::max(int j, int k)
{
  int sco, dir;
  if(DP[1][j-1][k-1] > DP[0][j-1][k-1] )
    {
      dir = 1;
      sco = DP[1][j-1][k-1]; 
    }
  else
    {
      dir = 0;
      sco =  DP[0][j-1][k-1];
    }
  if( DP[2][j-1][k-1] > sco) 
    {
      dir = 2;
      sco = DP[2][j-1][k-1];
    }
 
  if( S1[j - 1] == S2[k - 1] )
    {
      DP[0][j][k] = sco + m;
      TRACE[0][j][k] = dir;
    }
  else
    {
      DP[0][j][k] = sco + x;
      TRACE[0][j][k] = dir;
    }
  // calculation for diagonal

  if( DP[1][j-1][k] + e > DP[0][j-1][k] + o )
    {
      sco = DP[1][j-1][k] + e ;
      dir = 1;
    }
  else
    {
      dir = 0;
      sco = DP[0][j-1][k] +o;
    }
  if(DP[2][j-1][k] + o > sco)
    {
      DP[1][j][k] = DP[2][j-1][k] + o;
      TRACE[1][j][k] = 2;
    }
  else
    {
      DP[1][j][k] = sco;
      TRACE[1][j][k] = dir;
    }
  // calculation for vertical

  if( DP[2][j][k-1] + e > DP[0][j][k-1] + o)
    {
      sco = DP[2][j][k-1] + e ;
      dir = 2;
    }
  else
    {
      sco = DP[0][j][k-1] + o;
      dir = 0;
    }

  if( DP[1][j][k-1] + o > sco )
    {
      DP[2][j][k] = DP[1][j][k-1] + o;
      TRACE[2][j][k] = 1;
    }
  else
    {
      DP[2][j][k] = sco;
      TRACE[2][j][k] = dir;
    }
  // calculation for horizontal
}

void NW_affine::show()
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

SW_linear::SW_linear(const string &T1, const string &T2, const int m1, const int x1,const int o1): NW_linear(T1, T2, m1, x1, o1)
{
  ;
}

void  SW_linear::calculate_DP_matrix()
{
   for(int i = 1; i < N; i++)
    {
      for(int j = 1; j < M; j++)
	{
	  max(i, j);
	}
    }
}

void SW_linear::traceback(string &T1, string &T2)
{
  if( ! T1.empty() )
    {
      T1.erase(T1.begin(), T1.end() );
    }
  if( ! T2.empty())
    {
      T2.erase(T2.begin(), T2.end() );
    }//与えられた配列が空であることの確認。空でないなら空にする。
  //  cout << "input strings are empty" <<endl;
  int score = 0;
  int p = 4;
  int q = 5;
  for(int i = 0; i < N; i++)
    {
      for(int j = 0; j < M; j++)
	{
	  if( score < DP[i][j])
	    {
	      score = DP[i][j];
	      p = i;
	      q = j;
	    }
	}
    }


  cout << DP[p][q] << endl;
  
  while(1)
    {
      if(TRACE[p][q] == -1) break;
      switch(TRACE[p][q])
	{
	case 0:
	  {
	    T1 += S1[p-1];
	    T2 += S2[q-1];
	    p--;
	    q--;
	    break;
	  }
	case 1:
	  {
	    T1 += S1[p-1];
	    T2 += "-";
	    p--;
	    break;
	  }
	case 2:
	  {
	    T1 += "-";
	    T2 += S2[q-1];
	    q--;
	    break;
	  }
	default: cerr << "err" << endl; break;
	}
    }

  reverse(T1.begin(), T1.end() );
  reverse(T2.begin(), T2.end() );
}

void SW_linear::max(const int i,const int j)
{
  int sco = DP[i-1][j-1];
  int dir = 0; 
  if( S1[i-1] == S2[j-1])
    {
      sco += m;
    }
  else
    {
      sco += x;
    }

  if( DP[i-1][j] + o > sco)
    {
      sco = DP[i-1][j] + o;
      dir = 1;
    }

  if( DP[i][j-1] + o > sco)
    {
      sco = DP[i][j-1] + o;
      dir = 2;
    }

  if( sco < 0)
    {
      sco = 0;
      dir = -1;
    }

  TRACE[i][j] = dir;
  DP[i][j] = sco;
}

void SW_linear::show()
{
  for(int i = 0; i < N; ++i)
    {
      for(int j = 0; j <M; ++j)
	{
	  cout << setw(3) << DP[i][j];
	  cout << " ";
	}
      cout << endl;
    }
  cout << endl;
  for(int i = 0; i < N; ++i)
    {
      for(int j = 0; j <M; ++j)
	{
	  cout << setw(3) << TRACE[i][j];
	  cout << " ";
	}
      cout << endl;
    }
}

SW_affine::SW_affine(const string& T1, const string& T2, const int m1, const int x1, const int o1, const int e1): NW_affine(T1, T2, m1, x1, o1, e1)
{
  ;
}

void SW_affine::calulate_DP_matrix()
{
   for(int i = 0; i <3; i++)
    {
      DP[i][0][0] = 0;
    }
  for(int j = 1; j < N; j++)
    {
      DP[1][j][0] = 0;
      TRACE[1][j][0] = -1;
    }
  for(int k= 1; k < M; k++)
    {
      DP[2][0][k] = 0;
      TRACE[2][0][k] = -1;
    }
  //initialization
 for(int j = 1; j < N; j++)
    {
      for(int k = 1; k < M; k++)
	{
	  max(j, k);
	}
    }
}

void SW_affine::traceback(string &A1, string &A2)
{
  int p, q, r;
  p = 0;
  q = 0;
  r = 0;
  int max_score = 0;
  for(int i = 0; i < N; i++)
    {
      for(int j = 0; j < M; j++)
	{
	  if(DP[0][j][k] > max_score)
	    {
	      max_score = DP[0][i][j];
	      q = j;
	      r = k;
	    }
	}
    }

  while(p >= 0)
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

void SW_affine::max(const int j, const int k)
{
  int sco, dir;
  if(DP[1][j-1][k-1] > DP[0][j-1][k-1] )
    {
      dir = 1;
      sco = DP[1][j-1][k-1]; 
    }
  else
    {
      dir = 0;
      sco =  DP[0][j-1][k-1];
    }
  if( DP[2][j-1][k-1] > sco) 
    {
      dir = 2;
      sco = DP[2][j-1][k-1];
    }
  if( S1[j - 1] == S2[k - 1] )
    {
      sco += m;
    }
  else
    {
      sco += x;
    }
  if(sco < 0)
    {
      sco = 0;
      dir = -1;
    }
  DP[1][j][k] = sco;
  TRACE[1][j][k] = dir;
  // calculation for diagonal

  if( DP[1][j-1][k] + e > DP[0][j-1][k] + o )
    {
      sco = DP[1][j-1][k] + e ;
      dir = 1;
    }
  else
    {
      dir = 0;
      sco = DP[0][j-1][k] +o;
    }
  if(DP[2][j-1][k] + o > sco)
    {
      sco = DP[2][j-1][k] + o;
      dir = 2;
    }
  if(sco < 0)
    {
      sco = 0;
      dir = -1;
    }
  DP[1][j][k] = sco;
  TRACE[1][j][k] = dir;
  // calculation for vertical

  if( DP[1][j-1][k] + e > DP[0][j-1][k] + o )
    {
      sco = DP[1][j-1][k] + e ;
      dir = 1;
    }
  else
    {
      dir = 0;
      sco = DP[0][j-1][k] +o;
    }
  if(DP[2][j-1][k] + o > sco)
    {
      DP[1][j][k] = DP[2][j-1][k] + o;
      TRACE[1][j][k] = 2;
    }
  else
    {
      DP[1][j][k] = sco;
      TRACE[1][j][k] = dir;
    }
  // calculation for vertical

  if( DP[2][j][k-1] + e > DP[0][j][k-1] + o)
    {
      sco = DP[2][j][k-1] + e ;
      dir = 2;
    }
  else
    {
      sco = DP[0][j][k-1] + o;
      dir = 0;
    }

  if( DP[1][j][k-1] + o > sco )
    {
      sco = DP[1][j][k-1] + o;
      dir = 1;
    }
  if(sco < 0)
    {
      sco = 0;
      dir = -1;
    }
      DP[2][j][k] = sco;
      TRACE[2][j][k] = dir;
  // calculation for horizontal
}

void SW_affine::show()
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
