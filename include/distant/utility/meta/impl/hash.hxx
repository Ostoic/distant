#pragma once
#include "../hash.hpp"

namespace distant::utility::meta
{
	namespace detail
	{
		/**********************************************************************************************//**
		 * @brief Hash an array of bytes via FNV hash.
		 *
		 * @tparam S The number of bytes in the array.
		 * @param bytes An array consisting of the bytes to hash.
		 *
		 * @return A std::size_t containing the resulting hash.
		 **************************************************************************************************/
		template <std::size_t S>
		constexpr std::size_t hash_bytes(const std::array<unsigned char, S>& bytes) noexcept
		{
			// This is just an implementation of the Fowler–Noll–Vo (FNV) hash,
			// using Visual Studio's primes and basis taken from _Hash_Bytes.
#if defined(_WIN64)
			static_assert(sizeof(std::size_t) == 8, "This code is for 64-bit std::size_t.");
			const std::size_t offset_basis = 14695981039346656037ULL;
			const std::size_t prime = 1099511628211ULL;
#else
			static_assert(sizeof(std::size_t) == 4, "This code is for 32-bit std::size_t.");
			const std::size_t offset_basis = 2166136261U;
			const std::size_t prime = 16777619U;
#endif

			std::size_t result = offset_basis;
			for (std::size_t i = 0; i < S; ++i)
			{	
				result ^= static_cast<std::size_t>(bytes[i]);
				result *= prime;
			}

			return result;
		}
	}

	template <typename T>
	constexpr std::size_t hash<T>::operator()(const T& key) const noexcept
	{
		return detail::hash_bytes(meta::byte_array_from(key));
	}
} // namespace distant::utility::meta
