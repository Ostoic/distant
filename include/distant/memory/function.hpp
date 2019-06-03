// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/agents/process.hpp>
#include <distant/memory/virtual_ptr.hpp>
#include <distant/memory/calling_conventions.hpp>
#include <distant/memory/type_traits.hpp>

// Other declarations: ./fwd.hpp

namespace distant::memory
{
	// Todo: Figure out order for template parameters so that default template parameters makes sense.
	template <class Signature, class CallingConv = cdeclcall, class AddressT = dword, process_rights AccessRights = process_rights::all_access>
	class function;

	template <class R, class... Args, class CallingConv, class AddressT, process_rights AccessRights>
	class function<R(Args...), CallingConv, AddressT, AccessRights>
	{
	public:
		static constexpr auto required_process_rights = process_rights::all_access;

	public:
		function() noexcept = default;

		function(nullptr_t) 
			: fn_ptr_{nullptr} 
		{}

		// Function in this process should just act like std::function.
		template <class Fn, class = std::enable_if_t<std::is_function<Fn>::value>>
		function(Fn&& fn)
			: fn_ptr_{
				current_process(),
				std::addressof(std::forward<Fn>(fn)
			)}
		{}

		function(virtual_ptr<R(*)(Args...), AddressT, AccessRights> fn_ptr) 
			: fn_ptr_{fn_ptr} 
		{}

		const agents::process<AccessRights>& process() const noexcept { return fn_ptr_.process(); }
			  agents::process<AccessRights>& process() noexcept		  { return fn_ptr_.process(); }

		//void set_process(agents::process<AccessRights>& process) noexcept;

	private:
		virtual_ptr<R(*)(Args...), AddressT, AccessRights> fn_ptr_;
	};

} // namespace distant::memory


namespace distant
{
	using memory::function;

	template <class R, class... Args, class CallingConv, class AddressT, process_rights AccessRights>
	struct function_traits<function<R(Args...), CallingConv, AddressT, AccessRights>>
	{
		using return_type = R;
		static constexpr std::size_t arity = sizeof...(Args);

		template <std::size_t N>
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