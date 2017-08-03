#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <utility>

namespace distant {
namespace utility {
	
	class Literal 
	{
	public:
		constexpr Literal() = default;
		
	private:
		void* operator new (std::size_t) = delete;
		void  operator delete(void*) = delete;
		
		void  operator &() = delete;
		void  operator =(const Literal&) = delete;
	};
	
} // end namespace detail
} // end namespace distant