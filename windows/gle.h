#pragma once

#include <Windows.h>

#include <utility>

namespace distant {
namespace windows {

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

	protected:
		// We use update_gle to explicitly update the gle
		// This is used to record any errors inside gle encapsulations
		// The user may call get_last_error after accessing the interface,
		// to see if any gles were recorded.
		void update_gle() { m_error = GetLastError(); }

		void update_gle(const gle& other) 
		{ m_error = other.get_last_error(); }

		void update_gle(gle&& other) 
		{ 
			using std::swap; 
			swap(other.m_error, m_error);
		}

		error_type m_error;
	};

} // end namespace windows
} // end namespace distant