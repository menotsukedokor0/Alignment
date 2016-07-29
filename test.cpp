#include <iostream>
#include "alignment.h"
#include <fstream>
#include <typeinfo>
using namespace std;

int main(int argc, char* argv[])
{
  if(argc > 3)
    {
      cerr << "the number of option must be less than 2" << endl;
      return 1;
    }

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

  SW_linear sl(S1, S2, m, x, o);
  SW_affine sa(S1, S2, m, x, o, e);
  NW_linear nl(S1, S2, m, x, o);
  NW_affine na(S1, S2, m, x, o, e);
  
  int option[2] = {0, 0};
  for(int i = 0; i < argc; i++)
    {
      if(string(argv[i]) == "-local")
	{
	  option[0] = 1;
	}
      if(string(argv[i]) == "-affine")
	{
	  option[1] = 1;
	}
    }
  
  if(option[0])
    {
      if(option[1])
	{
	  aln = &sa;
	}
      else
	{
	  aln = &sl;
	}
    }
  else
    {
      if(option[1])
	{
	  aln = &na;
	}
      else
	{
	  aln = &nl;
	}
    }	
  cout << "Test sequeces:" << endl;
  cout << S1 << endl;
  cout << S2 << endl;
  cout << "Alignment Type:" <<
  cout << typeid(*aln).name() << endl;
  string A1, A2;
  aln->calculate_DP_matrix();
  //aln->show(); //DP行列とTRACE行列の確認
  aln->traceback(A1, A2);
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
