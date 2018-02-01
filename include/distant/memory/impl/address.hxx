#include <distant\memory\address.hpp>

namespace distant::memory
{
//interface:
	template <typename A>
	inline constexpr address<A>::operator address_type() const noexcept
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
	inline constexpr address<A>& address<A>::operator +=(address other) noexcept
	{
		*this = (static_cast<address_type>(*this) + static_cast<address_type>(other));
		return *this;
	}

	template <typename A>
	inline constexpr address<A>& address<A>::operator -=(address<A> other) noexcept
	{
		*this = (static_cast<address_type>(*this) - static_cast<address_type>(other));
		return *this;
	}

	template <typename A>
	inline constexpr address<A>& address<A>::operator *=(address<A> other) noexcept
	{
		*this = (static_cast<address_type>(*this) * static_cast<address_type>(other));
		return *this;
	}

	template <typename A>
	inline constexpr address<A>& address<A>::operator /=(address<A> other) noexcept
	{
		*this = (static_cast<address_type>(*this) / static_cast<address_type>(other));
		return *this;
	}

//{ctor}
	template <typename A>
	inline constexpr address<A>::address() noexcept 
		: address_(static_cast<underlying_type>(nullptr)) {}

	template <typename A>
	inline constexpr address<A>::address(nullptr_t) noexcept  
		: address() {}

#pragma warning(push)
#pragma warning(disable:4312)
	template <typename A>
	template <typename T>
	inline constexpr address<A>::address(T x) noexcept 
		: address_(static_cast<underlying_type>(x)) {}
#pragma warning(pop)

//free:
	template <typename A>
	inline constexpr bool operator==(address<A> lhs, address<A> rhs) noexcept
	{ return lhs.address_ == rhs.address_; }

	template <typename A>
	inline constexpr bool operator>(address<A> lhs, address<A> rhs) noexcept
	{ return lhs.address_ > rhs.address_; }

	template <typename A>
	inline constexpr bool operator!=(address<A> lhs, address<A> rhs) noexcept
	{ return !operator==(lhs, rhs); }

	template <typename A>
	inline constexpr bool operator<(address<A> lhs, address<A> rhs) noexcept
	{ return !operator>(lhs, rhs) && !operator==(lhs, rhs); }

	template <typename A>
	inline constexpr bool operator>=(address<A> lhs, address<A> rhs) noexcept
	{ return !operator>(lhs, rhs); }

	template <typename A>
	inline constexpr bool operator<=(address<A> lhs, address<A> rhs) noexcept
	{ return !operator>(lhs, rhs); }

	template <typename A>
	inline constexpr address<A> operator&(address<A> lhs, address<A> rhs) noexcept
	{ return static_cast<A>(lhs) & static_cast<A>(rhs); }

	template <typename A>
	inline constexpr address<A> operator|(address<A> lhs, address<A> rhs) noexcept
	{ return static_cast<A>(lhs) | static_cast<A>(rhs); }

	template <typename A>
	inline constexpr address<A> operator^(address<A> lhs, address<A> rhs) noexcept
	{ return static_cast<A>(lhs) ^ static_cast<A>(rhs); }

	template <typename A>
	inline constexpr address<A> operator<<(address<A> lhs, address<A> rhs) noexcept
	{ return static_cast<A>(lhs) << static_cast<A>(rhs); }

	template <typename A>
	inline constexpr address<A> operator>>(address<A> lhs, address<A> rhs) noexcept
	{ return static_cast<A>(lhs) >> static_cast<A>(rhs); }

	template <typename A>
	inline constexpr address<A> operator+(address<A> lhs, address<A> rhs) noexcept
	{ return static_cast<A>(lhs) + static_cast<A>(rhs); }

	template <typename A>
	inline constexpr address<A> operator-(address<A> lhs, address<A> rhs) noexcept
	{ return static_cast<A>(lhs) - static_cast<A>(rhs); }

	template <typename A>
	inline constexpr address<A> operator*(address<A> lhs, address<A> rhs) noexcept
	{ return static_cast<address>(lhs) * static_cast<A>(rhs); }

	template <typename A>
	inline constexpr address<A> operator/(address<A> lhs, address<A> rhs) noexcept
	{ return static_cast<A>(lhs) / static_cast<A>(rhs); }

	template <std::size_t N, typename A>
	inline constexpr distant::byte get(address<A> addr) noexcept
	{
		if constexpr (N >= sizeof(address<A>))
			static_assert(false, "[distant::get] Byte index out of range");

		const auto bytes = static_cast<A>(addr);
		return (bytes >> (8 * N)) & 0xff;
	}
}

namespace std
{
	template <typename A>
	struct hash<distant::memory::address<A>>
	{
		using argument_type = distant::memory::address;
		using result_type = std::size_t;

		result_type operator()(const argument_type& address) const noexcept
		{
			return std::hash<A>{}(static_cast<A>(address));
		}
	};
}
