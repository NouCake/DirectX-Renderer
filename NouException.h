#pragma once

#include "NouWin.h"

#include <exception>
#include <string>

class NouException
{
public:
	class BaseException : public std::exception
	{
	public:
		BaseException(int line, const char* file) noexcept;
		BaseException(int line, const char* file, std::string text) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;

		int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
		std::string GetOriginString() const noexcept;

	private:
		int line;
		std::string file;
		std::string text;

	protected:
		mutable std::string whatBuffer;
	};

	class HrException : BaseException
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
};

//MACROS
#define NOU_EXCEPT( hr ) NouException::HrException( __LINE__,__FILE__,hr )
#define NOU_LAST_EXCEPT() NouException::HrException( __LINE__,__FILE__, GetLastError() )
#define CHECK_HR_EXCEPT() if(FAILED(res)) throw NouException::HrException( __LINE__, __FILE__, res);