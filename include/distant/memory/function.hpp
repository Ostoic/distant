// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/memory/address.hpp>
#include <distant/memory/x86_calling_conventions.hpp>

// Other declarations: ./fwd.hpp

namespace distant {
namespace memory 
{
	template <typename Signature, typename CallingConv = x86_calling_conventions::cdeclcall, typename AddressT = dword>
	class function
	{
	public:
		using args_t = typename function_traits<function>::template argument_t<0>;
		using return_type = typename function_traits<function>::return_type;

	public:
		function() noexcept = default;
		explicit function(memory::address<AddressT> address);

		template <typename... Args>
		return_type operator()(Args&&... args);

		memory::address<AddressT> address() const noexcept;

	private:
		memory::address<AddressT> address_;
	};

} // namespace memory

using memory::function;

template <typename R, typename... Args, typename CallingConv, typename AddressT>
struct function_traits<function<R(Args...), CallingConv, AddressT>>
{
	using return_type = R;
	static constexpr auto arity = sizeof...(Args);

	template <unsigned int N>
	struct argument
	{
		static_assert(N < arity, "Argument index out of range");
		using type = std::tuple_element_t<N, std::tuple<Args...>>;
	};

	template <unsigned int N>
	using argument_t = typename argument<N>::type;
};

} // namespace distant

#include "impl/function.hxx"