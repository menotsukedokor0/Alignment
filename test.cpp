#include <iostream>
#include "alignment.h"
#include <fstream>
using namespace std;

int main(int argc, char* argv[])
{
  
  Alignment* aln;
  int m;
  int x;
  int o;
  int e;
  string S1;
  string S2;

  ifstream ifs("input.txt");
  ifs >> S1;
  ifs >> S2;
  ifs >> m;
  ifs >> x;
  ifs >> o;
  ifs >> e;
  
  NW_linear Nl(S1, S2, m, x, o);
  NW_affine Na(S1, S2, m, x, o, e);
  SW_linear Sl(S1, S2, m, x, o);
  
  // cout << "argv[1] = " << argv[1] << endl;
  //cout << "argv[1] == \"-affine\"? ";
  //cout << boolalpha  << bool(string(argv[1]) == "-affine") << endl;
  if(argc == 1) goto ERR;
  if( string(argv[1]) == "-affine")
    {
      aln = &Na;
    }
  else 
    {
      if(string(argv[1]) == "-SW" )
	aln = &Sl;
      else
	{
	ERR:
	  aln = &Nl;
	}
    }
  cout << S1 << endl;
  cout << S2 << endl;
 
  string A1, A2;
  aln->calculate_DP_matrix();
  aln->show(); //DP行列とTRACE行列の確認
  aln->traceback(A1,A2);
  for(int i = 0; i < A1.size(); ++i)
    cout << "_";
  cout << endl;

  cout << A1 << endl;
  cout << A2 << endl;
  for(int i = 0; i < A1.size(); i++)
    {
      if(A1[i] == A2[i])
	cout << '*';
      else
	cout << " ";
    }
  cout << endl;

  return 0;
}
