#pragma once
#include <exception>
#include <sstream>

class Exception_Handler: public std::exception
{
public:
	Exception_Handler(int line,const char* file)noexcept;
	const char* what()const noexcept override;

	virtual const char* m_getType()const noexcept;
	int m_getLine()const noexcept;
	
	const std::string& m_getFile()const noexcept;
	std::string m_getOriginString()const noexcept;
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};

