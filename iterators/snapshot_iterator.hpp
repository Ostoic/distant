#pragma once

#include <Tlhelp32.h>

#include <boost\iterator\iterator_facade.hpp>

#include <distant\windows\access_rights.hpp>
#include <distant\windows\kernel\process.hpp>

namespace distant	{
namespace iterators {

	template <windows::access_rights::process access>
	class snapshot_iterator : 
		public boost::iterator_facade<
			  snapshot_iterator<access>			// Derived class
			, windows::kernel::process<access>  // Value type
			, boost::forward_traversal_tag		// Iterator traversal category
		>	// The rest is use_default
	{
	public:
		//snapshot_iterator() :


	private:
		friend class boost::iterator_core_access;

		void increment() {}

		//bool equal(snapshot_iterator

	private:
	};

} // end namespace iterators
} // end namespace distant