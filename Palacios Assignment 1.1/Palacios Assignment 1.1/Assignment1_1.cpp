//This simple application proves that there is a range of 100 consecutive integral values who are not perfect squares.
//Author: John Palacios

#include <iostream>
using namespace std;

//recursively searches for at least 100 consecutive integers who are not perfect squares
//by measureing the difference of the squares of the two integers passed to the f(x).
void measureRange(int lower, int upper);

int main(){

	cout << "I posite that there are at least 100 consecutive integers who\n"
		<< "are not perfect squares, and this application is my proof.\n"
		<< "This simple application displays a list of at least one hundered\n"
		<< "consecutive integers who do not posses integral square roots if\n"
		<< "such a set of numbers exist.\n";

	measureRange(0, 1);

	return 0;
}

void measureRange(int lower, int upper){
	int diff;			//This is the difference of squares.
	int *range;			//This is a pointer to an array of the desired integers.
	char pow = 253;		//for display of hypertext.

	//Find the difference of two squares and compare that
	//to our desired difference.
	diff = upper*upper - lower*lower;
	
	if(diff >= 100){
		//I could have simply printed a counter, but I could not
		//pass up an oprotunity to practice dynamic memory allocation
		range = new int [diff - 1];
		for(int i = lower * lower + 1, count = 0; i < upper * upper; i++, count++){
			*(range + count) = i;
		}
		//now to print the list of consecutive integers who are not perfect squares.
		cout << "The desired set of consecutive integers who are not perfect squares follow:\n"
			<< "\t{";
		for(int i = 1; i < diff; i++){
			if(i % 10 == 0){
				cout << "\n\t" << range[i - 1] << ", ";
			}
			else if(i < diff - 1)
			cout << range[i - 1] << ", ";
			else
				cout << range[i - 1];
		}
		cout << "} These numbers exist between " << lower << pow << " and " << upper << pow << endl;
		delete [] range;
	}
	//should the two values provided not yield the results
	//I want, I simply increment them until they do.
	else
		measureRange(++lower, ++upper);
}