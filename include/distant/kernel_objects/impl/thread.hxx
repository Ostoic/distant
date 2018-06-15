#pragma once
#include "../thread.hpp"

#include <boost/winapi/thread.hpp>
#include <boost/winapi/wait.hpp>
#include <distant/support/winapi/thread.hpp>

namespace distant::kernel_objects
{
	inline unsigned int thread::hardware_concurrency() noexcept
	{
		return system::number_of_processors();
	}

	inline void thread::join()
	{
		if (!this->joinable())
			throw std::invalid_argument("[thread::join] Invalid join on unjoinable thread");

		if (this->get_id() == thread::id(GetCurrentThreadId()))
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
	}

	inline bool thread::joinable() const noexcept
	{
		return handle_ != nullptr;
	}

	inline const process<thread::required_process_rights>& thread::process() const noexcept
	{
		return *process_;
	}

	inline const distant::handle<thread>& thread::handle() const noexcept
	{
		return handle_;
	}

	inline thread::id thread::get_id() const noexcept
	{
		return thread::id{ boost::winapi::GetThreadId(handle_.native_handle()) };
	}

	inline thread::thread(distant::handle<thread>&& handle) noexcept
		: handle_(std::move(handle))
	{}

	template <typename Fn, typename... Args>
	thread::thread(memory::function<int> fn, Args&&... args)
	{
		address start_address = 0;
		//CreateRemoteThread(process.handle().native_handle(), nullptr, 0, , , ,)
		// Remote thread launch on distant::function
		// Todo: CreateRemoteThread or RtlCreateUserThread to start a thread in the process at the start of some executable memory (start_address).
		// Todo: Write each argument in the variadic pack to a section of the allocated memory.
		// Todo: Pass in the parameters according to the calling convetion specified by fn.
		// Todo: The thread will then call the desired function fn with the given parameters.
		handle_ = ::CreateRemoteThread(process.handle().native_handle(), nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(static_cast<dword>(start_address)), nullptr, 0, &id_);
	}

	inline thread::thread(thread&& other) noexcept
		: handle_(std::move(other.handle_))
	{}

	inline thread& thread::operator=(thread&& other) noexcept
	{
		handle_ = std::move(other.handle_);
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
	inline bool operator==(const thread::id& lhs, const thread::id& rhs) noexcept
	{
		return lhs.id_ == rhs.id_;
	}

	inline bool operator!=(const thread::id& lhs, const thread::id& rhs) noexcept
	{
		return !operator==(lhs, rhs);
	}

	inline bool operator==(const thread& lhs, const thread& rhs) noexcept
	{
		return lhs.get_id() == rhs.get_id();
	}

	inline bool operator!=(const thread& lhs, const thread& rhs) noexcept
	{
		return !(lhs == rhs);
	}
	
} // namespace distant::kernel_objects
