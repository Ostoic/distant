#pragma once

/*!
@file
Includes all the library components except the adapters for external
libraries.

@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <Windows.h>

#include <utility>
#include <string>
#include <iostream>

#include <distant\windows\error\format.hpp>

namespace distant {
namespace windows {
namespace error   {

	// Error code class for which every winapi class extends
	class gle
	{
	private:
		using error_type = DWORD;

	public:
		// Get the last error code that was recorded
		// See update_gle comment
		const gle& get_last_error() const { return *this; }

	public:
		constexpr gle() : m_error() {}

		gle(gle&& other) : 
			m_error(std::move(other.m_error))
		{}
		
		std::string to_string() const
		{ return error::format(get_value()); }

		friend std::ostream& operator <<(std::ostream&, const gle&);

	protected:
		error_type get_value() const { return m_error; }

		// We use update_gle to explicitly update the gle
		// This is used to record any errors inside gle encapsulations
		// The user may call get_last_error after accessing the interface,
		// to see if any gles were recorded.
		void update_gle() const { m_error = GetLastError(); }

		void update_gle(const gle& other) const { m_error = other.get_value(); }

		// m_error is mutable so we can still use const member functions
		// in gle-derived classes.
		mutable error_type m_error;
	};

	inline std::ostream& operator <<(std::ostream& stream, const gle& err)
	{
		stream << err.to_string();
		return stream;
	}

} // end namespace error
} // end namespace windows
} // end namespace distant