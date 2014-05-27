//Euclidean algorithm for finding the greatest common divisor.
//Author: John Palacios

#include <iostream>
using namespace std;

//Returns the greatest common divisor of a and b.
int gcd(int a, int b);

int main()
{
	int one;
	int two;
	char loop ='y';

	do
	{
	cout << "Please enter two ints.\n";
	cin >> one;
	cin >> two;

	cout << "The greatest common divisor of " << one << " and " << two << " is\n"
		<< gcd(one, two) << endl;
	cout << "Would you like to find the greatest common divisor of another pair\n"
		<< "of ints? (y/n)\n";
	cin >> loop;
	} while (tolower(loop) == 'y');

	return 0;
}

int gcd(int a, int b)
{
	int r;

	if(a < b)
		swap(a, b);

	while (b != 0){
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}