#pragma once
#include "..\map.hpp"


namespace meta
{
//interface: iterators
	template <typename K, typename V, std::size_t N = 5>
	inline constexpr typename map<K, V, N>::iterator
	map<K, V, N>::begin() const noexcept
	{
		return this->data_.begin();
	}

	template <typename K, typename V, std::size_t N = 5>
	inline constexpr typename map<K, V, N>::iterator
	map<K, V, N>::end() const noexcept
	{
		return this->data_.end();
	}

//interface: capcity
	template <typename K, typename V, std::size_t N = 5>
	inline constexpr std::size_t map<K, V, N>::size() const noexcept
	{
		return this->data_.size();
	}

	template <typename K, typename V, std::size_t N = 5>
	inline constexpr bool map<K, V, N>::empty() const noexcept
	{
		return N == 0;
	}

//interface: lookup
	template <typename K, typename V, std::size_t N = 5>
	inline constexpr std::size_t map<K, V, N>::count(const key_type& key) const noexcept
	{
		std::size_t count = 0;
		for (std::size_t i = 0; i < N; i++)
		{
			if (std::get<0>(this->data_[i]) == key)
				count++;
		}

		return count;
	}

	template <typename K, typename V, std::size_t N = 5>
	inline constexpr typename map<K, V, N>::iterator
	map<K, V, N>::find(const key_type& key) const
	{
		for (std::size_t i = 0; i < N; i++)
		{
			if (std::get<0>(this->data_[i]) == key)
				return iterator{ this->data_.data() + i };
		}

		return this->end();
	}

	template <typename K, typename V, std::size_t N = 5>
	inline constexpr const auto& map<K, V, N>::operator[](const key_type& key) const
	{
		return (*this->find(key)).second;
	}

	//template <typename K, typename V, std::size_t N = 5>
	//template <std::size_t M>
	//map<K, V, M> map<K, V, N>::operator=(const map<K, V, M>& other) const noexcept
	//{
	//	return 
	//}

//{ctor}:
	template <typename K, typename V, std::size_t N>
	template <typename... Ts>
	inline constexpr map<K, V, N>::map(Ts&&... ts)
		: data_(std::array<std::common_type_t<Ts...>, sizeof...(Ts)>{std::forward<Ts>(ts)...}) {}

	template <typename K, typename V, std::size_t N = 5>
	inline constexpr map<K, V, N>::map(const std::array<std::pair<K, V>, N>& array) noexcept
		: data_(array) {}

//free:
	template <typename... Ts>
	inline constexpr auto make_map(Ts&&... ts) noexcept
	{
		using Pair_t = std::common_type_t<Ts...>;
		using Key_t = typename Pair_t::first_type;
		using Value_t = typename Pair_t::second_type;
		return map<Key_t, Value_t, sizeof...(Ts)>{std::forward<Ts>(ts)...};
	}
} // namespace meta