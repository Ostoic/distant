// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/agents/interface.hpp>

#include <boost/winapi/wait.hpp>
#include <distant/utility/meta/array.hpp>
#include <distant/utility/meta/tuple.hpp>

#include <chrono>
#include <tuple>

namespace distant::synch
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

	namespace detail
	{
		template <class Agent, class Time>
		state wait_single_impl(Agent&& object, const Time time)
		{
			using namespace std::chrono;
			using agents::native_handle_of;

			const auto handle = native_handle_of(std::forward<Agent>(object));

			if (handle == nullptr)
				throw std::invalid_argument("[synch::wait] Invalid handle");

#pragma warning(push)
#pragma warning(disable:4244)
			return static_cast<state>(boost::winapi::WaitForSingleObject(
				handle,
				time
			));
#pragma warning(pop)
		}

		template <class Time, class... Agents>
		state wait_multiple_impl(const std::tuple<Agents...>& objects, bool wait_for_all, const Time time)
		{
			using namespace std::chrono;
			namespace meta = utility::meta;

			const auto result = meta::tuple_transform(objects, [](auto&& object)
			{
				return native_handle_of(std::forward<decltype(object)>(object));
			});

			auto handles = meta::to_array(result);

#pragma warning(push)
#pragma warning(disable:4244)
			return static_cast<state>(boost::winapi::WaitForMultipleObjects(
				handles.size(),
				handles.data(),
				false,
				static_cast<boost::winapi::DWORD_>(time)
			));
#pragma warning(pop)
		}
	}

	template <class Agent, class Rep, class Period>
	state wait(Agent&& object, const std::chrono::duration<Rep, Period> time)
	{
		using namespace std::chrono;
		return detail::wait_single_impl(std::forward<Agent>(object), duration_cast<milliseconds>(time).count());
	}

	template <class Agent>
	state wait(Agent&& object)
	{
		return detail::wait_single_impl(std::forward<Agent>(object), boost::winapi::infinite);
	}

	template <class Rep, class Period, class... Agents>
	state wait(const std::tuple<Agents...>& tuple, bool wait_for_all, const std::chrono::duration<Rep, Period> time)
	{
		return wait_all(tuple, time);
	}

	template <class... Agents>
	state wait_all(const std::tuple<Agents...>& tuple)
	{
		using namespace std::chrono;
		return detail::wait_multiple_impl(tuple, true, boost::winapi::infinite);
	}

	template <class Rep, class Period, class... Agents>
	state wait_all(const std::tuple<Agents...>& tuple, const std::chrono::duration<Rep, Period> time)
	{
		using namespace std::chrono;
		return detail::wait_multiple_impl(tuple, true, duration_cast<milliseconds>(time).count());
	}

	template <class... Agents>
	state wait_any(const std::tuple<Agents...>& tuple)
	{
		using namespace std::chrono;
		return detail::wait_multiple_impl(tuple, false, boost::winapi::infinite);
	}

	template <class Rep, class Period, class... Agents>
	state wait_any(const std::tuple<Agents...>& tuple, const std::chrono::duration<Rep, Period> time)
	{
		using namespace std::chrono;
		return detail::wait_multiple_impl(tuple, false, duration_cast<milliseconds>(time).count());
	}

} // end namespace distant

namespace distant
{
	using synch::wait;
	using synch::wait_all;
	using synch::wait_any;
}