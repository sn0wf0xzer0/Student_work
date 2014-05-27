//Change giving algorithm using greedy approach.
//Author: John Palacios

#include <iostream>
using namespace std;

//Returns a number of each coin denomination, whose total is equal to n.
void change(int, int &, int &, int &, int &);

int main()
{
	int money;
	int quarters;
	int dimes;
	int nickels;
	int pennies;

	cout << "Please enter change amount: ";
	cin >> money;
	change(money, quarters, dimes, nickels, pennies);
	cout << "Q: " << quarters << endl
		<< "D: " << dimes << endl
		<< "N: " << nickels << endl
		<< "P: " << pennies << endl;

	return 0;
}

void change(int n, int &quarter, int &dime, int &nickel, int &penny)
{
	int remain = n;
	int qtr = 25;
	int dim = 10;
	int nic = 5;

	if(remain >= qtr){
		quarter = remain / qtr;
		remain -= qtr * quarter;
	}
	else
		quarter = 0;

	if(remain >= dim){
		dime = remain / dim;
		remain -= dim * dime;
	}
	else
		dime = 0;

	if(remain >= nic){
		nickel = remain / nic;
		remain -= nic * nickel;
	}
	else
		nickel = 0;

	penny = remain;
}