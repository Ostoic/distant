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

	inline void thread::kill()
	{
		if (this->joinable())
			::TerminateThread(handle_.native_handle(), 0); // TerminateThread?
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
		using std::swap;
		swap(handle_, other.handle_);
	}

	inline bool thread::joinable() const noexcept
	{
		return handle_ != nullptr;
	}

	template <process_rights Access>
	process<Access> thread::process()
	{
		return process<Access>{::GetProcessIdOfThread(handle_.native_handle())};
	}

	inline const distant::kernel_handle& thread::handle() const noexcept
	{
		return handle_;
	}

	inline bool thread::equals(const thread& other) const noexcept
	{
		return this->get_id() == other.get_id();
	}

	inline thread::id thread::get_id() const noexcept
	{
		using traits = kernel_object_traits<thread>;
		return thread::id{ traits::get_id(handle_.native_handle()) };
	}

	inline thread::thread(const thread::id id) noexcept
		: handle_(kernel_object_traits<thread>::open(static_cast<uint>(id)))
	{}

	template <typename Fn, typename... Args>
	thread::thread(memory::function<int> fn, Args&&... args)
	{
		const address start_address = 0;
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
		//if (this->joinable())
			//::TerminateThread(handle_.native_handle(), 0); // TerminateThread?
	}

	inline void thread::detach_unchecked()
	{
		handle_.close();
	}
	
} // namespace distant::kernel_objects
