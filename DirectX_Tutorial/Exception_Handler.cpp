#include "Exception_Handler.h"

Exception_Handler::
Exception_Handler(int line, const char* file) noexcept : line{ line }, file{ file } {}

const char* Exception_Handler::
what()const noexcept
{
	std::ostringstream oss;
	oss << m_getType() << std::endl
		<< m_getOriginString();
	
	this->whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* 
Exception_Handler::m_getType() const noexcept
{
	return "Exception type!";
}

int 
Exception_Handler::m_getLine()const noexcept
{
	return line;
}

const std::string& 
Exception_Handler:: m_getFile()const noexcept
{
	return file;
}

std::string 
Exception_Handler::m_getOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line << std::endl;
	return oss.str();
}
