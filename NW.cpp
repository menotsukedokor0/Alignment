#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
using namespace std;

class NW
{
  int N,M;
  int m, x, o;
  string S1, S2;
  vector < vector <int> > D,TRACE;
public:
  NW(const string &, const string &,const int m1,const int x1,const int o1);
  void linear();
  void traceback(string &, string &, string &);
  int max(const int i, const int j);
  void show();
};
//affine はこのクラスを継承すればいいのではないかな


void NW::show()
{
  for(int i = 0; i < N; ++i)
    {
      for(int j = 0; j <M; ++j)
	{
	  cout << setw(3) << D[i][j];
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
// DP行列とTRACE行列の確認用

NW::NW(const string &T1, const string &T2,const int m1,const int x1,const int o1)
{
  S1 = T1;
  S2 = T2;
  N = S1.size() + 1;
  M = S2.size() + 1;
  m = m1;
  x = x1;
  o = o1;
  D.resize(N, vector <int> (M,0));
  TRACE.resize(N, vector <int> (M,-1));
};
/*
int NW::max(const int i,const int j)
{
  int sco = 0;
  int drn= -1; 
  // 一時的な最大スコアとトレース方向
  // 0…対角, 1…上方向, 2 左方向
  int dig =0; 
  // マッチの一時スコア
  
  if(D[i][j-1] + o  > D[i-1][j] + o)
    {
      sco = D[i][j-1] + o;
      drn = 2; //同じスコアなら上のほうが優先度あり(insertion < deletion)
    }
  else
    {
      sco = D[i-1][j] + o;
      drn = 1;
    }

  if( S1[i-1] == S2[j-1] )  // linear()内の行列成分とstringのインデックスのズレを解消
    {
      dig = D[i-1][j-1] + m;
    }
  else
    {
      dig = D[i-1][j-1] + x;
    }
  // match or missmatch
  if( sco > dig )
    {
      TRACE[i][j] = drn;
      return sco;
    }
  else
    {
      TRACE[i][j] = 0;
      return dig;
    }
}
*/
int NW::max(const int i,const int j)
{
  int sco = 0;
  int drn = 0; 
  if( S1[i-1] == S2[j-1])
    {
      sco = D[i-1][j-1] + m;
    }
  else
    {
      sco = D[i-1][j-1] + x;
      drn = 3;
    }
  if( D[i-1][j] > sco)
    {
      sco = D[i-1][j] + o;
      drn = 1;
    }

  if( D[i][j-1] > sco)
    {
      sco = D[i][j-1] + o;
      drn = 2;
    }
  TRACE[i][j] = drn;
  return sco;
}

 
void  NW::linear()
{
  D[0][0] = 0;
  for(int i=1; i<N ; i++)
    {
      D[i][0] = i * o;
      TRACE[i][0] = 1;
    }
  
  for(int j = 1; j < M; ++j)
    {
      D[0][j] = j * o;
      TRACE[0][j] = 2;
    }

  for(int i = 1; i < N; i++)
    {
      for(int j = 1; j < M; j++)
	{
	  D[i][j] = max(i, j);
	}
    }
}

void NW::traceback(string &T1, string &T2, string &T3)
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
	case 0:  T1 += S1[N-i-1]; T2 += S2[M-j-1]; T3 += "*"; i++; j++; break;
	case 1:  T1 += S1[N-i-1]; T2 += "-"; T3 += " "; i++; break;
	case 2:  T1 += "-"; T2 += S2[M-j-1]; T3 += " "; j++; break;
	case 3:  T1 += S1[N-i-1]; T2 += S2[M-j-1]; T3 += " "; i++; j++; break;
	default: cout << "error" << endl; break;
	}
      
    } 
  reverse(T1.begin(), T1.end() );
  reverse(T2.begin(), T2.end() );
  reverse(T3.begin(), T3.end() );
}



int main()
{
  string S1, S2;
  S1 = "GATTA";
  S2 = "GAATTC";
  cout << S1 << endl;
  cout << S2 << endl;
  NW aln(S1, S2, 2, -1, -2);
  string A1,A2,A3;
  aln.linear();
  aln.show(); //DP行列とTRACE行列の確認
  aln.traceback(A1,A2,A3);
  for(int i = 0; i < A1.size(); ++i)
    cout << "_";
  cout << endl;

  cout << A1 << endl;
  cout << A2 << endl;
  cout << A3 << endl; 
  return 0;
}
