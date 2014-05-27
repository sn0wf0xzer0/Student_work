//String comparison.
//Author: John Palacios

#include <iostream>
#include <string>
using namespace std;

//Returns true if str1 == str2
bool strComp(string, string);

int main()
{
	string str1;
	string str2;

	cout << "Please enter string one: ";
	getline(cin, str1);
	cout << "Please enter string two: ";
	getline(cin, str2);

	if(strComp(str1, str2))
		cout << "String one and two are identical!\n";
	else
		cout << "String one and two are not the same!\n";
	return 0;
}

bool strComp(string one, string two)
{
	int lenOne = one.length();
	int lenTwo = two.length();
	if(lenOne == lenTwo){
		for(int i = 0; i < lenOne; i++){
			if(tolower(one[i]) != tolower(two[i]))
				return false;
		}
		return true;
	}
}