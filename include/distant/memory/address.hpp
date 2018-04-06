// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/types.hpp>

#include <type_traits>
#include <iostream>

namespace distant
{
	namespace memory
	{
		template <typename AddressT>
		class address
		{
		public:
			using address_type = AddressT;

		public: // interface
			constexpr explicit operator address_type() const noexcept;

		public: // operators
			constexpr address& operator +=(address) noexcept;
			constexpr address& operator -=(address) noexcept;
			constexpr address& operator *=(address) noexcept;
			constexpr address& operator /=(address) noexcept;
			constexpr address& operator &=(address) noexcept;
			constexpr address& operator ^=(address) noexcept;
			constexpr address& operator |=(address) noexcept;

			friend constexpr bool operator==(const address lhs, const address rhs) noexcept { return static_cast<AddressT>(lhs) == static_cast<AddressT>(rhs); }
			friend constexpr bool operator< (const address lhs, const address rhs) noexcept { return static_cast<AddressT>(lhs) <  static_cast<AddressT>(rhs); }
			friend constexpr bool operator!=(const address lhs, const address rhs) noexcept { return !operator==(lhs, rhs); }
			friend constexpr bool operator>=(const address lhs, const address rhs) noexcept { return !operator<(lhs, rhs); }
			friend constexpr bool operator> (const address lhs, const address rhs) noexcept { return !operator<(lhs, rhs) && !operator==(lhs, rhs); }
			friend constexpr bool operator<=(const address lhs, const address rhs) noexcept { return !operator>(lhs, rhs); }

			friend constexpr address operator& (const address lhs, const address rhs) noexcept { return static_cast<AddressT>(lhs) &  static_cast<AddressT>(rhs); }
			friend constexpr address operator| (const address lhs, const address rhs) noexcept { return static_cast<AddressT>(lhs) |  static_cast<AddressT>(rhs); }
			friend constexpr address operator^ (const address lhs, const address rhs) noexcept { return static_cast<AddressT>(lhs) ^  static_cast<AddressT>(rhs); }
			friend constexpr address operator<<(const address lhs, const address rhs) noexcept { return static_cast<AddressT>(lhs) << static_cast<AddressT>(rhs); }
			friend constexpr address operator>>(const address lhs, const address rhs) noexcept { return static_cast<AddressT>(lhs) >> static_cast<AddressT>(rhs); }
			friend constexpr address operator+ (const address lhs, const address rhs) noexcept { return static_cast<AddressT>(lhs) +  static_cast<AddressT>(rhs); }
			friend constexpr address operator- (const address lhs, const address rhs) noexcept { return static_cast<AddressT>(lhs) -  static_cast<AddressT>(rhs); }
			friend constexpr address operator* (const address lhs, const address rhs) noexcept { return static_cast<AddressT>(lhs) *  static_cast<AddressT>(rhs); }
			friend constexpr address operator/ (const address lhs, const address rhs) noexcept { return static_cast<AddressT>(lhs) /  static_cast<AddressT>(rhs); }

		public: // {ctor}
			constexpr address() noexcept;

			template <typename T>
			constexpr address(T* x) noexcept;
			constexpr address(address_type x) noexcept;

		private:
			address_type address_;
		};

		template <std::size_t N, typename A>
		constexpr byte get(address<A>) noexcept;
	} // namespace memory

	using address = memory::address<dword>;
} // namespace distant

namespace std
{
	template <typename A>
	struct hash<distant::memory::address<A>>;
}

// Implementation:
#include "impl/address.hxx"
