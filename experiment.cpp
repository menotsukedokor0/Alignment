#include <iostream>
#include "alignment.h"
#include <fstream>
#include <typeinfo>
#include <time.h>
#include <stdlib.h>
using namespace std;

void generate_random_sequence(int N, string &S1, string &S2)
{
  srand((unsigned int) time(NULL) );
  for(int i = 0; i < N; i++){
    int tmp = rand() % 4; 
    switch( tmp )
      {
      case 0: S1 += 'A'; break;
      case 1: S1 += 'T'; break;
      case 2: S1 += 'G'; break;
      case 3: S1 += 'C'; break;
      default : cerr << "err" << endl;
      }
    tmp = rand() % 4;
    switch( tmp )
      {
      case 0: S2 += 'A'; break;
      case 1: S2 += 'T'; break;
      case 2: S2 += 'G'; break;
      case 3: S2 += 'C'; break;
      default : cerr << "err" << endl;
      }
  }
  
}

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

  ifstream ifs("experiment_input.txt");
  ifs >> m;
  ifs >> x;
  ifs >> o;
  ifs >> e;

  int N;
  ifs >> N;
  generate_random_sequence(N, S1, S2);


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
   //nコメントアウトした部分でオブジェクトを定義してやると何故か実行時ポリモーフィズムが実現できなくなる。typeid()によるとaln のさすオブジェクトの方が代入後もAlignmentクラスであるこという様子を見ている限り正しく代入できていないようだ。このままのプログラムでいくならif文連発で書いてgoto文を回避できるのでは。
  /*cout << "Test sequeces:" << endl;
  cout << S1 << endl;
  cout << S2 << endl; 
  */
  cout << "Alignment Type:" <<
  cout << typeid(*aln).name() << endl;
  string A1, A2;
  aln->calculate_DP_matrix();
  //aln->show(); //DP行列とTRACE行列の確認
  aln->traceback(A1, A2);
  /*
  for(int i = 0; i < A1.size(); ++i)
    cout << "_";
  cout << endl;
  cout << A1 << endl;
  cout << A2 << endl;
  */
  int count = 0;
  for(int i = 0; i < A1.size(); i++)
    {
      if(A1[i] == A2[i])
	{
	  //cout << '*';
	  count ++;
	}
      else 
	{
	//cout << " ";
	}
    }
  cout << endl;
  cout << "exact matching rate:";
  cout << (double)count / N << endl;
  return 0;
}
