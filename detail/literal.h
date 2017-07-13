#pragma once

#include <utility>

namespace distant {
namespace detail  {
	
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