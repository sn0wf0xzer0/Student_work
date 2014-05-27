//This program uses the Vigenere cipher to answer questions 18 and 19 on page 304.
//Author: John Palacios

#include <string>
#include <iostream>
using namespace std;

//Encrypts a message given some key using the Vigenere Cipher.
string encrypt(string, string);

//Decrypts a messae given some key using the Vigenere Cipher.
string decrypt(string, string);

int main()
{
	string first;
	string second;

	//#18 pg 304
	first = encrypt("SNOWFAL", "BLUE");
	cout << first << endl;
	//This should be TYIAGLF

	//#19 pg 304
	second = decrypt("OIKYWVHBX", "HOT");
	cout << second << endl;
	//This should be HURRICANE

	return 0;
}

string encrypt(string msg, string key)
{
	string cyp;
	int j = 0;
	for(int i = 0; i < msg.length(); i++, j++){
		j %= key.length();
		int m = toupper(msg[i]) - 'A';
		int k = toupper(key[j]) - 'A';
		int c = (m + k) % 26;
		cyp += (c + 'A');
	}
	return cyp;
}

string decrypt(string cyp, string key)
{
	string msg;

	for(int i = 0,j = 0; i < cyp.length(); i++, j++){
		j %= key.length();
		int m = toupper(cyp[i]) - 'A';
		int k = toupper(key[j]) - 'A';
		int c = (m - k) % 26;
		if (c < 0)
			c += 26;
		msg += (c + 'A');
	}
	return msg;
}