// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../map.hpp"

namespace distant::utility::meta
{
//interface: iterators
	template <typename K, typename V, std::size_t N = 5>
	constexpr typename map<K, V, N>::iterator
	map<K, V, N>::begin() const noexcept
	{
		return this->data_.begin();
	}

	template <typename K, typename V, std::size_t N = 5>
	constexpr typename map<K, V, N>::iterator
	map<K, V, N>::end() const noexcept
	{
		return this->data_.end();
	}

//interface: capcity
	template <typename K, typename V, std::size_t N = 5>
	constexpr std::size_t map<K, V, N>::size() const noexcept
	{
		return this->data_.size();
	}

	template <typename K, typename V, std::size_t N = 5>
	constexpr bool map<K, V, N>::empty() const noexcept
	{ return N == 0; }

//interface: lookup
	template <typename K, typename V, std::size_t N = 5>
	constexpr std::size_t map<K, V, N>::count(const key_type& key) const
	{
		std::size_t count = 0;
		std::size_t i = -1;
		while ((i = this->find_first(key, i + 1)) < N)
			++count;

		//this->hash_(key);

		return count;
	}

	template <typename K, typename V, std::size_t N = 5>
	constexpr typename map<K, V, N>::iterator
	map<K, V, N>::find(const key_type& key) const
	{
		return iterator{this->data_.data() + this->find_first(key)};
	}

	template <typename K, typename V, std::size_t N = 5>
	constexpr const typename map<K, V, N>::value_type& 
	map<K, V, N>::operator[](const key_type& key) const
	{
		const auto i = this->find_first(key);

		// Index of the key must be within the map
		if (!(i < N))
			throw std::out_of_range("[map::operator[]] Key was not found");

		return this->data_[i].second;
	}

	template <typename K, typename V, std::size_t N>
	template <std::size_t I>
	constexpr std::pair<K, V> map<K, V, N>::get() const noexcept
	{
		return std::get<I>(this->data_);
	}

//{ctor}:
	template <typename K, typename V, std::size_t N>
	template <typename... Ts>
	constexpr map<K, V, N>::map(Ts&&... ts)
		: data_(std::array<std::common_type_t<Ts...>, sizeof...(Ts)>{std::forward<Ts>(ts)...})
		/*, hash_()*/ {}

	template <typename K, typename V, std::size_t N = 5>
	constexpr map<K, V, N>::map(const std::array<std::pair<K, V>, N>& array) noexcept
		: data_(array) 
		/*, hash_()*/ {}

//private:
	template <typename K, typename V, std::size_t N>
	constexpr std::size_t map<K, V, N>::find_first(const key_type& key, std::size_t start) const
	{
		for (; start < N; ++start)
			if (this->data_[start].first == key)
				return start;

		return N;
	}

//free:
	template <typename... Ts>
	constexpr auto make_map(Ts&&... ts) noexcept
	{
		using pair_t = std::common_type_t<Ts...>;
		using key_t = typename pair_t::first_type;
		using value_t = typename pair_t::second_type;
		return map<key_t, value_t, /*meta::hash<Key_t>,*/ sizeof...(Ts)>{std::forward<Ts>(ts)...};
	}
} // namespace distant::utility::meta