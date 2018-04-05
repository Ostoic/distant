#pragma once

/*!
@file Defines distant::kernel::process

@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant/kernel/process_base.hpp>

namespace distant
{
	namespace kernel
	{
		/// Representation of an executable program
		template <access_rights::process AccessFlags>
		class process : private process_base
		{
		public: // interface
			// Import the process_base interface.
			using process_base::operator bool;
			using process_base::is_being_debugged;
			using process_base::access_rights;
			using process_base::get_handle;
			using process_base::is_zombie;
			using process_base::valid;
			using process_base::id;

			/// @brief Terminate the process
			/// @remark Function is disabled if the process does not have the \a termiante or query_information process right.
			template <typename Return = void>
			auto kill()
				-> std::enable_if_t<check_permission(AccessFlags, process_rights::terminate), Return>;

			/// @brief Query the process handle to see if it is still active
			/// @remark Function is disabled if the process does not have the \a synchronize or query_information process right.
			/// @return true if the process is active, and false otherwise
			template <typename Return = bool>
			auto is_active() const
				-> std::enable_if_t<check_permission(AccessFlags, process_rights::synchronize), Return>;

			/// @brief Test if the process is running under the WOW64 emulator.
			/// If the process has been compiled to run on 32-bit system and
			/// is being run on a 64-bit system, it will be emulated.
			/// @remark Function is disabled if the process does not have the \a query_limited_information or query_information process right.
			/// @return true if the process is being emulated, and false if not.
			template <typename Return = bool>
			auto is_32bit() const
				-> std::enable_if_t<
					check_permission(AccessFlags, process_rights::query_information) ||
					check_permission(AccessFlags, process_rights::query_limited_information), 
				Return>;

			/// @brief Test if the process is being run in 64bit.
			/// @remark Function is disabled if the process does not have the \a query_limited_information or query_information process right.
			/// @return true if the process is being run in 64bit mode, and false if not.
			template <typename Return = bool>
			auto is_64bit() const
				-> std::enable_if_t<
					check_permission(AccessFlags, process_rights::query_information) ||
					check_permission(AccessFlags, process_rights::query_limited_information),
				Return>;

			/// @brief Get the executable name of the process
			/// @remark Function is disabled if the process does not have the \a query_limited_information or query_information process right.
			/// @return std::wstring containing the executable name of the process
			template <typename Return = std::wstring>
			auto filename() const
				-> std::enable_if_t<
					check_permission(AccessFlags, process_rights::query_information) ||
					check_permission(AccessFlags, process_rights::query_limited_information),
				Return>;

			/// @brief Get the file path (in WIN32 format) of the process
			/// @remark Function is disabled if the process does not have the \a query_limited_information or query_information process right.
			/// @return std::wstring containing the file path of the process
			template <typename Return = filesystem::path>
			auto file_path() const
				-> std::enable_if_t<
					check_permission(AccessFlags, process_rights::query_information) ||
					check_permission(AccessFlags, process_rights::query_limited_information),
				Return>;


			template <access_rights::process OtherFlag,
			          typename = std::enable_if_t<check_permission(AccessFlags, OtherFlag)>>
			operator process<OtherFlag>&() noexcept;

			template <access_rights::process OtherFlag,
			          typename = std::enable_if_t<check_permission(AccessFlags, OtherFlag)>>
			operator const process<OtherFlag>&() const noexcept;

			/// @brief Query the process for memory information 
			/// @return memory_status object used to query for process information
			//auto memory_status() const;

		public: // {ctor}
			/// Default process constructor
			constexpr process() noexcept;

			/// Open process by id
			explicit process(std::size_t id) noexcept;

			process(process&& other) noexcept; /// move constructible
			process& operator =(process&& other) noexcept; /// move assignable

			explicit process(handle<process>&& handle) noexcept;

		private:
			friend class memory_status;
		}; // end class process

		/// @brief Create a new process
		template <access_rights::process Access>
		process<Access> launch();

		/// @brief Get the current process.
		/// @return distant::process_base object containing the current process.
		template <access_rights::process T = access_rights::process::all_access>
		process<T> current_process() noexcept;
	} // end namespace kernel

	using kernel::process;
	using kernel::current_process;
} // end namespace distant

// Implementation 
#include <distant/kernel/impl/process.hxx>
