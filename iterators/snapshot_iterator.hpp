#pragma once

#include <Tlhelp32.h>

#include <boost\iterator\iterator_facade.hpp>

#include <distant\utility\type_traits.hpp>

#include <distant\windows\kernel\process.hpp>
#include <distant\windows\system\snapshot.hpp>

namespace distant	{
namespace iterators {

	template <typename Kernel_Object>
	class snapshot_iterator : 
		public boost::iterator_facade<
			  snapshot_iterator<Kernel_Object> // Derived class
			, Kernel_Object					   // Value type
			, boost::forward_traversal_tag	   // Iterator traversal category
		>	// The rest is use_default
	{
	private:
		using snapshot_type = windows::system::snapshot<Kernel_Object>;

	public:
		snapshot_iterator(const snapshot_type& snapshot) : m_snapshot(snapshot)
		{}

	private:
		friend class boost::iterator_core_access;

		void increment() {}

		bool equal(const snapshot_iterator& other)
		{
			return m_snapshot == other.m_snapshot;
		}

	private:
		const snapshot_type& m_snapshot;

	};

} // end namespace iterators
} // end namespace distant