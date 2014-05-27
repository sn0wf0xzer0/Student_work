//Vigenere Cipher
//Edited by: John Palacios

#include <string>
#include <iostream>
using namespace std;

//Encrypts a message given some key using the Vigenere Cipher.
string encrypt(string, string);

//Decrypts a messae given some key using the Vigenere Cipher.
string decrypt(string, string);

int main()
{
	string msg, cyp, key;

	cout << "Enter a message: ";
	getline(cin, msg);
	cout << "Enter key: ";
	getline(cin, key);
	cout << "Cypher: " << encrypt(msg, key) << endl;

	key.clear();
	cout << "Enter a cypher: ";
	getline(cin, cyp);
	cout << "Enther key: ";
	getline(cin, key);
	cout << "Message: " << decrypt(cyp, key) << endl;

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