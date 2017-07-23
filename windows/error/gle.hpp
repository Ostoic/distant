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

namespace distant {
namespace windows {
namespace error   {

	// Error code class for which every winapi class extends
	class gle
	{
	public:
		using error_type = DWORD;

		// Get the last error code that was recorded
		// See update_gle comment
		error_type get_last_error() const { return m_error; }

	public:
		constexpr gle() : m_error() {}

		gle(gle&& other) : 
			m_error(std::move(other.m_error))
		{}
		
		std::string to_string() const
		{
			
		}

	protected:
		// We use update_gle to explicitly update the gle
		// This is used to record any errors inside gle encapsulations
		// The user may call get_last_error after accessing the interface,
		// to see if any gles were recorded.
		void update_gle() const { m_error = GetLastError(); }

		void update_gle(const gle& other) const
		{ m_error = other.get_last_error(); }

		void update_gle(gle&& other) const
		{ 
			using std::swap; 
			swap(other.m_error, m_error);
		}

		// m_error is mutable so we can still use const member functions
		// in gle-derived classes.
		mutable error_type m_error;
	};

} // end namespace error
} // end namespace windows
} // end namespace distant