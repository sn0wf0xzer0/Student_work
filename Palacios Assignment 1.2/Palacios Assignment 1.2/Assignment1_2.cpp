//This program finds a counter example to the statement: If n is an integer then n^2 – n + 41 is prime.
//Author: John Palacios

#include <iostream>
using namespace std;

int main()
{
	int result;

	//I will take the exhaustive approach to discover a counter example to this conjecture,
	//though I suspect that n = 41 itself is a counter example, and therefore will not
	//need to check all values up to 2^30th.
	for(int i = 2; i < 50; i++){
		result = i * i - i + 41;
		cout << "testing n^2 - n + 41 with n = " << i << " for primality...\n";
		for(int j = 2; j <= i; j++){
			//Check for factors other than 1 and i.
			if (result % j == 0){
				cout << "if n = " << i << " in the statement n^2 - n + 41, the result\n" 
					<< result << " has the factor " << j << " in addition to 1 and " << i << endl
					<< " and is therefore not a prime number. n = " << i << " is a counter example\n";
				//if n = 41, then a little mathmatics shows that n^2 - n + 41 = n^2, which has three factors of 41, 41 and 1.
				return 0;
				//Once a counter example is found, exit stage right.
			}
		}
	}
	return 0;
}