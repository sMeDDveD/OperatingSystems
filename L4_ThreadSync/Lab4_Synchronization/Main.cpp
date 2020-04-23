#include "MarkersEnv.h"

int main()
{
	try 
	{
		Markers::Main();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
}