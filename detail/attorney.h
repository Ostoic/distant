#pragma once

#include <distant\windows\handle.h>

namespace distant {
namespace detail  {
	
	// Attorney class that allows the Client to view an implementation detail of the given class.
	template <class Accessor, class F>
	class Attorney
	{
	public:
		constexpr Attorney() = default;

	private:
		template <typename result_type, typename Fn>
		result_type caller(Fn f)
		{ 
			return h.get_sub_handle(); 
		}

		friend class Accessor;
	
	};
	
} // end namespace detail
} // end namespace distant