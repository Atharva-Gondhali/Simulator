#include<iostream>

#include"Physics.h"

int main() 
{
	Physics physics;

	while(physics.running())
	{
		physics.update();

		physics.render();
	}

	return 0;
}