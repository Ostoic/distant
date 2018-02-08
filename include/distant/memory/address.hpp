#pragma once

#include <distant\types.hpp>

#include <type_traits>
#include <iostream>

namespace distant{
namespace memory {

	template <typename Address_t>
	class address
	{
	private:
		using underlying_type = std::uintptr_t;

	public:
		using address_type = Address_t;

	public: // interface
		constexpr explicit operator address_type() const noexcept;

	public: // operators
		friend constexpr bool operator==(address, address) noexcept;
		friend constexpr bool operator!=(address, address) noexcept;
		friend constexpr bool operator>(address, address) noexcept;
		friend constexpr bool operator>=(address, address) noexcept;
		friend constexpr bool operator<(address, address) noexcept;
		friend constexpr bool operator<=(address, address) noexcept;

		friend constexpr bool operator&(address, address) noexcept;
		friend constexpr bool operator|(address, address) noexcept;
		friend constexpr bool operator^(address, address) noexcept;
		friend constexpr bool operator<<(address, address) noexcept;
		friend constexpr bool operator>>(address, address) noexcept;
		friend constexpr bool operator+(address, address) noexcept;
		friend constexpr bool operator-(address, address) noexcept;
		friend constexpr bool operator*(address, address) noexcept;
		friend constexpr bool operator/(address, address) noexcept;

		constexpr address& operator +=(address) noexcept;
		constexpr address& operator -=(address) noexcept;
		constexpr address& operator *=(address) noexcept;
		constexpr address& operator /=(address) noexcept;

	public: // {ctor}
		constexpr address() noexcept;
		constexpr address(nullptr_t) noexcept;

		template <typename T>
		/*explicit*/ constexpr address(T x) noexcept;

	private:
		underlying_type address_;
	};

	template <std::size_t N, typename A>
	constexpr distant::byte get(address<A>) noexcept;

} // namespace memory

using address = memory::address<distant::dword>;

} // namespace distant

namespace std
{
	template <typename A>
	struct hash<distant::memory::address<A>>;
}

// Implementation:
#include <distant\memory\impl\address.hxx>
