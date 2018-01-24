#pragma once

#include <distant\types.hpp>

#include <type_traits>
#include <iostream>

namespace distant::memory
{
	class address
	{
	public: // interface
		constexpr explicit operator distant::dword() const noexcept;

	public: // operators
		friend constexpr bool operator==(address, address) noexcept;
		friend constexpr bool operator>(address, address) noexcept;

		constexpr address& operator +=(address) noexcept;
		constexpr address& operator -=(address) noexcept;
		constexpr address& operator *=(address) noexcept;
		constexpr address& operator /=(address) noexcept;

	public: // {ctor}
		constexpr address() noexcept;
		constexpr address(nullptr_t) noexcept;

		template <typename T = std::enable_if_t<std::is_arithmetic<T>::value>>
		constexpr address(T) noexcept;

	private:
		void* address_;
	};
}

namespace std
{
	template <>
	struct hash<distant::memory::address>;
}

// Implementation:
#include <distant\memory\impl\address.hxx>
