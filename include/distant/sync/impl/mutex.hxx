#pragma once
#include "../mutex.hpp"

#include <distant/error/winapi_error.hpp>
#include <distant/sync/wait.hpp>
#include <boost/winapi/mutex.hpp>

namespace distant::sync
{
// class mutex
	inline mutex::mutex()
		: handle_(boost::winapi::create_anonymous_mutex(nullptr, false))
	{
		if (this->handle_ == nullptr)
			throw winapi_error("[sync::mutex::{ctor}] Unnamed CreateMutex failed");
	}

	inline mutex::mutex(const std::wstring& name, const bool create_new)
		: handle_(
			create_new ?
				boost::winapi::create_mutex(nullptr, false, name.data()) :
				boost::winapi::open_mutex(static_cast<boost::winapi::DWORD_>(standard_rights::synchronize), false, name.data())
		)
	{
		if (this->handle_ == nullptr)
			throw winapi_error("[sync::mutex::{ctor}] Unnamed CreateMutex failed");
	}

	inline void mutex::lock()
	{
		if (wait(*this) != sync::state::signaled)
			throw winapi_error("[sync::mutex::lock] Unable to lock mutex, WaitForSingleObject failed");
	}

	inline bool mutex::try_lock()
	{
		const auto result = wait(*this, std::chrono::milliseconds(0));

		if (result == state::signaled)
			return true;

		if (result == state::failed)
			throw winapi_error("[sync::mutex::try_lock] Unable to lock mutex, WaitForSingleObject failed");

		return false;
	}

	inline void mutex::unlock() noexcept
	{
		boost::winapi::ReleaseMutex(handle_.native_handle());
	}

	inline const kernel_handle& mutex::handle() const noexcept
	{
		return handle_;
	}

// class lock_guard
	template <typename Mutex>
	lock_guard<Mutex>::lock_guard(Mutex& mutex, const bool adopt_mutex)
		: mutex_(mutex)
	{
		if (!adopt_mutex)
			mutex_.lock();
	}

	template <typename Mutex>
	lock_guard<Mutex>::~lock_guard() noexcept
	{
		mutex_.unlock();
	}
}
