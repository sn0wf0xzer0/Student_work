//This algorithm finds the elements in a finite sequence of integers that is greater than the sum of all of the previous elements.
//Author: John Palacios

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

//Generates an array of length size and fills it with random values.
//Element 0 is the length of this array.
int *genRandArray(int);

//returns an array containing the elements of a given array who are
//greater than the sum of all previous elements, element 0 is the
//number of elements in this array.
int *greatSums(int []);

int main()
{
	/*int *arr;
	int *result;
	int size;

	cout << "Please enter a size of array: \n";
	cin >> size;

	arr = genRandArray(size);
	result = greatSums(arr);

	for(int i = 1; i < size; i++)
	{
		cout << " " << arr[i];
	}
	cout << " end of list.\n";

	for(int i = 1; i < result[0]; i++)
	{
		cout << " " << result[i];
	}
	cout << " end of results.\n";*/
	//char loop = 'y';
	//int num;
	//int den;

	//do
	//{
	//	cout << "Please enter the numerator: ";
	//	cin >> num;
	//	cout << "Please enter the denomenator: ";
	//	cin >> den;
	//	cout << num << " divided by " << den << " is " << num / den << endl
	//		<< "with a remainder of " << num % den << endl;
	//	cout << "again?\n";
	//	cin >> loop;
	//} while (tolower(loop) == 'y');

	int **codes;
	int checkDigit[4] = {0};
	int sum = 0;

	codes = new int* [4];
	for(int i = 0; i < 4; i++){
		codes[i] = new int [10];
	}

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 10; j++){
			cout << "code " << i << " digit " << j << ": ";
			cin >> codes[i][j];
		}
	}

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 10; j++){
			checkDigit[i] += codes[i][j];
		}
	}

	for(int i = 0; i < 4; i++){
		checkDigit[i] %= 9;
		cout << "check digit " << i << " is " << checkDigit[i] << endl;
	}

	for(int i = 0; i < 4; i++){
		delete [] codes[i];
	}
	delete codes;
	codes = 0;

	return 0;
}

int *genRandArray(int size)
{
	int *arr;
	srand(time(0));
	arr = new int [size];
	arr[0] = size;
	for(int i = 1; i < size; i++)
	{
		arr[i] = (rand() - rand()) % 33000;
	}

	return arr;
}

int *greatSums(int arr[])
{
	int size = arr[0];
	int *saught;
	int qty = 1;
	int sum = 0;
	int count = 1;

	for(int i = 1; i < size; i++)
	{
		for(int j = 1; j < i; j++)
		{
			sum += arr[j];
		}
		if(arr[i] > sum)
			qty++;
		sum = 0;
	}

	saught = new int [qty];
	saught[0] = qty - 1;

	for(int i = 1; i < size; i++)
	{
		for(int j = 1; j < i; j++)
		{
			sum += arr[j];
		}
		if(arr[i] > sum)
		{
			saught[count] = arr[i];
			count++;
		}
		sum = 0;
	}

	return saught;
}