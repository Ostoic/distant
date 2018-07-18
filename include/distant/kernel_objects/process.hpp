// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

/***
 * @file Contains the interface for distant::process.
 */

#include <distant/kernel_objects/unsafe_process.hpp>

namespace distant::kernel_objects
{
	/// @brief distant::process represents a process, and has static type checking on the process access rights.
	/// @tparam AccessRights the desired permissions to open the process with.
	/// @see process_rights
	template <process_rights AccessRights = process_rights::all_access>
	class process
		: private unsafe_process
		, boost::equality_comparable<process<AccessRights>>
	{
	private:
		/// @brief SFINAE for access permissions
		template <access_rights::process Required, typename Return = void>
		using require_permission = std::enable_if_t<check_permission(AccessRights, Required), Return>;

	public: // interface
		// Import the unsafe_process interface.
		using unsafe_process::operator bool;
		using unsafe_process::is_being_debugged;
		using unsafe_process::access_rights;
		using unsafe_process::handle;
		using unsafe_process::is_zombie;
		using unsafe_process::valid;
		using unsafe_process::id;
		using unsafe_process::detach;
		using unsafe_process::joinable;
		using id_t = unsafe_process::id_t;

		/// @brief Terminate the process.
		/// @see process_rights
		/// @remark Requires \a AccessRights >= \a terminate.
		template <typename Return = void>
		auto kill()
			-> require_permission<process_rights::terminate, Return>;

		template <typename Return = void>
		auto join()
			-> require_permission<process_rights::synchronize, Return>;

		/// @brief Get number of handle s opened in the process
		/// @see process_rights
		/// @return unsigned int: the number of handles
		/// @remark Requires \a AccessRights >= \a query_limited_information.
		template <typename Return = std::size_t>
		auto handle_count() const
			-> require_permission<process_rights::query_limited_information, Return>;

		/// @brief Query the process handle to see if it is still active
		/// @see process_rights
		/// @return bool: \a true if the process is active, and \a false otherwise
		/// @remark Requires \a AccessRights >= \a syncronize.
		template <typename Return = bool>
		auto is_active() const
			-> require_permission<process_rights::synchronize, Return>;

		/// @brief Test if the process is running under the WOW64 emulator.
		/// <br> If the process has been compiled to run on 32-bit system and
		/// is being run on a 64-bit system, it will be emulated.
		/// @see process_rights
		/// @return bool: \a true if the process is being emulated, and \a false otherwise.
		/// @remark Requires \a AccessRights >= \a query_limited_information or \a AccessRights >= \a query_information.
		template <typename Return = bool>
		auto is_32bit() const
			-> require_permission<process_rights::query_information | process_rights::query_limited_information, Return>;

		/// @brief Test if the process is being run in 64bit.
		/// @see process_rights
		/// @return  bool: \a true if the process is being run in 64bit mode, and \a false otherwise.
		/// @remark Requires \a AccessRights >= \a query_limited_information or \a AccessRights >= \a query_information.
		template <typename Return = bool>
		auto is_64bit() const
			-> require_permission<process_rights::query_information | process_rights::query_limited_information, Return>;

		/// @brief Get the executable name of the process
		/// @see process_rights
		/// @return std::wstring: the executable image name for the given process.
		/// @remark Requires \a AccessRights >= \a query_limited_information or \a AccessRights >= \a query_information.
		template <typename Return = std::wstring>
		auto name() const
			-> require_permission<process_rights::query_information | process_rights::query_limited_information, Return>;

		/// @brief Get the file path (in WIN32 format) of the process
		/// @see process_rights
		/// @return filesystem::path: the absolute path to the executable image for the given process.
		/// @remark Requires \a AccessRights >= \a query_limited_information or \a AccessRights >= \a query_information.
		template <typename Return = filesystem::path>
		auto file_path() const
			-> require_permission<process_rights::query_information | process_rights::query_limited_information, Return>;

		/// @brief Allows implicit conversion to a process with a lower level of access.
		template <process_rights OtherAccess, typename = std::enable_if_t<(OtherAccess <= AccessRights)>>
		operator process<OtherAccess>&() noexcept;

		/// @brief Allows implicit conversion to a process with a lower level of access.
		template <process_rights OtherAccess, typename = std::enable_if_t<(OtherAccess <= AccessRights)>>
		operator const process<OtherAccess>&() const noexcept;

	public: // {ctor}
		/// @brief Default process constructor
		constexpr process() noexcept;

		/// @brief Open process by id
		explicit process(id_t id) noexcept;

		/// @brief Move constructible
		process(process&& other) noexcept;

		/// @brief Move assignable
		process& operator=(process&& other) noexcept;

		friend constexpr bool operator==(const process& lhs, const process& rhs) noexcept
		{ return static_cast<const unsafe_process&>(lhs) == static_cast<const unsafe_process&>(rhs); }

	private:
		explicit process(kernel_handle&& handle) noexcept;

		friend class memory_status;
		friend process<> current_process() noexcept;

	}; // class process

	/// @brief Create a new process
	// Todo: Implement
	template <process_rights Access>
	process<Access> launch();

	/// @brief Get the current process.
	/// @tparam Access the desired access rights to the process.
	/// @return distant::process: a process object representing the current process.
	template <process_rights Access>
	process<Access> current_process() noexcept;

	/// @brief Get the current process.
	/// @return distant::process: a process object representing the current process with the highest \a AccessRights.
	process<process_rights::all_access> current_process() noexcept;

} // namespace distant::kernel_objects

namespace distant
{
	using kernel_objects::process;
	using kernel_objects::current_process;

	template <process_rights Access>
	struct kernel_object_traits<process<Access>>
		: detail::process_traits
	{
		using id_t = typename process<Access>::id_t;

		using process_traits::access_rights_t;
		using process_traits::access_rights;
		using process_traits::get_id;
		using process_traits::is_valid_handle;
		using process_traits::open;

		static constexpr access_rights_t access_rights(const process<Access>&) noexcept
		{ return process_traits::access_rights(); }
	};
}

// Implementation
#include <distant/kernel_objects/impl/process.hxx>
