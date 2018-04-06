// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <array>
#include <tuple>

#include "hash.hpp"

namespace distant::utility::meta
{
	// Compile-time "map" with O(n) lookup.
	template <typename Key, typename Value, std::size_t Capacity = 5>
	class map
	{
	public: // subtypes
		using key_type = Key;
		using value_type = Value;

		using iterator = typename std::array<std::pair<Key, Value>, Capacity>::iterator;
		using const_iterator = iterator;

	public: // interface

	// Iterators
		constexpr iterator begin() const noexcept;

		constexpr iterator end() const noexcept;

	// Capacity
		constexpr std::size_t size() const noexcept;

		constexpr bool empty() const noexcept;

	// Lookup
		constexpr const value_type& operator[](const key_type& key) const;

		constexpr std::size_t count(const key_type& key) const;

		constexpr iterator find(const key_type& key) const;

	public: // {ctor}
		constexpr map() = default;

		constexpr map(const std::array<std::pair<Key, Value>, Capacity>&) noexcept;

		template <typename... Ts>
		constexpr map(Ts&&... ts);

	private: // impl
		constexpr std::size_t find_first(const key_type& key, std::size_t start = 0) const;

	private: // data
		//Hash hash_;
		std::array<std::pair<key_type, value_type>, Capacity> data_;
	};

	template <typename... Ts>
	constexpr auto make_map(Ts&&... ts) noexcept;

} // namespace distant::utility::meta

// Implementation:
#include "impl/map.hxx"