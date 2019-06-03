#pragma once

#include <string>
#include <string_view>
#include <tuple>

#include <distant/memory/address.hpp>
#include <distant/utility/meta/tuple.hpp>
#include <distant/memory/aligned_offset.hpp>
#include <distant/memory/virtual_reference.hpp>

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

	namespace detail
	{
		struct aligned_tag_t{};
		struct contiguous_tag_t{};
	}

	/// @brief Provides customization points for memory operations.
	template <class StandardLayoutT>
	struct operations_traits
	{
		static_assert(
			std::is_standard_layout<StandardLayoutT>::value,
			"[memory::operations_traits<StandardLayoutT>] Type must be a standard layout type.\n"
			""
		);

		static constexpr auto size(const StandardLayoutT& tx) noexcept
		{
			return sizeof(StandardLayoutT);
		}

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

	template <class T>
	struct operations_traits<T&>
	{
		static auto size(const T& t) noexcept
		{
			return operations_traits<std::remove_const_t<T>>::size(t);
		}

		template <class AddressT>
		static void write(process<vm_w_op>& process, const address<AddressT> address, const T& x)
		{
			operations_traits<std::remove_const_t<T>>::write(process, address, x);
		}

		template <class AddressT>
		static virtual_reference<T, AddressT, vm_read> 
			read(const process<vm_read>& process, const address<AddressT> address, const std::size_t size)
		{
			return make_virtual_ref<T>(process, address);
		}

	}; // struct operations_traits<StandardLayoutT>

	/// @brief memory::write std::string customization point.
	template <class Char>
	struct operations_traits<std::basic_string<Char>>
	{
		static auto size(const std::basic_string<Char>& string) noexcept
		{
			return (string.size() + 1) * sizeof(Char);
		}

		template <class AddressT>
		static void write(process<vm_w_op>& process, const address<AddressT> address, const std::basic_string<Char>& string)
		{
			SIZE_T bytes_written = 0;
			if (!::WriteProcessMemory(
				process.handle().native_handle(),
				reinterpret_cast<boost::winapi::LPVOID_>(static_cast<AddressT>(address)),
				string.data(),
				size(string),
				&bytes_written
			))
				throw winapi_error("[memory::write<std::string>] WriteProcessMemory failed, " + std::to_string(bytes_written) + " bytes written");
		}

		template <class AddressT>
		static std::basic_string<Char> read(const process<vm_read>& process, const address<AddressT> address, const std::size_t size)
		{
			std::basic_string<Char> buffer(size, 0);
			SIZE_T bytes_read = 0;

			/// Todo: Read until null terminator or threshold
			if (!::ReadProcessMemory(
				process.handle().native_handle(),
				reinterpret_cast<boost::winapi::LPCVOID_>(static_cast<AddressT>(address)),
				buffer.data(),
				buffer.capacity() * sizeof(Char),
				&bytes_read
			))
				throw winapi_error("[memory::read<std::string>] ReadProcessMemory failed, " + std::to_string(bytes_read) + " bytes read");

			return buffer.c_str();
		}

	}; // struct operations_traits<std::string>

	  /// @brief memory::write std::string customization point.
	template <>
	struct operations_traits<std::string_view>
	{
		static constexpr auto size(std::string_view string) noexcept
		{
			return string.size() + 1;
		}

		template <class AddressT>
		static void write(process<vm_w_op>& process, const address<AddressT> address, std::string_view string)
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
			return operations_traits<std::string>::read(process, address, size);
		}

	}; // struct operations_traits<std::string>

	// TODO: Specialize this for when T is StandardLayout so that we can do a contiguous write instead
	template <class T, std::size_t N>
	struct operations_traits<std::array<T, N>>
	{
		static constexpr auto size(const std::array<T, N>& array) noexcept
		{
			return N * operations_traits<T>::size(array[0]);
		}

		template <class AddressT>
		static void write(process<vm_w_op>& process, const address<AddressT> address, const std::array<T, N>& array)
		{
			for (int i = 0; i < N; ++i)
				memory::write(process, address + i * operations_traits<T>::size(array[0]), array[i]);
		}

		template <class AddressT>
		static std::array<T, N> read(const process<vm_read>& process, const address<AddressT> address, const std::size_t size)
		{
			std::array<T, N> buffer;

			for (int i = 0; i < N; ++i)
				buffer[i] = memory::read<T>(process, address + i * operations_traits<T>::size(buffer[0]), operations_traits<T>::size(buffer[0]));

			return buffer;
		}

	};

	/// @brief memory::write std::string customization point.
	//template <>
	//struct operations_traits<const char*>
	//{
	//	template <class AddressT>
	//	static void write(process<vm_w_op>& process, const address<AddressT> address, const char* string)
	//	{
	//		SIZE_T bytes_written = 0;
	//		if (!::WriteProcessMemory(
	//			process.handle().native_handle(),
	//			reinterpret_cast<boost::winapi::LPVOID_>(static_cast<AddressT>(address)),
	//			string.data(),
	//			string.size() + 1,
	//			&bytes_written
	//		))
	//			throw winapi_error("[memory::write<std::string>] WriteProcessMemory failed, " + std::to_string(bytes_written) + " bytes written");
	//	}

	//	template <class AddressT>
	//	static std::string read(const process<vm_read>& process, const address<AddressT> address, const std::size_t size)
	//	{
	//		return operations_traits<std::string>::read(process, address, size);
	//	}

	//}; // struct operations_traits<std::string>

	template <class... Ts>
	struct operations_traits<std::tuple<Ts...>>
	{
		static constexpr auto size(const std::tuple<Ts...>& tuple) noexcept
		{
			using namespace boost::mp11;

			std::size_t size = 0;
			tuple_for_each(tuple, [&size](const auto& element)
			{
				size += operations_traits<std::decay_t<decltype(element)>>::size(element);
			});

			return size;
		}
	};

	template <class... Ts>
	struct operations_traits<std::pair<std::tuple<Ts...>, detail::aligned_tag_t>>
	{
		template <class... Us>
		static constexpr auto size(const std::tuple<Us...>& tuple) noexcept
		{
			using namespace boost::mp11;

			std::size_t size = 0;
			tuple_for_each(tuple, [&size](const auto& element)
			{
				size += operations_traits<std::decay_t<decltype(element)>>::size(element);
			});

			return size;
		}

		template <class AddressT>
		static auto write(process<vm_w_op>& process, const address<AddressT> write_start, const std::tuple<Ts...>& tuple)
		{
			using namespace utility;
			using namespace boost::mp11;

			// TMP transform std::tuple<Ts...> into typelist of morphims for the case of having non-pod template parameters
			// consider aligning based on image of write operation (could be std::string -> char[16] for example).
			std::array<address<AddressT>, sizeof...(Ts)> addresses;
			mp_for_each<mp_iota_c<sizeof...(Ts)>>([&](const auto index_c)
			{
				const auto& element = std::get<index_c>(tuple);
				using element_t = meta::remove_cvref<std::tuple_element_t<index_c, std::tuple<Ts...>>>;

				operations_traits<element_t>
					::write(process, write_start + aligned_offset<index_c, std::tuple<Ts...>>(), element);

				addresses[index_c.value] = write_start + aligned_offset<index_c, std::tuple<Ts...>>();
			});

			return addresses;
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

	}; // struct operations_traits<std::pair<std::tuple<Ts...>, detail::aligned_tag_t>>

	template <class... Ts>
	struct operations_traits<std::pair<std::tuple<Ts...>, detail::contiguous_tag_t>>
	{
		static constexpr auto size(const std::pair<std::tuple<Ts...>, detail::contiguous_tag_t>& pair) noexcept
		{
			using namespace boost::mp11;

			std::size_t size = 0;
			tuple_for_each(pair.first, [&size](const auto& element)
			{
				size += operations_traits<std::decay_t<decltype(element)>>::size(element);
			});

			return size;
		}

		template <class AddressT>
		static auto write(process<vm_w_op>& process, address<AddressT> write_start, const std::tuple<Ts...>& tuple)
		{
			using namespace utility;
			using namespace boost::mp11;

			std::array<address<AddressT>, sizeof...(Ts)> addresses;
			mp_for_each<mp_iota_c<sizeof...(Ts)>>([&](const auto index_c)
			{
				const auto& element = std::get<index_c>(tuple);
				using element_t = meta::remove_cvref<std::tuple_element_t<index_c, std::tuple<Ts...>>>;

				operations_traits<element_t>
					::write(process, write_start, element);

				addresses[index_c.value] = write_start;
				write_start += operations_traits<element_t>::size(element);
			});

			return addresses;
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
		} // operations_traits<std::pair<std::tuple<Ts...>, detail::contiguous_tag_t>>
	};

} // namespace distant::memory
