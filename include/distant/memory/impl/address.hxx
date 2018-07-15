// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../address.hpp"

#include <iomanip>

namespace distant::memory
{
//interface:
	template <typename A>
	constexpr address<A>::operator address_type() const noexcept
	{
#pragma warning(push)
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4302)
		return static_cast<address_type>(this->address_);
#pragma warning(pop)
#pragma warning(pop)
	}

//operators:
	template <typename A>
	constexpr address<A>& address<A>::operator +=(const address other) noexcept
	{
		*this = (static_cast<address_type>(*this) + static_cast<address_type>(other));
		return *this;
	}

	template <typename A>
	constexpr address<A>& address<A>::operator -=(const address<A> other) noexcept
	{
		*this = (static_cast<address_type>(*this) - static_cast<address_type>(other));
		return *this;
	}

	template <typename A>
	constexpr address<A>& address<A>::operator *=(const address<A> other) noexcept
	{
		*this = (static_cast<address_type>(*this) * static_cast<address_type>(other));
		return *this;
	}

	template <typename A>
	constexpr address<A>& address<A>::operator /=(const address<A> other) noexcept
	{
		*this = (static_cast<address_type>(*this) / static_cast<address_type>(other));
		return *this;
	}

	template <typename A>
	constexpr address<A>& address<A>::operator &=(const address<A> other) noexcept
	{
		*this = (static_cast<address_type>(*this) & static_cast<address_type>(other));
		return *this;
	}

	template <typename A>
	constexpr address<A>& address<A>::operator ^=(const address<A> other) noexcept
	{
		*this = (static_cast<address_type>(*this) ^ static_cast<address_type>(other));
		return *this;
	}

	template <typename A>
	constexpr address<A>& address<A>::operator |=(const address<A> other) noexcept
	{
		*this = (static_cast<address_type>(*this) | static_cast<address_type>(other));
		return *this;
	}

	template <typename A>
	constexpr address<A>& address<A>
		::operator ++() noexcept
	{
		*this = ++static_cast<address_type>(*this);
		return *this;
	}

	template <typename A>
	constexpr address<A>& address<A>
		::operator --() noexcept
	{
		*this = --static_cast<address_type>(*this);
		return *this;
	}

	/*template <typename A>
	constexpr address<A>& address<A>::operator=(underlying_type x) noexcept
	{
		this->address_ = static_cast<underlying_type>(x);
		return *this;
	}*/

//{ctor}
	template <typename A>
	constexpr address<A>::address() noexcept
		: address_(static_cast<address_type>(0))
	{}

	template <typename A>
	template <typename T>
	constexpr address<A>::address(T* x) noexcept
		: address(reinterpret_cast<address_type>(x))
	{}


#pragma warning(push)
#pragma warning(disable:4312)
	template <typename A>
	constexpr address<A>::address(address_type x) noexcept
		: address_(x)
	{}
#pragma warning(pop)

	template <typename CharT, typename TraitsT, typename AddressT>
	std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& stream, const distant::memory::address<AddressT> address)
	{
		using char_t = typename TraitsT::char_type;
		const auto old_flags = stream.flags();

		stream
			<< std::hex << "0x"
			<< std::setfill(char_t{ '0' }) << std::setw(2 * sizeof(distant::address))
			<< static_cast<distant::address::address_type>(address);

		stream.flags(old_flags);
		return stream;
	}

	template <std::size_t N, typename A>
	constexpr distant::byte get_byte(const address<A> addr) noexcept
	{
		static_assert(N < sizeof(address<A>), "[distant::get_byte<address>] Byte index out of range");

		const auto bytes = static_cast<A>(addr);
		return (bytes >> (8 * N)) & 0xff;
	}
} // namespace distant::memory

namespace std
{
	template <typename A>
	struct hash<distant::memory::address<A>>
	{
		using argument_type = distant::memory::address<A>;
		using result_type = std::size_t;

		result_type operator()(const argument_type& address) const noexcept
		{
			return std::hash<A>{}(static_cast<A>(address));
		}
	};
}
