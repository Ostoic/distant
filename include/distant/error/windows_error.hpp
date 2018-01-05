#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <string>
#include <system_error>

#include <boost\winapi\basic_types.hpp>

namespace distant {
namespace error   {

	class windows_category : public std::error_category
	{
		const char* name() const noexcept override final;

		std::string message(int value) const override final;
	};

	const windows_category& get_windows_category();

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
		void get_last() noexcept;

		void set_success() noexcept;

		void set(boost::winapi::DWORD_ code) noexcept;
	};

	const windows_error_code last_error;

} // end namespace error 

using windows_error = error::windows_error_code;

} // end namespace distant

// Implementation:
#include <distant\error\impl\windows_error.hxx>