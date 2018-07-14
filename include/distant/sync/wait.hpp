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

	namespace detail
	{
		template <class KernelObject, class Time>
		state wait_single_impl(KernelObject&& object, const Time time)
		{
			using namespace std::chrono;
			using kernel_objects::native_handle_of;

			const auto handle = native_handle_of(std::forward<KernelObject>(object));

			if (handle == nullptr)
				throw std::invalid_argument("[sync::wait] Invalid handle");

#pragma warning(push)
#pragma warning(disable:4244)
			return static_cast<state>(boost::winapi::WaitForSingleObject(
				handle,
				time
			));
#pragma warning(pop)
		}

		template <class Time, class... KernelObjects>
		state wait_multiple_impl(const std::tuple<KernelObjects...>& objects, bool wait_for_all, const Time time)
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

	template <class KernelObject, class Rep, class Period>
	state wait(KernelObject&& object, const std::chrono::duration<Rep, Period> time)
	{
		using namespace std::chrono;
		return detail::wait_single_impl(std::forward<KernelObject>(object), duration_cast<milliseconds>(time).count());
	}

	template <class KernelObject>
	state wait(KernelObject&& object)
	{
		using namespace std::chrono;
		return detail::wait_single_impl(std::forward<KernelObject>(object), boost::winapi::infinite);
	}

	template <class Rep, class Period, class... KernelObjects>
	state wait(const std::tuple<KernelObjects...>& tuple, bool wait_for_all, const std::chrono::duration<Rep, Period> time)
	{
		using namespace std::chrono;
		return detail::wait_multiple_impl(tuple, wait_for_all, duration_cast<milliseconds>(time).count());
	}

	template <class... KernelObjects>
	state wait_all(const std::tuple<KernelObjects...>& tuple)
	{
		using namespace std::chrono;
		return detail::wait_multiple_impl(tuple, true, boost::winapi::infinite);
	}

	template <class Rep, class Period, class... KernelObjects>
	state wait_all(const std::tuple<KernelObjects...>& tuple, const std::chrono::duration<Rep, Period> time)
	{
		using namespace std::chrono;
		return detail::wait_multiple_impl(tuple, true, duration_cast<milliseconds>(time).count());
	}

	template <class... KernelObjects>
	state wait_any(const std::tuple<KernelObjects...>& tuple)
	{
		using namespace std::chrono;
		return detail::wait_multiple_impl(tuple, false, boost::winapi::infinite);
	}

	template <class Rep, class Period, class... KernelObjects>
	state wait_any(const std::tuple<KernelObjects...>& tuple, const std::chrono::duration<Rep, Period> time)
	{
		using namespace std::chrono;
		return detail::wait_multiple_impl(tuple, false, duration_cast<milliseconds>(time).count());
	}

} // end namespace distant

namespace distant
{
	using sync::wait;
	using sync::wait_all;
	using sync::wait_any;
}