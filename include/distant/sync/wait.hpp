// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/kernel_objects/interface.hpp>

#include <boost/winapi/wait.hpp>
#include <distant/utility/meta/array.hpp>
#include <distant/utility/meta/tuple.hpp>

#include <chrono>
#include <tuple>

namespace distant::sync
{
	namespace detail
	{
		template <class... Ts>
		struct uniform_tuple
		{
			constexpr uniform_tuple(Ts&&... ts) noexcept
				: tuple_(std::forward<Ts>(ts)...)
			{}

		private:
			std::tuple<Ts&&...> tuple_;
		};
	}

	// Windows wait codes
	enum class state : long long int
	{
		abandoned = boost::winapi::WAIT_ABANDONED_, // Wait abandoned
		signaled = boost::winapi::WAIT_OBJECT_0_,	// Object returned from wait
		timeout = boost::winapi::WAIT_TIMEOUT_,		// Wait timed out
		failed = boost::winapi::WAIT_FAILED_,		// Bad call
													//io_completion = boost::winapi::WAIT_IO_COMPLETION_, // APC ended call
	};

	template <class KernelObject, typename Rep, typename Period>
	state wait(const KernelObject& object, const std::chrono::duration<Rep, Period>& time)
	{
		using namespace std::chrono;

#pragma warning(push)
#pragma warning(disable:4244)
		return static_cast<state>(boost::winapi::WaitForSingleObject(
			native_handle_of(object),
			duration_cast<milliseconds>(time).count())
		);
#pragma warning(pop)
	}

	template <typename Rep, typename Period, class... KernelObjects>
	state wait(const std::tuple<KernelObjects...>& objects, bool wait_for_all, const std::chrono::duration<Rep, Period>& time)
	{
		using namespace std::chrono;
		namespace meta = utility::meta;

		const auto tuple = meta::tuple_transform(objects, [](const auto& object)
		{
			return native_handle_of(object);
		});

		auto handles = meta::to_array(tuple);

#pragma warning(push)
#pragma warning(disable:4244)
		return static_cast<state>(boost::winapi::WaitForMultipleObjects(
			handles.size(),
			handles.data(),
			false,
			duration_cast<milliseconds>(time).count())
		);
#pragma warning(pop)
	}

	template <typename Rep, typename Period, class... KernelObjects>
	state wait_all(const std::tuple<KernelObjects...>& tuple, const std::chrono::duration<Rep, Period>& time)
	{
		return wait(tuple, true, time);
	}

	template <typename Rep, typename Period, class... KernelObjects>
	state wait_any(const std::tuple<KernelObjects...>& tuple, const std::chrono::duration<Rep, Period>& time)
	{
		return wait(tuple, false, time);
	}

} // end namespace distant

namespace distant
{
	using sync::wait;
	using sync::wait_all;
	using sync::wait_any;
}