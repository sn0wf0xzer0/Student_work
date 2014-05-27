//Startup code for SDL
#include "Gopher.h"
#include <string>

int main( int argc, char* args[] )
{
	Gopher gm;
	if ( !gm.Init() )
		return -1;

	while (gm.Update());
	
	gm.Close();

	return 0;
}