// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/config.hpp>
#include <distant/handle.hpp>
#include <distant/kernel_objects/kernel_object.hpp>

namespace distant::kernel_objects
{
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379557(v=vs.85).aspx
	class securable : public kernel_object
	{
	public:
		/*********************************/
		/** Windows Object constructors **/
		/*********************************/
		// Invalid handle default constructor
		securable() noexcept = default;

		explicit securable(distant::handle<securable>&& h);

		securable(securable&& tmp) noexcept = default;

		securable& operator =(securable&& other) noexcept = default;
	};
} // end namespace distant::kernel_objects
