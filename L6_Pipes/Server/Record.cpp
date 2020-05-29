#include "Record.h"


Record::Record(int offset, Status status): offset(offset), state(status)
{
}

void Record::SetState(Status state)
{
    this->state = state;
}

int Record::GetOffset() const
{
    return offset;
}

void Record::AddReader()
{
	readers++;
}

void Record::ReleaseReader()
{
	readers--;
    if (readers == 0)
    {
		state = Status::FREE;
    }
}

bool Record::IsReadable() const
{
	return state == Status::READ || state == Status::FREE;
}

bool Record::IsWritable() const
{
	return state == Status::FREE;
}

Record::Status Record::GetState() const
{
    return state;
}
