#pragma once

#include <distant/config.hpp>
#include <distant/kernel_objects/process.hpp>
#include <distant/system/information.hpp>
#include <distant/memory/function.hpp>
#include <distant/type_traits.hpp>

#include <thread>
#include <iosfwd>
#include "fwd.hpp"

namespace distant
{
	using kernel_objects::thread;

	template <>
	struct kernel_object_traits<thread>
		: default_kernel_object_traits<thread>
	{};
}

namespace distant::kernel_objects 
{
	class thread
	{
	public:
		using handle_type = typename kernel_object_traits<thread>::handle_type;

	public:
		static constexpr auto required_process_rights = process_rights::create_thread | process_rights::query_information | vm_rw_op;

		static unsigned int hardware_concurrency() noexcept;

		class id;

	public:
		void join();

		void detach();

		void swap(thread& other) noexcept;

		id get_id() const noexcept;

		bool joinable() const noexcept;

		const distant::process<required_process_rights>& process() const noexcept;

		const distant::unsafe_handle& handle() const noexcept;

	public:
		thread() noexcept = default;

		explicit thread(distant::unsafe_handle&& handle) noexcept;

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
		handle_type handle_;
		distant::process<required_process_rights>* process_;
	};

	class thread::id
	{
	public:
		id() noexcept : id_(0) {}

	private:
		explicit id(const uint id) : id_(id) {}

	private:
		uint id_;

		friend class thread;

		template <typename CharT, typename TraitsT>
		friend std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& stream, thread::id id);

		friend bool operator==(const thread::id& lhs, const thread::id& rhs) noexcept;
	};

	template <typename CharT, typename TraitsT>
	std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& stream, const thread::id id)
	{
		stream << id.id_;
		return stream;
	}

	bool operator==(const thread::id& lhs, const thread::id& rhs) noexcept;
	bool operator!=(const thread::id& lhs, const thread::id& rhs) noexcept;

	bool operator==(const thread& lhs, const thread& rhs) noexcept;
	bool operator!=(const thread& lhs, const thread& rhs) noexcept;
	
} // namespace distant::kernel_objects

// Implementation
#include "impl/thread.hxx"