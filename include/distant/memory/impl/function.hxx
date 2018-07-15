#pragma once
#include "../function.hpp"

#include <distant/memory/virtual_memory.hpp>

#include <boost/mp11/mpl.hpp>

namespace distant::memory
{
	template <class R, class... Args, class CallingConv, class AddressT, process_rights AccessRights>
	R function<R(Args...), CallingConv, AddressT, AccessRights>
		::operator()(Args&&... args)
	{
		const auto remote_arguments_ptr = virtual_malloc(ptr_.process(), sizeof(std::tuple<std::decay_t<Args>...>));

		// Should there really be a distant::function? The only possibility for operator() is a multithreaded call
		// so unless we're in-process, I don't think this conveys the correct semantics for remote function calling.
		// This is what std::thread does to launch a new thread.
		// We should instead provide the same functionality, except with memory in the remote process.
		// consider distant::function_signature as an alternative
		// wpm overload for std::tuple?
		// wpm overload for unique_ptr<>? (probably not).
		const auto arguments = std::make_unique<std::tuple<std::decay_t<Args>...>>(std::forward<Args>(args)...);

		// calling_convention::push_arguments_asm
		// calling_convention::pre_call_asm
		// calling_convention::call_asm
		// calling_convention::post_call_asm

		//_STD make_unique<tuple<decay_t<_Fn>, decay_t<_Args>...> >(
			//_STD forward<_Fn>(_Fx), _STD forward<_Args>(_Ax)...));
		// Todo:
		// Require that a thread's entry point exists already?
		// create thread, then call function, or call function which creates a thread?

		// Todo:
		// static_assembler: prolog asm
		// calling_conv: push arguments
		// calling_conv?: call address (do other calling convetions not push the return onto the stack?
		// calling_conv: cleanup stack
		// calling_conv/something: retrieve return from eax register/function return.
		// static_assembler:epilog asm
		return R{};
	}

	/**
	* calling a function int(int, const char*) from another thread:
	* interface:
	*	int result = distant::call?(function, (int)param1, (const char*)param2);
	* impl:
	*	copy function parameters into separate process (similar to how std::thread does)
	*		wpm(process, 0x..., param1)
	*		wpm(process, 0x..., param2)
	*
	*	allocate space for function return:
	*		alloc(process, sizeof(int))
	*
	*	inject call code: (depending on calling convention)
	*		push param2
	*		push param1
	*		call function_address
	*		add esp, 0x8
	*		mov [return_space], eax
	*		ret
	*
	*	wait(create_remote_thread(call_code_address));
	*/

	/*template <class R, class... Args, class CallingConv, class AddressT, process_rights AccessRights>
	void function<R(Args...), CallingConv, AddressT, AccessRights>::set_process(process<AccessRights>& process) noexcept
	{
		ptr_ = make_virtual_ptr<R(*)(Args...), AddressT, AccessRights>(process, ptr_.get());
	}*/

} // namespace distant::memory
