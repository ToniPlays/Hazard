#include "Debug.h"
#include "Utils/Utility.h"
#pragma once

std::vector<Message*> Debug::logs;

void Debug::Log(std::string text)
{
	Message* message = new Message();
	message->level = 0;
	message->text = text;
	message->time = CurrentTime();
	logs.push_back(message);
}

void Debug::Warn(std::string text)
{
	Message* message = new Message();
	message->level = 1;
	message->text = text;
	message->time = CurrentTime();
	logs.push_back(message);
}

void Debug::Error(std::string text)
{
	Message* message = new Message();
	message->level = 2;
	message->text = text;
	message->time = CurrentTime();
	logs.push_back(message);
}
