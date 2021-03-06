// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <cstddef>
#include <string>

#include <distant/config.hpp>
#include <distant/type_traits.hpp>
#include <distant/scoped_handle.hpp>
#include <distant/support/filesystem.hpp>

#include <distant/concepts/handleable.hpp>
#include <boost/operators.hpp>

namespace distant::agents
{
	/// @brief Base type of distant::process
	/// This version does not have static access_rights checking
	class basic_process
		: public concepts::handleable<basic_process>
		, public boost::equality_comparable<basic_process>
	{
	public:
		class id_t;

	public: // interface

		/// @brief Terminate the process
		void kill();

		void join();

		void detach();

		void suspend();

		void resume();

		//NtSuspendProcess pfnNtResumeProcess = (NtSuspendProcess)GetProcAddress(GetModuleHandle("ntdll"), "NtResumeProcess");
		//pfnNtResumeProcess(WoW::handle);

		bool joinable() const noexcept;

		/// @brief Query the process handle to see if it is still active
		/// @return true if the process is active, false otherwise.
		bool is_active() const;

		/// @brief Test if the process is running under the WOW64 emulator.
		/// If the process has been compiled to run on 32-bit system and
		/// is being run on a 64-bit system, it will be emulated.
		/// @return true if the process is being emulated, and false if not.
		bool is_32bit() const;

		/// @brief Test if the process is being run in 64bit.
		/// @return true if the process is being run in 64bit mode, and false if not.
		bool is_64bit() const;

		/// @brief Get the name of the process executable.
		/// @return string containing the name.
		std::wstring name() const;

		/// @brief Get the path to the process executable.
		/// @return the path.
		filesystem::path file_path() const;

		/// @brief Test if the process is being debugged by another process.
		/// @return true if the process is being debugged, and false if it is not.
		bool is_being_debugged() const;

		/// @brief Test if the process is a "zombie" process.
		/// A zombie process in this case is one that is not listed upon process list enumeration,
		/// but still has an active handle in the operating system.
		/// @return true if the process is a zombie.
		bool is_zombie() const;

		/// Get number of handle s opened in the process
		/// @return the number of handles
		std::size_t handle_count() const;

		/// @brief Retrieve the process id.
		/// @return the process id.
		id_t id() const noexcept;

		/// @brief Get the access rights that were used to open the current process
		/// @return process access_rights indicating the level of access we have to the process.
		process_rights access_rights() const noexcept { return access_rights_; }

		/// @brief Test if the process kernel_object is valid
		/// @return true if the process is valid, false otherwise.
		bool valid() const noexcept;

	public: // {ctor}
		/// @brief Construct an empty process
		constexpr basic_process() noexcept;

		/// @brief Open process by id
		/// @param id the pid (process id) of the process to open.
		/// @param access the requested access rights to open the process with.
		explicit basic_process(id_t id, process_rights access = process_rights::all_access) noexcept;

		basic_process(basic_process&& other) noexcept; // move constructible
		basic_process& operator=(basic_process&& other) noexcept; // move assignable

	private:
		void detach_unchecked() noexcept;

	protected:
		explicit basic_process(kernel_handle&& handle, process_rights access) noexcept;

		template <class> friend struct concepts::handleable;

		kernel_handle handle_;
		process_rights access_rights_;

	}; // class basic_process

	class basic_process::id_t
		: public boost::totally_ordered<basic_process::id_t>
	{
	public:
		constexpr id_t() noexcept : id_(0) {}

		constexpr explicit operator uint() const noexcept { return id_; }

		constexpr id_t(const uint id) : id_(id) {}

		friend constexpr bool operator==(const id_t lhs, const id_t rhs) noexcept
		{ return lhs.id_ == rhs.id_; }

		friend constexpr bool operator<(const id_t lhs, const id_t rhs) noexcept
		{ return lhs.id_ < rhs.id_; }

		template <typename CharT, typename TraitsT>
		friend std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& stream, const id_t id)
		{ return (stream << static_cast<uint>(id)); }

	private:
		uint id_;

		friend class basic_process;
	};

	constexpr bool operator==(const basic_process& lhs, const basic_process& rhs) noexcept
	{ return lhs.id() == rhs.id(); }

} // namespace distant::agents

namespace distant
{
	namespace detail
	{
		struct process_traits : public default_kernel_object_traits
		{
			using access_rights_t = process_rights;

			static constexpr access_rights_t access_rights() noexcept
			{
				return access_rights_t::all_access;
			}

			static uint get_id(const native_handle_t native_handle) noexcept
			{
				return boost::winapi::GetProcessId(native_handle);
			}

			static bool is_valid_handle(const native_handle_t native_handle) noexcept
			{
				// snapshot_iterator uses this to test for zombies during iterator increments.
				return native_handle != nullptr && ::GetProcessVersion(get_id(native_handle)) != 0;
			}

			static native_handle_t open(const boost::winapi::DWORD_ id, const access_rights_t access = access_rights()) noexcept
			{
				using under_t = std::underlying_type_t<access_rights_t>;
				return boost::winapi::OpenProcess(static_cast<under_t>(access), false, id);
			}
		};
	}

	template <>
	struct kernel_object_traits<agents::basic_process>
		: public detail::process_traits
	{
		using id_t = agents::basic_process::id_t;

		using process_traits::access_rights_t;
		using process_traits::access_rights;
		using process_traits::get_id;
		using process_traits::is_valid_handle;
		using process_traits::open;

		static access_rights_t access_rights(const agents::basic_process& process) noexcept
		{
			return process.access_rights();
		}
	};
}

#include "impl/basic_process.hxx"