#pragma once

#include <exception>
#include <string>

class JobException : public std::exception {
public:
	JobException(const std::string& message) : m_Message(message) {}

	const char* what() const noexcept override { return m_Message.c_str(); }

private:
	std::string m_Message;
};