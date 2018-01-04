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
	class windows_error : public std::error_code
	{
	public:
		/// Construct with the given error code.
		explicit windows_error(boost::winapi::DWORD_ code) noexcept;

		/// GetLastError constructor
		explicit windows_error(gle) noexcept;

		/// Default constructor: no error.
		explicit windows_error() noexcept;

		void update() noexcept;

		void set_success() noexcept;

		void set(boost::winapi::DWORD_ code) noexcept;
	};

} // end namespace error 

using error::windows_error;

} // end namespace distant

// Implementation:
#include <distant\error\impl\windows_error.hxx>