#pragma once

#include <future>

#include <distant/memory/function.hpp>

namespace distant::synch
{
	class future
	{

	};

	template <class Return, class... Args, class CallingConvention, class AddressT, process_rights Access>
	std::future<Return>
		async(const memory::function<Return(Args...)>& function, Args&&... args)
	{
		//function.

		std::promise<Return> promise;

		return (std::future<Return>(promise._Get_state_for_future(), std::_Nil{}));
	}

} // namespace distant::synch
