// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../winapi_error.hpp"

#include <boost/winapi/basic_types.hpp>
#include <boost/winapi/error_codes.hpp>
#include <boost/winapi/error_handling.hpp>
#include <boost/winapi/get_last_error.hpp>
#include <boost/winapi/local_memory.hpp>
#include <boost/winapi/limits.hpp>

#include <distant/config.hpp>
#include <distant/support/winapi/last_error.hpp>

namespace distant::error {

/******************/
//winapi_category:
/******************/
	inline const char* winapi_category::name() const noexcept
	{
		return "winapi_error";
	}

	inline std::string winapi_category::message(const int value) const
	{
		namespace winapi = boost::winapi;

		// Retrieve the system error message for the given error code.
		char* buffer = nullptr;

		winapi::format_message(
			winapi::FORMAT_MESSAGE_FROM_SYSTEM_ |
			winapi::FORMAT_MESSAGE_ALLOCATE_BUFFER_,
			nullptr, value,
			winapi::MAKELANGID_(winapi::LANG_NEUTRAL_, winapi::SUBLANG_DEFAULT_),
			reinterpret_cast<winapi::LPSTR_>(&buffer), 0, nullptr);

		std::string result = buffer;
		winapi::LocalFree(buffer);

		const auto pos = result.find("\r\n");
		if (pos != std::string::npos)
			result.erase(pos, pos + 2);

		return result;
	}

	inline const winapi_category& get_windows_category() noexcept
	{
		thread_local winapi_category error_category;
		return error_category;
	}


/*******************/
//winapi_error_code:
/*******************/

	inline winapi_error_code::winapi_error_code() noexcept
		: winapi_error_code(boost::winapi::GetLastError()) {}

	inline winapi_error_code::winapi_error_code(const boost::winapi::DWORD_ code) noexcept
		: std::error_code(code, get_windows_category()) {}

	inline void winapi_error_code::update_last() noexcept
	{
		this->set_last(boost::winapi::GetLastError());
	}

	inline void winapi_error_code::set_success() noexcept
	{
		this->set_last(boost::winapi::NO_ERROR_);
	}

	inline void winapi_error_code::set_last(const boost::winapi::DWORD_ code) noexcept
	{
		boost::winapi::SetLastError(code);
		this->assign(code, this->category());
	}

	inline winapi_error_code last_error() noexcept
	{
		return winapi_error_code{};
	}

// class winapi_error
	inline winapi_error::winapi_error()
		: std::system_error(last_error())
	{}

	inline winapi_error::winapi_error(const std::string& message)
		: std::system_error(last_error(), message)
	{}

//free:
	template <typename CharT, typename CharTraits>
	std::basic_ostream<CharT, CharTraits>& operator<<(std::basic_ostream<CharT, CharTraits>& stream, const winapi_error& error)
	{
		stream << error.what() << " " << error.code();
		return stream;
	}

	template <typename CharT, typename CharTraits>
	std::basic_ostream<CharT, CharTraits>& operator<<(std::basic_ostream<CharT, CharTraits>& stream, const winapi_error_code& code)
	{
		const auto& category = code.category();
		stream << category.message(code.value()) << " " << category.name() << ":" << code.value();
		//stream << winapi_error{ code };
		return stream;
	}

} // namespace distant::error
