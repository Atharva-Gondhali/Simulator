#include<iostream>

#include"Physics.h"

int main() 
{
	Physics physics;

	while(physics.running())
	{
		if (physics.start == true)
		{
			physics.update();
		}
		physics.render();
	}

	return 0;
}