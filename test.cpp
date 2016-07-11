#include <iostream>
#include "alignment.h"
#include <iostream>
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
  
  if(argv[1] == "-affine")
    aln = &Nl;
  else
    aln = &Na;

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
