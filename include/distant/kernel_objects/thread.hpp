#pragma once

#include <distant/kernel_objects/process.hpp>
#include <distant/system/information.hpp>
#include <distant/memory/virtual_function.hpp>

namespace distant {
namespace kernel_objects 
{
	class thread
	{
	public:
		static constexpr auto required_access = process_rights::create_thread | process_rights::query_information | vm_rw_op;

	public:
		void join();

		void detach();

		void swap(thread& other) noexcept;

		unsigned int id() const noexcept;

		bool joinable() const noexcept;

		thread() noexcept;

		template <typename Fn, typename... Args>
		explicit thread(const process<required_access>&, memory::virtual_function<int>, Args&&... args);

		thread(thread&& other) noexcept;
		thread& operator=(thread&& other) noexcept;

		~thread() noexcept;

		thread(const thread&) = delete;
		thread& operator=(const thread&) = delete;

	public:
		static unsigned int hardware_concurrency() noexcept
		{
			return distant::system::number_of_processors();
		}

	private:
		void detach_unchecked();

	private:
		handle<thread> handle_;
		unsigned int id_;
	};

	bool operator==(const thread& lhs, const thread& rhs) noexcept;
	bool operator!=(const thread& lhs, const thread& rhs) noexcept;
	
} // namespace kernel_objects

using kernel_objects::thread;

} // namespace distant

// Implementation
#include "impl/thread.hxx"