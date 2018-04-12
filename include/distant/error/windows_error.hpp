// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once


#include <string>
#include <ostream>
#include <system_error>

#include <boost/winapi/basic_types.hpp>

namespace distant {
namespace error   {

	class windows_category : public std::error_category
	{
		const char* name() const noexcept override final;

		std::string message(int value) const override final;
	};

	const windows_category& get_windows_category() noexcept;

	struct gle {};

	/// Windows error code
	class windows_error_code : public std::error_code
	{
	public:
		/// Construct an error code with the given code.
		explicit windows_error_code(boost::winapi::DWORD_ code) noexcept;

		/// Construct an error code with the code obtained from ::GetLastError.
		windows_error_code(gle) noexcept;

		/// Construct an error code with no error.
		explicit windows_error_code() noexcept;

		/// Update the error code
		void update_last() noexcept;

		void set_success() noexcept;

		void set(boost::winapi::DWORD_ code) noexcept;
	};

	class windows_error : public std::system_error
	{
	public:
		windows_error();
		explicit windows_error(const std::string& message);
	};

	/// @brief Return the last error local to the executing thread.
	/// @return the last error code that was set.
	windows_error_code last_error() noexcept;

	/// @brief Write a windows error to an output stream.
	/// @param stream the output stream.
	/// @param error the windows error to write.
	/// @return the modified output stream.
	template <typename CharT, typename CharTraits>
	std::basic_ostream<CharT, CharTraits>& operator<<(std::basic_ostream<CharT, CharTraits>& stream, const windows_error_code& error);

} // end namespace error 

using error::last_error;
using windows_error = error::windows_error;

} // end namespace distant

// Implementation:
#include <distant/error/impl/windows_error.hxx>