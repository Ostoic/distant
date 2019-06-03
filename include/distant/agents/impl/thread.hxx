#pragma once
#include "../thread.hpp"

#include <boost/winapi/thread.hpp>
#include <distant/synch/wait.hpp>
#include <distant/support/winapi/thread.hpp>

namespace distant::agents
{
	inline unsigned int thread
		::hardware_concurrency() noexcept
	{
		return system::number_of_processors();
	}

	inline void thread
		::kill()
	{
		if (this->joinable())
			if (!::TerminateThread(handle_.native_handle(), 0)) // TerminateThread?
				throw winapi_error("[thread::kill] TerminateThread failed");
	}

	inline void thread
		::join()
	{
		if (!this->joinable())
			throw std::invalid_argument("[thread::join] Invalid join on unjoinable thread");

		if (this->id() == thread::id_t(::GetCurrentThreadId()))
			throw std::invalid_argument("[thread::join] Join on current thread, deadlock would occur");

		if (wait(*this) == synch::state::failed)
			throw winapi_error("[thread::join] WaitForSingleObjectEx failed");

		namespace winapi = boost::winapi;
		winapi::DWORD_ exit_code;
		if (GetExitCodeThread(
				handle_.native_handle(),
				&exit_code)
			== 0
		)
			throw winapi_error("[thread::join] GetExitCodeThread failed");

		this->detach_unchecked();
	}

	inline void thread
		::detach()
	{
		if (!this->joinable())
			throw std::invalid_argument("[thread::join] Detach called on non-joinable thread");

		this->detach_unchecked();
	}

	inline void thread::suspend()
	{
		if (::SuspendThread(this->handle_.native_handle()) == -1)
			throw winapi_error("[thread::resume] SuspendThread failed");
	}

	inline void thread::resume()
	{
		if (::ResumeThread(this->handle_.native_handle()) == -1)
			throw winapi_error("[thread::resume] ResumeThread failed");
	}

	inline void thread
		::swap(thread& other) noexcept
	{
		using std::swap;
		swap(handle_, other.handle_);
		swap(id_, other.id_);
	}

	inline bool thread
		::is_active() const noexcept
	{
		if (!this->joinable()) return false;

		using namespace std::chrono;
		return wait(*this, milliseconds(0)) == synch::state::timeout;
	}

	inline bool thread
		::joinable() const noexcept
	{
		return handle_ != nullptr;
	}

	template <process_rights Access>
	process<Access> thread
		::process() noexcept
	{
		return agents::process<Access>{ agents::process<Access>::id_t{ ::GetProcessIdOfThread(handle_.native_handle()) }};
	}

	inline thread::id_t thread
		::id() const noexcept
	{
		return id_;
	}

	inline thread::thread(std::thread::id id) noexcept
		: thread(thread::id_t(*reinterpret_cast<unsigned int*>(&id)))
	{}

	inline thread::thread(std::thread&& thread) noexcept
		: thread(thread.get_id())
	{
		thread.detach();
	}

	inline thread::thread(const thread::id_t id) noexcept
		: handle_(kernel_object_traits<thread>::open(static_cast<uint>(id)))
		, id_(id)
	{}

	template <typename Fn, typename... Args>
	thread::thread(memory::function<int> fn, Args&&... args)
	{
		const address start_address = 0;
		//CreateRemotethread(process.handle().native_handle(), nullptr, 0, , , ,)
		// Remote thread launch on distant::function
		// Todo: CreateRemotethread or RtlCreateUserthread to start a thread in the process at the start of some executable memory (start_address).
		// Todo: Write each argument in the variadic pack to a section of the allocated memory.
		// Todo: Pass in the parameters according to the calling convetion specified by fn.
		// Todo: The thread will then call the desired function fn with the given parameters.
		handle_ = ::CreateRemoteThread(process.handle().native_handle(), nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(static_cast<dword>(start_address)), nullptr, 0, &id_);
	}

	inline thread::thread(thread&& other) noexcept
		: handle_(std::move(other.handle_))
		, id_(other.id_)
	{}

	inline thread& thread
		::operator=(thread&& other) noexcept
	{
		handle_ = std::move(other.handle_);
		id_ = other.id_;
		return *this;
	}

	inline thread
		::~thread() noexcept
	{
		//if (this->joinable())
			//::Terminatethread(handle_.native_handle(), 0); // Terminatethread?
	}

	inline void thread
		::detach_unchecked() noexcept
	{
		handle_.close();
	}

	inline thread::thread(kernel_handle&& handle) noexcept
		: handle_(handle.native_handle())
		, id_(kernel_object_traits<thread>::get_id(handle_.native_handle()))
	{ handle = nullptr; }


} // namespace distant::agents
