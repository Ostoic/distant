#pragma once

#include <distant/config.hpp>
#include <distant/kernel_objects/process.hpp>
#include <distant/system/information.hpp>
#include <distant/memory/function.hpp>
#include <distant/type_traits.hpp>
#include <distant/support/winapi/thread.hpp>

#include <boost/operators.hpp>
#include <distant/concepts/handleable.hpp>

#include <thread>
#include <iosfwd>
#include "fwd.hpp"

namespace distant::kernel_objects
{
	class remote_thread
		: public boost::equality_comparable<remote_thread>
		, public concepts::handleable<remote_thread>
	{
	public:
		static constexpr auto required_process_rights = process_rights::create_thread | process_rights::query_information | vm_rw_op;

		static unsigned int hardware_concurrency() noexcept;

		class id_t;

	public:
		void kill();

		void join();

		void detach();

		void swap(remote_thread& other) noexcept;

		id_t id() const noexcept;

		bool is_active() const noexcept;

		bool joinable() const noexcept;

		template <process_rights Access = process_rights::all_access>
		process<Access> process() noexcept;

	public:
		constexpr remote_thread() noexcept : id_(0) {}

		explicit remote_thread(std::thread::id id) noexcept;
		explicit remote_thread(std::thread&& thread) noexcept;
		explicit remote_thread(remote_thread::id_t id) noexcept;

		template <typename Fn, typename... Args>
		explicit remote_thread(function<int>, Args&&... args);

		remote_thread(remote_thread&& other) noexcept;
		remote_thread& operator=(remote_thread&& other) noexcept;

		~remote_thread() noexcept;

		remote_thread(const remote_thread&) = delete;
		remote_thread& operator=(const remote_thread&) = delete;

	private:
		void detach_unchecked() noexcept;

		template <class> friend struct concepts::handleable;

		explicit remote_thread(kernel_handle&& handle) noexcept;

	private:
		kernel_handle handle_;
		uint id_;
	};

	class remote_thread::id_t
		: public boost::totally_ordered<remote_thread::id_t>
	{
	public:
		constexpr id_t() noexcept : id_(0) {}

		constexpr id_t(const uint id) noexcept : id_(id) {}

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

	constexpr bool operator==(const remote_thread& lhs, const remote_thread& rhs) noexcept
	{ return lhs.id() == rhs.id(); }

	inline remote_thread current_thread() noexcept
	{ return remote_thread(remote_thread::id_t(::GetCurrentThreadId())); }

} // namespace distant::kernel_objects

namespace distant
{
	using kernel_objects::remote_thread;
	using kernel_objects::current_thread;

	template <>
	struct kernel_object_traits<remote_thread>
		: default_kernel_object_traits
	{
		using id_t = remote_thread::id_t;

		using access_rights_t = thread_rights;

		static constexpr access_rights_t access_rights() noexcept
		{
			return thread_rights::all_access;
		}

		static constexpr access_rights_t access_rights(const remote_thread&) noexcept
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
#include "impl/remote_thread.hxx"
