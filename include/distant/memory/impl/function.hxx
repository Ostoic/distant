#pragma once
#include "../function.hpp"

namespace distant::memory
{
	template <typename R, typename... As, typename CC, typename A>
	R function<R(As...), CC, A>::operator()(As... args)
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

	template <typename R, typename... As, typename CC, typename A>
	void function<R(As...), CC, A>::set_process(const process<required_process_rights>& process) noexcept
	{
		ptr_ = make_virtual_ptr<R(*)(As...), A>(process, ptr_.get());
	}
	
} // namespace distant::memory
