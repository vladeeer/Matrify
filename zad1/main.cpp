#include"Matrify.h"

int main()
{
	Matrify matrify;

	// Program Loop
	while (!matrify.getQuit())
	{
		matrify.update();
	}

	return 0;
}