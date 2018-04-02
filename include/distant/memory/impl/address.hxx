#pragma once
#include "../address.hpp"

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
	constexpr address<A>::address(nullptr_t) noexcept  
		: address() 
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

//free:
	/*template <typename A>
	constexpr bool operator==(const address<A> lhs, const address<A> rhs) noexcept
	{ return static_cast<A>(lhs) == static_cast<A>(rhs); }

	template <typename A>
	constexpr bool operator<(const address<A> lhs, const address<A> rhs) noexcept
	{ return static_cast<A>(lhs) < static_cast<A>(rhs); }

	template <typename A>
	constexpr bool operator!=(const address<A> lhs, const address<A> rhs) noexcept
	{ return !operator==(lhs, rhs); }

	template <typename A>
	constexpr bool operator>(const address<A> lhs, const address<A> rhs) noexcept
	{ return !operator<(lhs, rhs) && !operator==(lhs, rhs); }

	template <typename A>
	constexpr bool operator>=(const address<A> lhs, const address<A> rhs) noexcept
	{ return !operator>(lhs, rhs); }

	template <typename A>
	constexpr bool operator<=(const address<A> lhs, const address<A> rhs) noexcept
	{ return !operator>(lhs, rhs); }

	template <typename A>
	constexpr address<A> operator&(const address<A> lhs, const address<A> rhs) noexcept
	{ return static_cast<A>(lhs) & static_cast<A>(rhs); }

	template <typename A>
	constexpr address<A> operator|(const address<A> lhs, const address<A> rhs) noexcept
	{ return static_cast<A>(lhs) | static_cast<A>(rhs); }

	template <typename A>
	constexpr address<A> operator^(const address<A> lhs, const address<A> rhs) noexcept
	{ return static_cast<A>(lhs) ^ static_cast<A>(rhs); }

	template <typename A>
	constexpr address<A> operator<<(const address<A> lhs, const address<A> rhs) noexcept
	{ return static_cast<A>(lhs) << static_cast<A>(rhs); }

	template <typename A>
	constexpr address<A> operator>>(const address<A> lhs, const address<A> rhs) noexcept
	{ return static_cast<A>(lhs) >> static_cast<A>(rhs); }

	template <typename A>
	constexpr address<A> operator+(const address<A> lhs, const address<A> rhs) noexcept
	{ return static_cast<A>(lhs) + static_cast<A>(rhs); }

	template <typename A>
	constexpr address<A> operator-(const address<A> lhs, const address<A> rhs) noexcept
	{ return static_cast<A>(lhs) - static_cast<A>(rhs); }

	template <typename A>
	constexpr address<A> operator*(const address<A> lhs, const address<A> rhs) noexcept
	{ return static_cast<A>(lhs) * static_cast<A>(rhs); }

	template <typename A>
	constexpr address<A> operator/(const address<A> lhs, const address<A> rhs) noexcept
	{ return static_cast<A>(lhs) / static_cast<A>(rhs); }*/

	template <std::size_t N, typename A>
	constexpr distant::byte get(const address<A> addr) noexcept
	{
		static_assert(N < sizeof(address<A>), "[distant::get<address>] Byte index out of range");

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
