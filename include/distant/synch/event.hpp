#pragma once

#include <string>

#include <distant/scoped_handle.hpp>

namespace distant::synch
{
	class event
	{
	public:
		/// @brief Create an event object with a randomly-generated name.
		/// @remark The event is constructed in an unsignaled state.
		/// @exception Throws a winapi_error in the event that an API call fails.
		explicit event();
		
		/// @brief Create a system-wide event object of the given name.
		/// @remark The event is constructed in an unsignaled state.
		/// @exception Throws a winapi_error in the event that an API call fails.
		explicit event(const std::wstring& name);

		/// @brief Get the handle to the event.
		/// @return its handle.
		const kernel_handle& handle() const noexcept;

		/// @brief Get the name of the event.
		/// @return the event's name.
		const std::wstring& name() const noexcept;

	private:
		kernel_handle handle_;
		const std::wstring name_;
	};

} // namespace distant::synch
