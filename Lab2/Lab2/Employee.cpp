#include "Employee.h"
#include <fstream>
#include <iostream>
#include <string>

namespace EmployeeIO
{
	void Write(const Employee& employee, std::ofstream& output, double salaryPerHour)
	{
		output << employee.num << " " << employee.name << " " << employee.hours << " "
			<< employee.hours * salaryPerHour << std::endl;
	}

	Employee Read()
	{
		std::string name;
		int num;
		double hours;

		std::cout << "Enter employee's ID: " << std::endl;
		std::cin >> num;

		std::cout << "Enter employee's name: " << std::endl;
		std::cin >> name;

		std::cout << "Enter employee's hours: " << std::endl;
		std::cin >> hours;

		Employee e{};

		strcpy_s(e.name, name.c_str());
		e.hours = hours;
		e.num = num;

		return e;
	}

}
