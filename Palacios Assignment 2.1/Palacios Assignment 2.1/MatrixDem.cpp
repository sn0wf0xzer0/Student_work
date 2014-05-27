//This is the main file for assignment 2. Here the sum and product of two int type matricies shall be found.
//Author: John Palacios

#include <iostream>
#include "Matrix.h"
using namespace std;

//This function simply calls the add method of the int type Matricies.
Matrix<int> *add(Matrix<int> &, Matrix<int> &);

//This function simply calls the multiply method of the int type Matricies.
Matrix<int> *multiply(Matrix<int> &, Matrix<int> &);

//Prints the values of an int type matrix.
void print(Matrix<int> &);

int main()
{
	//Matrix<int> A(m, n);  <-- this gives a custom matrix, also, it is possible to change the types.
	Matrix<int> a;					//Instance of int type matrix variable a.
	Matrix<int> b;					//Instance of int type matrix variable b.
	Matrix<int> *sum;				//result of add f(x);
	Matrix<int> *product;			//result of multiply f(x);

	cout << "This simple application demonstrates an addition and multiplication\n"
		<< "algorithm developed for use with matricies. Please follow the prompts\n"
		<< "as they appear, thank you.\n";

	//This assumes default 4x4 matricies, other combinations of matricies are possible.
	cout << "Please enter 16 integral values for matrix A.\n";
	a.setVals();					//Prepare for some typing. Each matrix consists of 16 integral values.
	cout << "Matrix A is:\n";
	print(a);						//repeat matrix.
	cout << "Thank you, please enter 16 integral values for matrix B.\n";
	b.setVals();
	cout << "Matrix B is:\n";
	print(b);

	//Now to perform some arithmatic...
	sum = add(a, b);
	product = multiply(a, b);

	//... and display the results in matrix form.
	cout << "The resulting sum of matricies A and B is:\n";
	print(*(sum));

	cout << "The resulting product of matricies A and B is:\n";
	print(*(product));

	cout << "Thank you for using this application, good bye now.\n";
	return 0;
}

Matrix<int> *add(Matrix<int> &a, Matrix<int> &b)
{
	return a + b; //Since the addition operator is overloaded, no work needs to be done here.
}

Matrix<int> *multiply(Matrix<int> &a, Matrix<int> &b)
{
	return a * b; //Multiplication operator already defines appropriate behavior.
}

void print(Matrix<int> &mat)
{
	char upperLeft = 218;				//These char variables add an asthetic touch to the
	char lowerLeft = 192;				//matrix display.
	char upperRight = 191;
	char lowerRight = 217;
	char middle = 179;
	int rows = mat.getVert();
	int cols = mat.gethori();

	//sloppy if varying numbers of digits, but sufficient for now.
	for(int i = 0; i < rows; i++){
		if(i == 0){ 
			cout << "\t" << upperLeft << " ";
		}
		else if(i == rows - 1){ 
			cout << "\t" << lowerLeft << " ";
		}
		else 
			cout << "\t" << middle << " ";
		for(int j = 0; j < cols; j++){
			if(j == cols -1 && i == 0){
				cout << mat.getVal(i, j) << " " << upperRight;
			}
			else if(j == cols - 1 && i == rows - 1){
				cout << mat.getVal(i, j) << " " << lowerRight;
			}
			else if(j == cols - 1){
				cout << mat.getVal(i, j) << " " << middle;
			}
			else
				cout << mat.getVal(i, j) << " ";
		}
		cout << endl;
	}
}