#pragma once

#include <boost\iterator\iterator_facade.hpp>
#include <boost\winapi\basic_types.hpp>

#include <distant\system\detail\dispatch.hpp>
#include <distant\detail\attorney.hpp>
#include <distant\detail\fwd.hpp>

namespace distant::system {

	// snapshot_iterator models the ForwardIterator concept
	template <typename KernelObject>
	class snapshot_iterator : 
		public boost::iterator_facade<
			  snapshot_iterator<KernelObject> // Derived class
			, KernelObject					   // Value type
			, boost::forward_traversal_tag	   // Iterator traversal category
			, KernelObject					   // Reference type (otherwise we return address of local variable)
		>	// The rest is use_default
	{
	private: // subtypes
		using snapshot_type = system::snapshot<KernelObject>;
		using entry_type = typename system::detail::snapshot_dispatcher<KernelObject>::entry_type;

	public: // interface
		class snapshot_end {}; // sentinel value

	public: // {ctor}
		explicit snapshot_iterator(const snapshot_type& snapshot, snapshot_end) noexcept;

		explicit snapshot_iterator(const snapshot_type& snapshot);

		snapshot_iterator();

	private:
		friend class boost::iterator_core_access;

		bool current_valid();

		bool next();

		// interface for boost::iterator_facade
		void increment();

		KernelObject dereference() const;

		bool equal(const snapshot_iterator& other) const;

	private: // data
		boost::winapi::HANDLE_ m_object_handle;
		boost::winapi::HANDLE_ m_native_snap;

		/// ToolHelp entry type
		entry_type m_entry;
	};

} // end namespace distant::system

// Implementation:
#include <distant\system\impl\snapshot_iterator.hxx>