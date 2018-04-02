#pragma once

#include <distant/handle.hpp>
#include <distant/kernel/object.hpp>

namespace distant::kernel
{
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379557(v=vs.85).aspx
	class securable : public object
	{
	public:
		/*********************************/
		/** Windows Object constructors **/
		/*********************************/
		// Invalid handle default constructor
		securable() noexcept = default;

		explicit securable(handle<securable>&& h);

		securable(securable&& tmp) noexcept = default;

		securable& operator =(securable&& other) noexcept = default;
	};
} // end namespace distant::kernel
