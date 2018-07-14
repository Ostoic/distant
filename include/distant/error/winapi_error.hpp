// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once


#include <string>
#include <ostream>
#include <system_error>

#include <boost/winapi/basic_types.hpp>

namespace distant::error {

	class winapi_category : public std::error_category
	{
		const char* name() const noexcept override final;

		std::string message(int value) const override final;
	};

	const winapi_category& get_windows_category() noexcept;

	struct gle {};

	/// @brief An error_code representing a winapi error.
	/// This is NOT a type of exception. 
	class winapi_error_code : public std::error_code
	{
		using base = std::error_code;

	public:
		/// Construct an error code with the given code.
		explicit winapi_error_code(boost::winapi::DWORD_ code) noexcept;

		/// Construct an error code with the code obtained from ::GetLastError.
		winapi_error_code(gle) noexcept;

		/// Construct an error code with no error.
		explicit winapi_error_code() noexcept;

		/// Update the error code
		void update_last() noexcept;

		void set_success() noexcept;

		void set_last(boost::winapi::DWORD_ code) noexcept;
	};

	/// @brief Represents a winapi error_code in the form of a system_error exception.
	class winapi_error : public std::system_error
	{
	public:
		winapi_error();
		winapi_error(const winapi_error_code code) : std::system_error(code) {}
		explicit winapi_error(const std::string& message);
	};

	/// @brief Return the last error local to the executing thread.
	/// @return the \a winapi_error_code representing the specified error code
	winapi_error_code last_error() noexcept;

	/// @brief Write a windows error to an output stream.
	/// @param stream the output stream.
	/// @param error the windows error to write.
	/// @return the modified output stream.
	template <typename CharT, typename CharTraits>
	std::basic_ostream<CharT, CharTraits>& operator<<(std::basic_ostream<CharT, CharTraits>& stream, const winapi_error& error);

	/// @brief Write a windows error code to an output stream. 
	/// @param stream the output stream.
	/// @param error the windows error to write.
	/// @return the modified output stream.
	template <typename CharT, typename CharTraits>
	std::basic_ostream<CharT, CharTraits>& operator<<(std::basic_ostream<CharT, CharTraits>& stream, const winapi_error_code& error);

} // end namespace distant::error

namespace distant
{
	using error::last_error;
	using error::winapi_error;
}

// Implementation:
#include "impl/winapi_error.hxx"
