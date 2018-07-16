#pragma once

#include <string>
#include <tuple>

#include <distant/memory/address.hpp>
#include <distant/utility/meta/tuple.hpp>
#include <distant/memory/aligned_offset.hpp>

namespace distant::memory
{
	template <class Domain, class Codomain>
	struct morphism
	{
	public:
		explicit morphism(const Domain& element) : element_(element) {}

	private:
		const Domain& element_;
	};

	template <class Domain>
	using endomorphism = morphism<Domain, Domain>;

	/// @brief Provides customization points for memory operations.
	template <class StandardLayoutT>
	struct operations_traits
	{
		static_assert(
			std::is_standard_layout<StandardLayoutT>::value,
			"[memory::operations_traits<StandardLayoutT>] Type must be a standard layout type."
		);

		template <class AddressT>
		static void write(const process<vm_w_op>& proc, const address<AddressT> address, const StandardLayoutT& x)
		{
			SIZE_T bytes_written = 0;
			StandardLayoutT buffer = x;
			if (!::WriteProcessMemory(
				proc.handle().native_handle(),
				reinterpret_cast<LPVOID>(static_cast<AddressT>(address)),
				&buffer,
				sizeof(StandardLayoutT),
				&bytes_written
			))
				throw winapi_error("[memory::write] WriteProcessMemory failed, " + std::to_string(bytes_written) + " bytes written");
		}

		template <class AddressT>
		static StandardLayoutT read(const process<vm_read>& process, const address<AddressT> address, const std::size_t size)
		{
			StandardLayoutT result;
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

	}; // struct operations_traits<StandardLayoutT>

	/// @brief memory::write std::string customization point.
	template <>
	struct operations_traits<std::string>
	{
		template <class AddressT>
		static void write(const process<vm_w_op>& process, const address<AddressT> address, const std::string& string)
		{
			SIZE_T bytes_written = 0;
			if (!::WriteProcessMemory(
				process.handle().native_handle(),
				reinterpret_cast<boost::winapi::LPVOID_>(static_cast<AddressT>(address)),
				string.data(),
				string.size() + 1,
				&bytes_written
			))
				throw winapi_error("[memory::write<std::string>] WriteProcessMemory failed, " + std::to_string(bytes_written) + " bytes written");
		}

		template <class AddressT>
		static std::string read(const process<vm_read>& process, const address<AddressT> address, const std::size_t size)
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

	}; // struct operations_traits<std::string>

	template <class... Ts>
	struct operations_traits<std::tuple<Ts...>>
	{
		template <class AddressT>
		static void write(const process<vm_w_op>& process, const address<AddressT> write_start, const std::tuple<Ts...>& tuple)
		{
			using namespace utility;
			using namespace boost::mp11;

			// TMP transform std::tuple<Ts...> into typelist of morphims for the case of having non-pod template parameters

			mp_for_each<mp_iota_c<sizeof...(Ts)>>([&](const auto index)
			{
				const auto& element = std::get<index>(tuple);
				using element_t = std::tuple_element_t<index, std::tuple<Ts...>>;

				operations_traits<element_t>
					::write(process, write_start + aligned_offset<index, std::tuple<Ts...>>(), element);
			});
		}

		template <class AddressT>
		static std::tuple<Ts...> read(const process<vm_read>& process, const address<AddressT> address, const std::size_t size)
		{
			std::tuple<Ts...> tuple;
			using namespace utility;
			meta::tuple_for_each(tuple, [&](auto&& element)
			{
				//operations_traits<decltype(element)>
					//::write(process, address, std::forward<decltype(element)>(element));
			});

			return tuple;
		}
	};

} // namespace distant::memory
