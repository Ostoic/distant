#pragma once
#include "../function.hpp"

namespace distant::memory
{
	template <typename R, typename... Args, typename CallingConv, typename AddressT, process_rights AccessRights>
	R function<R(Args...), CallingConv, AddressT, AccessRights>::operator()(Args&&... args)
	{
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

	template <typename R, typename... Args, typename CallingConv, typename AddressT, process_rights AccessRights>
	void function<R(Args...), CallingConv, AddressT, AccessRights>::set_process(process<AccessRights>& process) noexcept
	{
		ptr_ = make_virtual_ptr<R(*)(Args...), AddressT, AccessRights>(process, ptr_.get());
	}
	
} // namespace distant::memory
