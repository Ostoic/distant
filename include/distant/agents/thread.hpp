#pragma once

#include <distant/config.hpp>
#include <distant/agents/process.hpp>
#include <distant/system/information.hpp>
#include <distant/memory/function.hpp>
#include <distant/type_traits.hpp>
#include <distant/support/winapi/thread.hpp>

#include <boost/operators.hpp>
#include <distant/concepts/handleable.hpp>

#include <thread>
#include <iosfwd>
#include "fwd.hpp"

namespace distant::agents
{
	class thread
		: public boost::equality_comparable<thread>
		, public concepts::handleable<thread>
	{
	public:
		static constexpr auto required_process_rights = process_rights::create_thread | process_rights::query_information | vm_rw_op;

		static unsigned int hardware_concurrency() noexcept;

		static thread launch();

		class id_t;

	public:
		/// @brief Terminate the thread.
		void kill();

		/// @brief Blocks the current thread until the thread finishes its execution.
		void join();

		void detach();

		/// @brief Suspends the thread's execution
		void suspend();

		/// @brief Resume execution of the thread, if suspended.
		void resume();

		void swap(thread& other) noexcept;

		/// @brief Get the thread's unique identifier.
		/// @return A value of type thread::id_t identifying the thread.
		id_t id() const noexcept;

		/// @brief Determine if the given thread is active or not.
		/// @return true if the thread is active, and false otherwise.
		bool is_active() const noexcept;

		/// @brief Checks if the thread identifies an active thread of execution. A thread that has finished
		/// @brief executing code, but has not yet been joined is still considered an active thread of execution and is therefore joinable.
		/// @return true if the thread object identifies an active thread of execution, false otherwise
		bool joinable() const noexcept;

		template <process_rights Access = process_rights::all_access>
		process<Access> process() noexcept;

	public:
		constexpr thread() noexcept : id_(0) {}

		explicit thread(std::thread::id id) noexcept;
		explicit thread(std::thread&& thread) noexcept;
		explicit thread(thread::id_t id) noexcept;

		template <typename Fn, typename... Args>
		explicit thread(function<int>, Args&&... args);

		thread(thread&& other) noexcept;
		thread& operator=(thread&& other) noexcept;

		~thread() noexcept;

		thread(const thread&) = delete;
		thread& operator=(const thread&) = delete;

	private:
		void detach_unchecked() noexcept;

		template <class> friend struct concepts::handleable;

		explicit thread(kernel_handle&& handle) noexcept;

	private:
		kernel_handle handle_;
		uint id_;
	};

	class thread::id_t
		: public boost::totally_ordered<thread::id_t>
	{
	public:
		constexpr id_t() noexcept : id_(0) {}

		constexpr id_t(const uint id) noexcept : id_(id) {}

		explicit id_t(const std::thread::id id) noexcept : id_(*reinterpret_cast<const uint*>(&id)) {}

		constexpr explicit operator uint() const noexcept { return id_; }

		operator std::thread::id() const noexcept { return *reinterpret_cast<const std::thread::id*>(&id_); }

		friend constexpr bool operator==(const id_t& lhs, const id_t& rhs) noexcept
		{ return lhs.id_ == rhs.id_; }

		friend constexpr bool operator<(const id_t& lhs, const id_t& rhs) noexcept
		{ return lhs.id_ < rhs.id_; }

		template <typename CharT, typename TraitsT>
		friend std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& stream, const id_t id)
		{ return (stream << static_cast<uint>(id)); }

	private:
		uint id_;
	};

	constexpr bool operator==(const thread& lhs, const thread& rhs) noexcept
	{ return lhs.id() == rhs.id(); }

	inline thread current_thread() noexcept
	{ return thread(thread::id_t(::GetCurrentThreadId())); }

} // namespace distant::agents

namespace distant
{
	using agents::thread;
	using agents::current_thread;

	template <>
	struct kernel_object_traits<thread>
		: default_kernel_object_traits
	{
		using id_t = thread::id_t;

		using access_rights_t = thread_rights;

		static constexpr access_rights_t access_rights() noexcept
		{
			return thread_rights::all_access;
		}

		static constexpr access_rights_t access_rights(const thread&) noexcept
		{
			return access_rights();
		}

		static uint get_id(const native_handle_t native_handle) noexcept
		{
			return boost::winapi::GetThreadId(native_handle);
		}

		static bool is_valid_handle(const native_handle_t native_handle) noexcept
		{
			return native_handle != nullptr;
		}

		static native_handle_t open(const boost::winapi::DWORD_ id, access_rights_t access = access_rights()) noexcept
		{
			using under_t = std::underlying_type_t<access_rights_t>;
			return boost::winapi::OpenThread(static_cast<under_t>(access), false, id);
		}
	};
}

// Implementation
#include "impl/thread.hxx"
