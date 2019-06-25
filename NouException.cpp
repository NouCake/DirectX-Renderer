#include "NouException.h"
#include <sstream>


//BASE EXCEPTION

NouException::BaseException::BaseException(int line, const char* file) noexcept
	:
	line(line),
	file(file),
	text("None")
{

}

NouException::BaseException::BaseException(int line, const char* file, std::string text) noexcept
	:
	line(line),
	file(file),
	text(text)
{

}

const char* NouException::BaseException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

int NouException::BaseException::GetLine() const noexcept
{
	return line;
}

const char* NouException::BaseException::GetType() const noexcept
{
	return "NouEngine Exception";
}

const std::string& NouException::BaseException::GetFile() const noexcept
{
	return file;
}

std::string NouException::BaseException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[Text] " << text << std::endl
		<< "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}

//HR EXCEPTION


NouException::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	BaseException(line, file),
	hr(hr)
{}

std::string NouException::HrException::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

const char* NouException::HrException::GetType() const noexcept
{
	return "NouWindow Exception";
}

HRESULT NouException::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string NouException::HrException::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}

const char* NouException::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}