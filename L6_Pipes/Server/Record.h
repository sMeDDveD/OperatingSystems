#pragma once
class Record
{
public:
    enum class Status
    {
	    READ,
	    WRITE,
	    FREE
    };

private:
	int offset;
	int readers = 0;
	Status state;
	
public:
	Record() = default;
    explicit Record(int offset, Status status = Status::FREE);

    void SetState(Status state);
    int GetOffset() const;
	void AddReader();
	void ReleaseReader();
	bool IsReadable() const;
	bool IsWritable() const;
    Status GetState() const;
	~Record() = default;
};

