#pragma once

class Test
{
public:
	Test() = default;

	virtual void Reset() = 0;
	virtual void Init() = 0;
	virtual void Run() = 0;
	virtual void Terminate() = 0;

	virtual const char* GetName() = 0;
};