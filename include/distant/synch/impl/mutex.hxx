#pragma once
#include "../mutex.hpp"

#include <distant/error/winapi_error.hpp>
#include <distant/synch/wait.hpp>
#include <boost/winapi/mutex.hpp>

namespace distant::synch
{
//class mutex
//public:
	inline mutex open(const std::wstring& name)
	{
		const auto handle = boost::winapi::open_mutex(
			static_cast<boost::winapi::DWORD_>(standard_rights::synchronize),
			false,
			name.data()
		);

		return mutex{kernel_handle{handle}};
	}

	inline mutex::mutex()
		: handle_{boost::winapi::create_anonymous_mutex(nullptr, false)}
	{
		if (this->handle_ == nullptr)
			throw winapi_error("[synch::mutex::{ctor}] Anonymous CreateMutex failed");
	}

	mutex::mutex(const std::wstring& name)
		: handle_{boost::winapi::create_mutex(nullptr, false, name.data())}
		, name_{name}
	{
		if (this->handle_ == nullptr)
			throw winapi_error("[synch::mutex::{ctor}] Named CreateMutex failed");
	}

	inline void mutex::lock()
	{
		if (wait(*this) != synch::state::signaled)
			throw winapi_error("[synch::mutex::lock] Unable to lock mutex, WaitForSingleObject failed");
	}

	inline bool mutex::try_lock()
	{
		const auto result = wait(*this, std::chrono::milliseconds(0));

		if (result == state::signaled)
			return true;

		if (result == state::failed)
			throw winapi_error("[synch::mutex::try_lock] Unable to lock mutex, WaitForSingleObject failed");

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

	inline const std::wstring& mutex::name() const noexcept
	{
		return name_;
	}

//private:
	inline mutex::mutex(kernel_handle&& handle)
		: handle_{std::move(handle)}
	{}

//class lock_guard
//public:
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
