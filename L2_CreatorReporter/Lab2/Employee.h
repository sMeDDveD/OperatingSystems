#pragma once
#include <fstream>
#include <iostream>
#include <string>

struct Employee
{
	int num;
	char name[10];
	double hours;
};

namespace EmployeeIO
{
	inline void Serialize(const Employee& employee, std::ofstream& out)
	{
		out.write(reinterpret_cast<const char*>(&employee), sizeof(Employee));
	}

	inline Employee Deserialize(std::ifstream& input)
	{
		Employee employee{};
		input.read(reinterpret_cast<char*>(&employee), sizeof(Employee));
		return employee;
	}

	
	void Write(const Employee& employee, std::ofstream& output, double salaryPerHour);
	Employee Read(std::istream& input, std::ostream& output);
}