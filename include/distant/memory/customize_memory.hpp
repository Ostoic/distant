#pragma once

namespace distant::memory
{
	/// @brief Provides customization points for memory operations.
	namespace customize
	{
		/// @brief memory::write standard layout customization point.
		template <typename T>
		struct write
		{
			static_assert(std::is_standard_layout<T>::value);

			template <typename AddressT>
			static void invoke(const process<vm_w_op>& proc, const address<AddressT> address, T x)
			{
				SIZE_T bytes_read = 0;
				T buffer = std::move(x);
				if (!::WriteProcessMemory(proc.handle().native_handle(), reinterpret_cast<LPVOID>(static_cast<AddressT>(address)), &buffer, sizeof(T), &bytes_read))
					throw windows_error("[memory::write] WriteProcessMemory failed, " + std::to_string(bytes_read) + " bytes written");
			}
		};

		/// @brief memory::read standard layout customization point.
		template <typename T>
		struct read
		{
			static_assert(std::is_standard_layout<T>::value);

			template <typename AddressT>
			T invoke(const process<vm_read>& process, const address<AddressT> address)
			{
				T result;
				SIZE_T bytes_read = 0;

				if (!::ReadProcessMemory(process.handle().native_handle(),
					reinterpret_cast<LPVOID>(static_cast<AddressT>(address)),
					&result, sizeof(T), &bytes_read
				))
					throw windows_error("[memory::read] ReadProcessMemory failed, " + std::to_string(bytes_read) + " bytes read");

				return result;
			}
		};
	}
}
