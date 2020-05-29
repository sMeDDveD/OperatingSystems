#pragma once
#include <string>
#include "Employee.h"

namespace Constants
{
	const std::string pipePrefix = R"(\\.\pipe\mynamedpipe)";
}


struct EmployeeMessage
{
	EmployeeMessage() = default;
    enum class Status
    {
        OK,
        FAILED
    };

	Status status;
	Employee employee;


    EmployeeMessage(Status status, const Employee& employee)
        : status(status),
          employee(employee)
    {
    }
};

struct ControlMessage
{
	ControlMessage() = default;
    enum class Type
    {
        READ,
        WRITE,
        CLOSE
    };

	Type type;
	int id;


    ControlMessage(Type type, int id)
        : type(type),
          id(id)
    {
    }
};