#include <iostream>
#include "Subprocess.h"

int main() {
	try {
		Subprocess(R"(Subprocess.exe)").CreateSubprocess(true);
	}
	catch (const std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}
	
	system("pause");
}