// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <boost/winapi/basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

BOOST_SYMBOL_IMPORT boost::winapi::HANDLE_ WINAPI OpenThread(
	boost::winapi::DWORD_ dwDesiredAccess,
	boost::winapi::BOOL_ bInheritHandle,
	boost::winapi::DWORD_ dwThreadId
);

BOOST_SYMBOL_IMPORT boost::winapi::DWORD_ WINAPI GetThreadId(
	boost::winapi::HANDLE_ Thread
);

#endif // !defined BOOST_USE_WINDOWS_H

namespace boost  {
namespace winapi {

	constexpr auto THREAD_ALL_ACCESS_				= THREAD_ALL_ACCESS;
	constexpr auto THREAD_DIRECT_IMPERSONATION_		= THREAD_DIRECT_IMPERSONATION;
	constexpr auto THREAD_GET_CONTEXT_				= THREAD_GET_CONTEXT;
	constexpr auto THREAD_IMPERSONATE_				= THREAD_IMPERSONATE;
	constexpr auto THREAD_QUERY_INFORMATION_		= THREAD_QUERY_INFORMATION;
	constexpr auto THREAD_QUERY_LIMITED_INFORMATION_= THREAD_QUERY_LIMITED_INFORMATION;
	constexpr auto THREAD_SET_CONTEXT_				= THREAD_SET_CONTEXT;
	constexpr auto THREAD_SET_INFORMATION_			= THREAD_SET_INFORMATION;
	constexpr auto THREAD_SET_LIMITED_INFORMATION_	= THREAD_SET_LIMITED_INFORMATION;
	constexpr auto THREAD_SET_THREAD_TOKEN_			= THREAD_SET_THREAD_TOKEN;
	constexpr auto THREAD_SUSPEND_RESUME_			= THREAD_SUSPEND_RESUME;
	constexpr auto THREAD_TERMINATE_				= THREAD_TERMINATE;
	
	using ::OpenThread;
	using ::GetThreadId;

} // end namespace winapi
} // end namespace boost