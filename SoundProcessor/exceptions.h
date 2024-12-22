
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <iostream>
#include <string>
#include <exception>

class SyntaxException : std::exception
{
private:
	std::string message;
public:
	SyntaxException(const std::string& msg) 
	{
		message += "SyntaxException: ";
		message += msg;
	};

	const char* what() const noexcept override
	{
		return message.c_str();
	}
};

class FileFormatException : std::exception
{
private:
	std::string message;
public:
	FileFormatException(const std::string& msg) 
	{
		message += "FileFormatException: ";
		message += msg;
	};

	const char* what() const noexcept override
	{
		return message.c_str();
	}
};







#endif // !EXCEPTIONS_H

