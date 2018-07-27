#pragma once
#include "../remote_thread.hpp"

#include <boost/winapi/thread.hpp>
#include <distant/sync/wait.hpp>
#include <distant/support/winapi/thread.hpp>

namespace distant::kernel_objects
{
	inline unsigned int remote_thread
		::hardware_concurrency() noexcept
	{
		return system::number_of_processors();
	}

	inline void remote_thread
		::kill()
	{
		if (this->joinable())
			if (!::TerminateThread(handle_.native_handle(), 0)) // TerminateThread?
				throw winapi_error("[thread::kill] TerminateThread failed");
	}

	inline void remote_thread
		::join()
	{
		if (!this->joinable())
			throw std::invalid_argument("[thread::join] Invalid join on unjoinable thread");

		if (this->id() == remote_thread::id_t(::GetCurrentThreadId()))
			throw std::invalid_argument("[thread::join] Join on current thread, deadlock would occur");

		if (wait(*this) == sync::state::failed)
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

	inline void remote_thread
		::detach()
	{
		if (!this->joinable())
			throw std::invalid_argument("[remote_thread::join] Detach called on non-joinable remote_thread");

		this->detach_unchecked();
	}

	inline void remote_thread::suspend()
	{
		if (::SuspendThread(this->handle_.native_handle()) == -1)
			throw winapi_error("[remote_thread::resume] SuspendThread failed");
	}

	inline void remote_thread::resume()
	{
		if (::ResumeThread(this->handle_.native_handle()) == -1)
			throw winapi_error("[remote_thread::resume] ResumeThread failed");
	}

	inline void remote_thread
		::swap(remote_thread& other) noexcept
	{
		using std::swap;
		swap(handle_, other.handle_);
		swap(id_, other.id_);
	}

	inline bool remote_thread
		::is_active() const noexcept
	{
		if (!this->joinable()) return false;

		using namespace std::chrono;
		return wait(*this, milliseconds(0)) == sync::state::timeout;
	}

	inline bool remote_thread
		::joinable() const noexcept
	{
		return handle_ != nullptr;
	}

	template <process_rights Access>
	process<Access> remote_thread
		::process() noexcept
	{
		return kernel_objects::process<Access>{ kernel_objects::process<Access>::id_t{ ::GetProcessIdOfThread(handle_.native_handle()) }};
	}

	inline remote_thread::id_t remote_thread
		::id() const noexcept
	{
		return id_;
	}

	inline remote_thread::remote_thread(std::thread::id id) noexcept
		: remote_thread(remote_thread::id_t(*reinterpret_cast<unsigned int*>(&id)))
	{}

	inline remote_thread::remote_thread(std::thread&& remote_thread) noexcept
		: remote_thread(remote_thread.get_id())
	{
		remote_thread.detach();
	}

	inline remote_thread::remote_thread(const remote_thread::id_t id) noexcept
		: handle_(kernel_object_traits<remote_thread>::open(static_cast<uint>(id)))
		, id_(id)
	{}

	template <typename Fn, typename... Args>
	remote_thread::remote_thread(memory::function<int> fn, Args&&... args)
	{
		const address start_address = 0;
		//CreateRemoteremote_thread(process.handle().native_handle(), nullptr, 0, , , ,)
		// Remote remote_thread launch on distant::function
		// Todo: CreateRemoteremote_thread or RtlCreateUserremote_thread to start a remote_thread in the process at the start of some executable memory (start_address).
		// Todo: Write each argument in the variadic pack to a section of the allocated memory.
		// Todo: Pass in the parameters according to the calling convetion specified by fn.
		// Todo: The remote_thread will then call the desired function fn with the given parameters.
		handle_ = ::CreateRemoteThread(process.handle().native_handle(), nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(static_cast<dword>(start_address)), nullptr, 0, &id_);
	}

	inline remote_thread::remote_thread(remote_thread&& other) noexcept
		: handle_(std::move(other.handle_))
		, id_(other.id_)
	{}

	inline remote_thread& remote_thread
		::operator=(remote_thread&& other) noexcept
	{
		handle_ = std::move(other.handle_);
		id_ = other.id_;
		return *this;
	}

	inline remote_thread
		::~remote_thread() noexcept
	{
		//if (this->joinable())
			//::Terminateremote_thread(handle_.native_handle(), 0); // Terminateremote_thread?
	}

	inline void remote_thread
		::detach_unchecked() noexcept
	{
		handle_.close();
	}

	inline remote_thread::remote_thread(kernel_handle&& handle) noexcept
		: handle_(handle.native_handle())
		, id_(kernel_object_traits<remote_thread>::get_id(handle_.native_handle()))
	{ handle = nullptr; }


} // namespace distant::kernel_objects
