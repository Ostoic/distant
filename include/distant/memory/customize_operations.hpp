#pragma once

#include <string>
#include <distant/memory/address.hpp>

namespace distant::memory
{
	/// @brief Provides customization points for memory operations.
	namespace customize
	{
		/// Todo: Make this a single traits class and specialize for each needed customization
		/// @brief memory::write standard layout customization point.
		template <typename T>
		struct write
		{
			template <typename AddressT>
			static void invoke(const process<vm_w_op>& proc, const address<AddressT> address, T&& x)
			{
				SIZE_T bytes_written = 0;
				T buffer = std::forward<T>(x);
				if (!::WriteProcessMemory(proc.handle().native_handle(), reinterpret_cast<LPVOID>(static_cast<AddressT>(address)), &buffer, sizeof(T), &bytes_written))
					throw winapi_error("[memory::write] WriteProcessMemory failed, " + std::to_string(bytes_written) + " bytes written");
			}
		};

		/// @brief memory::write std::string customization point.
		template <>
		struct write<std::string>
		{
			template <typename AddressT>
			static void invoke(const process<vm_w_op>& process, const address<AddressT> address, const std::string& string)
			{
				SIZE_T bytes_written = 0;
				if (!WriteProcessMemory(
					process.handle().native_handle(),
					reinterpret_cast<boost::winapi::LPVOID_>(static_cast<AddressT>(address)),
					string.data(),
					string.size() + 1, 
					&bytes_written
				))
					throw winapi_error("[memory::write<std::string>] WriteProcessMemory failed, " + std::to_string(bytes_written) + " bytes written");
			}
		};

		/// @brief memory::read standard layout customization point.
		template <typename T>
		struct read
		{
			static_assert(std::is_standard_layout<T>::value);

			template <typename AddressT>
			static T invoke(const process<vm_read>& process, const address<AddressT> address, const std::size_t size)
			{
				T result;
				SIZE_T bytes_read = 0;

				if (!::ReadProcessMemory(
					process.handle().native_handle(),
					reinterpret_cast<LPVOID>(static_cast<AddressT>(address)),
					&result, 
					size,
					&bytes_read
				))
					throw winapi_error("[memory::read] ReadProcessMemory failed, " + std::to_string(bytes_read) + " bytes read");

				return result;
			}
		};

		/// @brief memory::read std::string customization point.
		template <>
		struct read<std::string>
		{
			template <typename AddressT>
			static std::string invoke(const process<vm_read>& process, const address<AddressT> address, const std::size_t size)
			{
				std::string buffer(size, 0);
				SIZE_T bytes_read = 0;

				/// Todo: Read until null terminator or threshold
				if (!::ReadProcessMemory(
					process.handle().native_handle(),
					reinterpret_cast<boost::winapi::LPCVOID_>(static_cast<AddressT>(address)),
					buffer.data(),
					size,
					&bytes_read
				))
					throw winapi_error("[memory::read<std::string>] ReadProcessMemory failed, " + std::to_string(bytes_read) + " bytes read");

				return buffer.c_str();
			}
		};
	}
}
