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

		if (this->id() == thread::id_t(GetCurrentThreadId()))
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

	inline bool thread::is_active() const noexcept
	{
		if (!this->joinable()) return false;

		using namespace std::chrono;
		return wait(*this, milliseconds(0)) == sync::state::timeout;
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
		return this->id() == other.id();
	}

	inline thread::id_t thread::id() const noexcept
	{
		using traits = kernel_object_traits<thread>;
		return thread::id_t{ traits::get_id(handle_.native_handle()) };
	}

	inline thread::thread(std::thread::id id) noexcept
		: thread(thread::id_t(*reinterpret_cast<unsigned int*>(&id)))
	{}

	inline thread::thread(const thread::id_t id) noexcept
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

	inline void thread::detach_unchecked() noexcept
	{
		handle_.close();
	}

} // namespace distant::kernel_objects
