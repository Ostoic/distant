#pragma once

#include <distant/config.hpp>
#include <distant/kernel_objects/process.hpp>
#include <distant/system/information.hpp>
#include <distant/memory/function.hpp>
#include <distant/type_traits.hpp>

#include <distant/concepts/equality_comparable.hpp>
#include <distant/support/winapi/thread.hpp>

#include <thread>
#include <iosfwd>
#include "fwd.hpp"

namespace distant::kernel_objects 
{
	class thread 
		: public concepts::equality_comparable<thread>
	{
	public:
		static constexpr auto required_process_rights = process_rights::create_thread | process_rights::query_information | vm_rw_op;

		static unsigned int hardware_concurrency() noexcept;

		class id;

	public:
		void kill();

		void join();

		void detach();

		void swap(thread& other) noexcept;

		id get_id() const noexcept;

		bool joinable() const noexcept;

		template <process_rights Access>
		process<Access> process();

		const kernel_handle& handle() const noexcept;

		bool equals(const thread& other) const noexcept;

	public:
		constexpr thread() noexcept {}

		explicit thread(thread::id id) noexcept;

		template <typename Fn, typename... Args>
		explicit thread(function<int>, Args&&... args);

		thread(thread&& other) noexcept;
		thread& operator=(thread&& other) noexcept;

		~thread() noexcept;

		thread(const thread&) = delete;
		thread& operator=(const thread&) = delete;

	private:
		void detach_unchecked();

	private:
		kernel_handle handle_;
	};

	class thread::id 
		: public concepts::equality_comparable<thread::id>
	{
	public:
		id() noexcept : id_(0) {}

		bool equals(const id other) const noexcept { return id_ == other.id_; }

		id(const uint id) : id_(id) {}

		explicit operator uint() const noexcept { return id_; }

		template <typename CharT, typename TraitsT>
		friend std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& stream, const id id)
		{ return (stream << static_cast<uint>(id)); }

	private:
		uint id_;
	};

} // namespace distant::kernel_objects

namespace distant
{
	using kernel_objects::thread;

	template <>
	struct kernel_object_traits<thread>
		: default_kernel_object_traits
	{
		using id_t = thread::id;

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