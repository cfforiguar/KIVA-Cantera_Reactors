// function example
#include <iostream>
#include <new>
using namespace std;

int addition (int * p, int i)
{
  int n;
  int tmp=0;
  for (n=0; n<i; n++)
    tmp=tmp+p[n];
  return tmp;
}

int main ()
{
  int i,n;
  int * p;
  cout << "How many numbers would you like to type? ";
  cin >> i;
  p= new int[i];
    for (n=0; n<i; n++)
    {
      cout << "Enter number: ";
      cin >> p[n];
    }
    cout << "You have entered: ";
    for (n=0; n<i; n++)
      cout << p[n] << ", ";
    cout << "The sum od your values is: ";
    cout << addition(p,i);
  
  return 0;
}
