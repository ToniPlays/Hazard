
#pragma once

#include <exception>
#include <string>

class CompileException : public std::exception {
public:
    CompileException(uint32_t line, const std::string& message) : m_Line(line), m_Message(message) {}

    const char* what() const noexcept override { return m_Message.c_str(); }
    uint32_t get_line() const noexcept { return m_Line; }

private:
    uint32_t m_Line;
    std::string m_Message;
};
