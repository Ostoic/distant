#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <utility>
#include <string>
#include <iostream>

#include <boost\winapi\basic_types.hpp>
#include <boost\winapi\error_codes.hpp>
#include <boost\winapi\get_last_error.hpp>
#include <distant\support\winapi\last_error.hpp>

#include <distant\error\format.hpp>

namespace distant::error {

	/*struct gle_category : std::error_category
	{
		const char* name() const noexcept override
		{ return "GetLastError"; }

		std::string message(int code) const override
		{ return error::format(code); }
	};*/
	
	// Error code for objects that set GetLastError in the TLS
	class gle
	{
	private:
		using error_type = boost::winapi::DWORD_;

	public:
		// Get the last error code that was recorded
		// See update_gle comment
		const gle& get_last_error() const 
		{ return *this; }

		error_type get_error_code() const
		{ return m_error; }

		bool success() const
		{ return get_error_code() == boost::winapi::ERROR_SUCCESS_; }

	public:
		constexpr gle() : m_error() {}

		gle(gle&& other) : m_error(std::move(other.m_error)) {}

		friend std::ostream& operator <<(std::ostream&, const gle&);

	protected:
		std::string format_gle() const
		{ return error::format(get_error_code()); }

		// We use update_gle to explicitly update the gle
		// This is used to record any errors inside gle encapsulations
		// The user may call get_last_error after accessing the interface,
		// to see if any gles were recorded.
		void update_gle() const 
		{ m_error = boost::winapi::GetLastError(); }

		void update_gle(const gle& other) const 
		{  m_error = other.get_error_code(); }

		void set_success() const
		{ this->set_last_error(boost::winapi::ERROR_SUCCESS_); }

		void set_last_error(error_type error_code) const 
		{
			m_error = error_code; 
			distant::winapi::SetLastError(error_code); 
		}

		// m_error is mutable so we can still use const member functions
		// in gle-derived classes.
		mutable error_type m_error;
	};

	inline std::ostream& operator <<(std::ostream& stream, const gle& err)
	{
		stream << err.format_gle();
		return stream;
	}

} // end namespace distant::error 
