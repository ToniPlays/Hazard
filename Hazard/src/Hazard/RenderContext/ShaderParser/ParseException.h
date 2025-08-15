#pragma once

#include <string>
#include <exception>

class ParseException : public std::exception {
public:
    ParseException(const std::string& message): m_Message(message) {}

    const char* what() const noexcept override { return m_Message.c_str(); }

private:
    std::string m_Message;
};
