#include "NouException.h"
#include <sstream>

NouException::NouException(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{

}

const char* NouException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

int NouException::GetLine() const noexcept
{
	return line;
}

const char* NouException::GetType() const noexcept
{
	return "NouEngine Exception";
}

const std::string& NouException::GetFile() const noexcept
{
	return file;
}

std::string NouException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}