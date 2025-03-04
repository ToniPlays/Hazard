#pragma once

#include "Application/Test.h"

class Sandbox : public Test
{
public:
	void Reset() override;
	void Init() override;
	void Run() override;
	void Terminate() override;
	void OnEvent(Event& e) override;

	const char* GetName() override { return "Sandbox"; }

private:
};
