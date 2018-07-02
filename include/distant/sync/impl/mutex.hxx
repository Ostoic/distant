#pragma once
#include "../mutex.hpp"

#include <distant/error/windows_error.hpp>

#include <boost/winapi/wait.hpp>
#include <boost/winapi/mutex.hpp> 

namespace distant::sync
{
// class mutex
	inline mutex::mutex()
		: handle_(boost::winapi::create_anonymous_mutex(nullptr, false))
	{
		if (this->handle_ == nullptr)
			throw windows_error("[sync::mutex::{ctor}] Unnamed CreateMutex failed");
	}

	inline mutex::mutex(const std::wstring& name, const bool create_new)
		: handle_(
			create_new ? 
				boost::winapi::create_mutex(nullptr, false, name.data()) : 
				boost::winapi::open_mutex(static_cast<boost::winapi::DWORD_>(standard_rights::synchronize), false, name.data())
		)
	{
		if (this->handle_ == nullptr)
			throw windows_error("[sync::mutex::{ctor}] Unnamed CreateMutex failed"); 
	}

	inline void mutex::lock()
	{
		const auto result = boost::winapi::WaitForSingleObject(this->handle_.native_handle(), boost::winapi::infinite);
		if (result != boost::winapi::wait_object_0)
			throw windows_error("[sync::mutex::lock] Unable to lock mutex, WaitForSingleObject failed");
	}

	inline bool mutex::try_lock()
	{
		const auto result = boost::winapi::WaitForSingleObject(this->handle_.native_handle(), 0);

		if (result == boost::winapi::wait_object_0)
			return true;

		if (result == boost::winapi::wait_failed)
			throw windows_error("[sync::mutex::try_lock] Unable to lock mutex, WaitForSingleObject failed");

		return false;
	}

	inline void mutex::unlock() noexcept
	{
		boost::winapi::ReleaseMutex(this->handle_.native_handle());
	}

	inline const distant::unsafe_handle& mutex::handle() const noexcept
	{
		return this->handle_;
	}

// class lock_guard
	template <typename Mutex>
	lock_guard<Mutex>::lock_guard(Mutex& mutex, const bool adopt_mutex)
		: mutex_(mutex)
	{
		if (!adopt_mutex)
			this->mutex_.lock();
	}

	template <typename Mutex>
	lock_guard<Mutex>::~lock_guard() noexcept
	{
		this->mutex_.unlock();
	}
}
