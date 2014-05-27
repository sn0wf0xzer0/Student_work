//This application Calculates the center of mass of a set of objects at some relative distances.
//Athour: John Palacios

#include <iostream>
#include "Coord.h"
using namespace std;


const int NUM_OBJ = 3;

//This function determines the center of mass given array of masses and an array of positions.
Coord centerMass(double [], Coord [], int);

//Determines the center of mass given arrays of masses and positions, and an index to ignore.
Coord ignoreMass(double [], Coord [], int, int);

//Constant time complexity version of f(x) ignmoreMass.
Coord constIgnore(double [], Coord [], Coord, double, int);

int main()
{
	char dims[NUM_OBJ] = {'x', 'y', 'z'};
	double mass[NUM_OBJ];
	double totalMass = 0;
	Coord local[NUM_OBJ];
	Coord centerOfMassA;
	Coord centerOfMassB;
	Coord centerOfMassC;
	int indToIgnore;

	cout << "Center of mass Calculator ver. 0.1.1\n";

	for(int i = 0; i < NUM_OBJ; i++){
		cout << "Please enter mass of object " << i << ": ";
		cin >> mass[i];
		for(int j = 0; j < NUM_OBJ; j++){
			cout << "Please enter " << dims[j] << " coordinant for object " << i << ": ";
			cin >> local[i].coords[j];
		}
	}

	for(int i = 0; i < NUM_OBJ; i++){
		totalMass += mass[i];
	}

	centerOfMassA = centerMass(mass, local, NUM_OBJ);

	cout << "The center of mass for this system is located at\n"
		<< "(x, y, z) (" << centerOfMassA.coords[0] << ", " << centerOfMassA.coords[1] << ", " << centerOfMassA.coords[2] << ")\n";

	cout << "Now to recalculate the Center of mass when ignoring one object.\n"
		<< "Please enter which object to ignore: ";
	cin >> indToIgnore;

	centerOfMassB = ignoreMass(mass, local, NUM_OBJ, indToIgnore);

	cout << "The center of mass for this system is located at\n"
		<< "(x, y, z) (" << centerOfMassB.coords[0] << ", " << centerOfMassB.coords[1] << ", " << centerOfMassB.coords[2] << ")\n";

	centerOfMassC = constIgnore(mass, local, centerOfMassA, totalMass, indToIgnore);

	cout << "The center of mass for this system (calculated at constant time) is located at\n"
		<< "(x, y, z) (" << centerOfMassC.coords[0] << ", " << centerOfMassC.coords[1] << ", " << centerOfMassC.coords[2] << ")\n";

	return 0;
}

Coord centerMass(double m[], Coord pos[], int size)
{
	double totalMass = 0;
	Coord *moments;
	Coord WeightedS;
	Coord com;

	moments = new Coord [size];
	
	for(int i = 0; i < size; i++){
		totalMass += m[i];
		for(int j = 0; j < 3; j++){
			moments[i].coords[j] = m[i] * pos[i].coords[j];
		}
	}

	for(int i = 0; i < size; i++){
	WeightedS.coords[i] = 0;
	}

	for(int i = 0; i < size; i++){
		for(int j = 0; j < 3; j++){
			WeightedS.coords[j] += moments[i].coords[j];
		}
	}

	for(int i = 0; i < size; i++){
	com.coords[i] = WeightedS.coords[i] / totalMass;
	}

	return com;
}

Coord ignoreMass(double m[], Coord pos[], int numObj, int ignore)
{
	double totalMass = 0;
	double *masses;
	Coord *positions;
	Coord *moments;
	Coord WeightedS;
	Coord com;
	int size = numObj - 1;
	int count = 0;

	masses = new double [size];
	positions = new Coord [size];
	moments = new Coord [size];
	
	for(int i = 0; i < size + 1; i++){
		if(i != ignore){
			masses[count] = m[i];
			for(int j = 0; j < 3; j++){
				positions[count].coords[j] = pos[i].coords[j];
			}
			count++;
		}
	}

	for(int i = 0; i < size; i++){
		totalMass += masses[i];
		for(int j = 0; j < 3; j++){
			moments[i].coords[j] = masses[i] * positions[i].coords[j];
		}
	}

	for(int i = 0; i < 3; i++){
	WeightedS.coords[i] = 0;
	}

	for(int i = 0; i < size; i++){
		for(int j = 0; j < 3; j++){
			WeightedS.coords[j] += moments[i].coords[j];
		}
	}

	for(int i = 0; i < 3; i++){
	com.coords[i] = WeightedS.coords[i] / totalMass;
	}

	return com;
}

Coord constIgnore(double m[], Coord pos[], Coord com, double totalMass, int ignore)
{
	Coord newCOM;
	double newTotMass = totalMass - m[ignore];

	for(int i = 0; i < 3; i++){
		newCOM.coords[i] = com.coords[i] * totalMass;
	}

	for(int i = 0; i < 2; i++){
		newCOM.coords[i] -= m[ignore] * pos[ignore].coords[i];
		newCOM.coords[i] /= newTotMass;
	}

	return newCOM;
}