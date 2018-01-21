#pragma once

#include <distant\kernel\process_base.hpp>
#include <distant\kernel\access_token.hpp>

namespace distant::kernel
{
	/*namespace detail
	{
		class current_process
		{
		private:
			static process_base get() noexcept
			{
				const auto currentHandle = boost::winapi::GetCurrentProcess();
				return process_base{
					handle<process_base>{currentHandle, access_rights::handle::close_protected}
				};
			}

		public:
			process_base operator()() noexcept
			{ return current_process::get(); }

			static access_token<access_rights::token::all_access, process_base> access_token()
			{
				return get_access_token<access_rights::token::all_access>(current_process::get());
			}
		};
	}

	detail::current_process current_process;*/
}
