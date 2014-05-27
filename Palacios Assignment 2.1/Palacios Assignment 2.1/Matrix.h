//The templated Matrix class represents a matrix of values for which multiplication and addition are defined; matrix multiplication and
//addition operations are here defined for objects of this class.
//NOTE: this design has the following improvements over origional specification: Greater generality through templating, variable matrix
//		sizing, and contingencies for illigal operations. The overloaded operators use implied left hand operands rather than explicit
//		left hand operands, are protected from illigal matrix dimensions, and return pointers - so as to provide greater freedom for
//		the main application. This is intentional, and not arbatrary.
//Author: John Palacios

#ifndef MATRIX
#define MATRIX

template <class T>
class Matrix
{
private:
	T **vars;							//Two dimensional array of T type values.
	int vert;							//Vertical hight of matrix. Number of rows.
	int hori;							//Horizontal length of matrix. Number of collumns.
public:
	Matrix();							//Creates default 4x4 square matrix.
	Matrix(int, int);					//Creates custom mxn rectangular matrix.
	Matrix(const Matrix &);				//Copy Constructor.
	~Matrix()							//destructor.
	{ delete [] vars; }
	void setVals();						//Runs nested loop for user input of matrix values. (increment rows, then colls)
	int getVert()						//Returns vert value.
	{ return vert; }
	int gethori()						//Returns hori value.
	{ return hori; }
	T getVal(int row, int col)					//Second method for accessing vars elements.
	{ return vars[row][col]; }
	Matrix operator[] (int elem)		//Overloaded index operator.
	{ return vars[elem]; }
	Matrix* &operator+(const Matrix &);	//Overloaded addition operator. Returns a pointer to a newly created matrix which is the sum
										//of the left and right operands.
	Matrix* &operator*(const Matrix &);	//Overloaded multiplication operator. Returns a pointer to a newly created matrix which is the
										//product of the left and right operands.
};

template <class T>
Matrix<T>::Matrix()
{
	vert = 4;
	hori = 4;
	vars = new T* [vert];
	for(int i = 0; i < vert; i++)
	{
		vars[i] = new T [hori];
	}
}

template <class T>
Matrix<T>::Matrix(int m, int n)
{
	vert = m;
	hori = n;
	vars = new T* [vert];
	for(int i = 0; i < vert; i++)
	{
		vars[i] = new T [hori];
	}
}

template <class T>
Matrix<T>::Matrix(const Matrix &obj)
{
	vert = obj.vert;					//Set attributes from copied object.
	hori = obj.hori;
	vars = new T* [vert];
	for(int i = 0; i < vert; i++)
	{
		vars[i] = new T [hori];
	}
	for(int i = 0; i < vert; i++){
		for(int j = 0; j < hori; j++){
		vars[i][j] = obj.vars[i][j]			//element wise copying of element values.
		}
	}
}

template <class T>
void Matrix<T>::setVals()
{
	//This algorithm could be less specific, but it works well for this assignment.
	for(int i = 0; i < vert; i++){
		for(int j = 0; j < hori; j++){
			cout << "Enter value at row " << i << " col " << j << endl;
			cin >> vars[i][j];
		}
	}
}

template <class T>
Matrix<T>* &Matrix<T>::operator+(const Matrix &obj)
{
	Matrix *result;						//pointer variable to be returned.
	if(vert != obj.vert || hori != obj.hori){
		cout << "Error: attempting to add matricies of different dimensions.\n";
		result = new Matrix<T>(0, 0);
		return result;
	}
	
	else
	{
		result = new Matrix(vert, hori);			//Nested loop stores sums into dynamic variable.
		for(int i = 0; i < vert; i++){
			for(int j = 0; j < hori; j++){
				result->vars[i][j] = vars[i][j] + obj.vars[i][j];
			}
		}
	}
	return result;
}

template <class T>
Matrix<T>* &Matrix<T>::operator*(const Matrix &obj)
{
	Matrix *result;						//pointer variable to be returned.
	T sum = 0;							//An accumulator for each element of result vars.
	int watchVal;
	//Deterimin dimensions of resultant matrix.
	if(vert == obj.vert && hori == obj.hori)
		result = new Matrix(hori, vert);
	else if(hori == obj.vert)
		result = new Matrix(vert, obj.hori);
	else{				//No other pattern is acceptable.				
		cout << "Error: attempting to multiply matricies not of m x k * k x n form.\n";
		result = new Matrix<T>(0, 0);
		return result;
	}

	for(int m = 0; m < result->vert; m++){
		for(int n = 0; n < result->hori; n++){
			for(int i = 0; i < hori; i++){
				sum += vars[m][i] * obj.vars[i][n];
			}
			result->vars[m][n] = sum;
			sum = 0;
		}
	}
	return result;
}
#endif /* MATRIX */