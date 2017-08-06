#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <Windows.h>

#include <utility>
#include <string>
#include <iostream>

#include <distant\windows\error\format.hpp>

namespace distant::windows::error {

	// Error code class for which every winapi class extends
	class gle
	{
	private:
		using error_type = DWORD;

	public:
		// Get the last error code that was recorded
		// See update_gle comment
		const gle& get_last_error() const { return *this; }

		error_type get_error_code() const
		{
			return m_error;
		}

		bool success() const
		{
			return get_error_code() == ERROR_SUCCESS;
		}

	public:
		constexpr gle() : m_error() {}

		gle(gle&& other) 
			: m_error(std::move(other.m_error))
		{}

		friend std::ostream& operator <<(std::ostream&, const gle&);

	protected:
		std::string to_string() const
		{ 
			return error::format(get_value()); 
		}

		error_type get_value() const 
		{
			return m_error; 
		}

		// We use update_gle to explicitly update the gle
		// This is used to record any errors inside gle encapsulations
		// The user may call get_last_error after accessing the interface,
		// to see if any gles were recorded.
		void update_gle() const 
		{
			m_error = GetLastError(); 
		}

		void update_gle(const gle& other) const 
		{ 
			m_error = other.get_value();
		}

		void set_last_error(error_type error_code) const 
		{
			m_error = error_code; 
			SetLastError(error_code); 
		}

		// m_error is mutable so we can still use const member functions
		// in gle-derived classes.
		mutable error_type m_error;
	};

	inline std::ostream& operator <<(std::ostream& stream, const gle& err)
	{
		stream << err.to_string();
		return stream;
	}

} // end namespace distant::windows::error 