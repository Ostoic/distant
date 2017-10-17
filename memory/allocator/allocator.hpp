#pragma once

#include <limits>
#include <cstddef>

#include <distant\memory\allocator\allocator_policy.h>

namespace distant {
namespace memory  {

template <typename T>
class allocator_base {
	using value_type = T;
};

template <typename T>
class allocator_base <const T> {
	using value_type = T;
};

//template<typename T/*,
//		 typename Policy = distant::memory::allocation_policy<T>,
//		 typename Traits = distant::memory::*/>
//class allocator : allocator_base<T>
//{
//public : 
//    //    aliases
//	using value_type = 
//
//public : 
//    //    convert an allocator<T> to allocator<U>
//    template<typename U>
//    struct rebind {
//		using other = allocator<U>;
//    };
//
//public : 
//	inline explicit allocator() {}
//	inline ~allocator() {}
//
//	inline explicit allocator(allocator const&) {}
//
//    template<typename U>
//	inline explicit allocator(allocator<U> const&) {}
//
//    //    address
//    inline pointer address(reference r) { return &r; }
//    inline const_pointer address(const_reference r) { return &r; }
//
//    //    memory allocation
//    inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0) 
//	{ return reinterpret_cast<pointer>(::operator new(cnt * sizeof (T))); }
//
//    inline void deallocate(pointer p, size_type) 
//	{ ::operator delete(p); }
//
//    // Max allocation size possible
//    inline size_type max_size() const  
//	{ return std::numeric_limits<size_type>::max() / sizeof(T); }
//
//    //    construction/destruction
//    inline void construct(pointer p, const T& t) 
//	{ 
//
//		//new(p) T(t); 
//	}
//    inline void destroy(pointer p) 
//	{
//		//p->~T(); 
//	}
//
//	inline bool operator==(allocator const&) { return true; }
//	inline bool operator!=(allocator const& a) { return !operator==(a); }
//};    //    end of class Allocator 

} // end namespace memory
} // end namespace distant