#include <distant\memory\address.hpp>

namespace distant::memory
{
//interface:
	inline constexpr address::operator distant::dword() const noexcept
	{
		return reinterpret_cast<distant::dword>(this->address_);
	}

//operators:
	inline constexpr address& address::operator +=(address other) noexcept
	{
		*this = (static_cast<distant::dword>(*this) + static_cast<distant::dword>(other));
		return *this;
	}

	inline constexpr address& address::operator -=(address other) noexcept
	{
		*this = (static_cast<distant::dword>(*this) - static_cast<distant::dword>(other));
		return *this;
	}

	inline constexpr address& address::operator *=(address other) noexcept
	{
		*this = (static_cast<distant::dword>(*this) * static_cast<distant::dword>(other));
		return *this;
	}

	inline constexpr address& address::operator /=(address other) noexcept
	{
		*this = (static_cast<distant::dword>(*this) / static_cast<distant::dword>(other));
		return *this;
	}

//{ctor}
	inline constexpr address::address() noexcept : address_(nullptr) {}
	inline constexpr address::address(nullptr_t) noexcept  : address() {}

	template <typename T>
	inline constexpr address::address(T x) noexcept : address_(reinterpret_cast<void*>(x)) {}

//free:
	inline constexpr bool operator==(address lhs, address rhs) noexcept
	{ return lhs.address_ == rhs.address_; }

	inline constexpr bool operator>(address lhs, address rhs) noexcept
	{ return lhs.address_ > rhs.address_; }

	inline constexpr bool operator!=(address lhs, address rhs) noexcept
	{ return !operator==(lhs, rhs); }

	inline constexpr bool operator<(address lhs, address rhs) noexcept
	{ return !operator>(lhs, rhs) && !operator==(lhs, rhs); }

	inline constexpr bool operator>=(address lhs, address rhs) noexcept
	{ return !operator>(lhs, rhs); }

	inline constexpr bool operator<=(address lhs, address rhs) noexcept
	{ return !operator>(lhs, rhs); }

	inline constexpr address operator&(address lhs, address rhs) noexcept
	{ return static_cast<distant::dword>(lhs) & static_cast<distant::dword>(rhs); }

	inline constexpr address operator|(address lhs, address rhs) noexcept
	{ return static_cast<distant::dword>(lhs) | static_cast<distant::dword>(rhs); }

	inline constexpr address operator^(address lhs, address rhs) noexcept
	{ return static_cast<distant::dword>(lhs) ^ static_cast<distant::dword>(rhs); }

	inline constexpr address operator<<(address lhs, address rhs) noexcept
	{ return static_cast<distant::dword>(lhs) << static_cast<distant::dword>(rhs); }

	inline constexpr address operator>>(address lhs, address rhs) noexcept
	{ return static_cast<distant::dword>(lhs) >> static_cast<distant::dword>(rhs); }

	inline constexpr address operator+(address lhs, address rhs) noexcept
	{ return static_cast<distant::dword>(lhs) + static_cast<distant::dword>(rhs); }

	inline constexpr address operator-(address lhs, address rhs) noexcept
	{ return static_cast<distant::dword>(lhs) - static_cast<distant::dword>(rhs); }

	inline constexpr address operator*(address lhs, address rhs) noexcept
	{ return static_cast<distant::dword>(lhs) * static_cast<distant::dword>(rhs); }

	inline constexpr address operator/(address lhs, address rhs) noexcept
	{ return static_cast<distant::dword>(lhs) / static_cast<distant::dword>(rhs); }


}

namespace std
{
	template <>
	struct hash<distant::memory::address>
	{
		using argument_type = distant::memory::address;
		using result_type = std::size_t;

		result_type operator()(const argument_type& address) const noexcept
		{
			return std::hash<distant::dword>{}(static_cast<distant::dword>(address));
		}
	};
}
