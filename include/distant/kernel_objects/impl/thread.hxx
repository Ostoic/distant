#pragma once
#include "../thread.hpp"

#include <boost/winapi/thread.hpp>
#include <boost/winapi/wait.hpp>

namespace distant::kernel_objects
{
	inline unsigned int thread::hardware_concurrency() noexcept
	{
		return distant::system::number_of_processors();
	}

	inline void thread::join()
	{
		if (!this->joinable())
			throw std::invalid_argument("[thread::join] Invalid join on non-joinable thread");

		if (this->id_ == GetCurrentThreadId())
			throw std::invalid_argument("[thread::join] Join on current thread, deadlock would occur");

		namespace winapi = boost::winapi;
		if (winapi::WaitForSingleObjectEx(
				handle_.native_handle(),
				winapi::infinite,
				false) 
			== winapi::wait_failed
		)
			throw windows_error("[thread::join] WaitForSingleObjectEx failed");

		winapi::DWORD_ exit_code;
		if (GetExitCodeThread(
				handle_.native_handle(),
				&exit_code) 
			== 0
		)
			throw windows_error("[thread::join] GetExitCodeThread failed");

		this->detach_unchecked();
	}

	inline void thread::detach()
	{
		if (!this->joinable())
			throw std::invalid_argument("[thread::join] Detach called on non-joinable thread");

		this->detach_unchecked();
	}

	inline void thread::swap(thread& other) noexcept
	{
		handle_.swap(other.handle_);
		std::swap(id_, other.id_);
	}

	inline bool thread::joinable() const noexcept
	{
		return handle_ != nullptr;
	}

	inline const distant::handle<thread>& thread::handle() const noexcept
	{
		return handle_;
	}


	inline unsigned int thread::id() const noexcept
	{
		return this->id_;
	}

	inline thread::thread() noexcept
		: handle_()
		, id_(0)
	{}

	template <typename Fn, typename... Args>
	thread::thread(const process<process_access>& process, memory::function<int> fn, Args&&... args)
	{
		//CreateRemoteThread(process.handle().native_handle(), nullptr, 0, , , ,)
		// Remote thread launch on distant::function
		handle_ = ::CreateRemoteThread(process.handle().native_handle(), nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(static_cast<dword>(fn.address())), nullptr, 0, &id_);
		
	}

	inline thread::thread(thread&& other) noexcept
		: handle_(std::move(other.handle_))
		, id_(other.id_)
	{}

	inline thread& thread::operator=(thread&& other) noexcept
	{
		handle_ = std::move(other.handle_);
		id_ = other.id_;
		return *this;
	}

	inline thread::~thread() noexcept
	{
		if (this->joinable())
			TerminateThread(handle_.native_handle(), 0); // TerminateThread?
	}

	inline void thread::detach_unchecked()
	{
		handle_.close();
	}

//free:
	inline bool operator==(const thread& lhs, const thread& rhs) noexcept
	{
		return lhs.id() == rhs.id();
	}

	inline bool operator!=(const thread& lhs, const thread& rhs) noexcept
	{
		return !(lhs == rhs);
	}
	
} // namespace distant::kernel_objects
