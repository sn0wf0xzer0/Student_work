//This application allows encryption of messages using the Cesar Cipher method.
//Author: John Palacios

#include <string>
#include <iostream>
using namespace std;

//This function transforms it's input string into another string given some
//alphabetical distance, the Cesar method.
string encrypt(string, int);

//Overloaded Cesar encryption algorithm using an input modifier.
string encrypt(string, int, int);

//This function transorms it's input string into another string given some
//alphabetical distance, the inverse to the Cesar method.
string decrypt(string, int);

//Clears cin input buffer.
void flushCin();

int main()
{
	char loop;
	char modifier;
	int key;
	int mult;
	string input;
	string output;

	do
	{
		cout << "Encryption or Decryption?\n";
		getline(cin, input);
		if(tolower(input[0]) == 'e'){
			cout << "Please enter your message: ";
			input.clear();
			getline(cin, input);
			flushCin();
			for(int i = 0; i < 3; i++){
				cout << "Is there a multiplier? (y/n): ";
				cin >> modifier;
				if(tolower(modifier) == 'y'){
					cout << "please enter multiplier: ";
					cin >> mult;
					cout << "Please enter encryption key: ";
					cin >> key;
					output.clear();
					output = encrypt(input, mult, key);
					cout << output << endl;
				}
				else{
				cout << "Please enter encryption key: ";
				cin >> key;
				output.clear();
				output = encrypt(input, key);
				cout << output << endl;
				}
			}
		}
		if(tolower(input[0]) == 'd'){
			cout << "Please enter you cipher: ";
			input.clear();
			getline(cin, input);
			flushCin();
			for(int i = 0; i < 4; i++){
				cout << "Please enter encryption key: ";
				cin >> key;
				output.clear();
				output = decrypt(input, key);
				cout << output << endl;
			}
		}
		cout << "Loop? (y/n): ";
		cin >> loop;
	} while (tolower(loop) != 'n');
	return 0;
}

string encrypt(string message, int key)
{
	int size = message.length();
	int c;
	string cipher;

	for(int i = 0; i < size; i++){
		c = toupper(message[i]) - 'A';
		c += key;
		c %= 26;
		cipher += c + 'A';
	}
	return cipher;
}

string encrypt(string message, int mult, int key)
{
	int size = message.length();
	int c;
	string cipher;

	for(int i = 0; i < size; i++){
		c = toupper(message[i]) - 'A';
		c *= mult;
		if(mult < 0){
			c += key;
			c %= -26;
			c = -c;
		}
		else
		{
		c += key;
		c %= 26;
		}
		cipher += c + 'A';
	}
	return cipher;
}

string decrypt(string cipher, int key)
{
	string message;
	int m;
	int size = cipher.length();

	for(int i = 0; i < size; i++){
		m = toupper(cipher[i]) - 'A';
		m -= key;
		if(m < 0){
			m += 26;
		}
		message += m + 'A';
	}
	return message;
}

void flushCin()
{
	char nextChar;

	do
	{
		cin.get(nextChar);
	}while (nextChar != '\n');
	cin.putback('\n');
}