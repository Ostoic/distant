#pragma once

#include <boost\iterator\iterator_facade.hpp>

#include <distant\utility\type_traits.hpp>

//#include <distant\windows\kernel\process.hpp>
#include <distant\windows\system\detail\tool_help.hpp>
#include <distant\detail\attorney.hpp>

// Forward declare snapshot
namespace distant::windows::system { template <class> class snapshot; }
namespace distant::iterators {

	template <typename Kernel_Object>
	class snapshot_iterator : 
		public boost::iterator_facade<
			  snapshot_iterator<Kernel_Object> // Derived class
			, Kernel_Object					   // Value type
			, boost::forward_traversal_tag	   // Iterator traversal category
			, Kernel_Object					   // Reference type (otherwise we return address of local variable)
		>	// The rest is use_default
	{
	private:
		using index_type = std::size_t;
		using snapshot_type = windows::system::snapshot<Kernel_Object>;
		using entry_type = typename windows::system::detail::snapshot_dispatcher<Kernel_Object>::entry_type;

	public:
		class snapshot_end {};

		explicit snapshot_iterator(const snapshot_type& snapshot, snapshot_end)
			: m_native_snap(expose::native_handle(snapshot.get_handle()))
			, m_index(0) {}

		explicit snapshot_iterator(const snapshot_type& snapshot)
			: m_native_snap(expose::native_handle(snapshot.get_handle()))
			, m_index(1)
		{
			// Bring the snapshot_entry implementation free functions into scope
			using windows::system::detail::snapshot_entry::first;

			m_entry.dwSize = sizeof(entry_type);
			if (!first<Kernel_Object>(m_native_snap, &m_entry))
				m_index = 0;
		}

		snapshot_iterator()
			: m_native_snap(NULL)
			, m_index(0) {}

	private:
		friend class boost::iterator_core_access;

		void increment() 
		{
			// Bring the snapshot_entry implementation free functions into scope
			using windows::system::detail::snapshot_entry::next;

			if (m_index)
			{
				if (!next<Kernel_Object>(m_native_snap, &m_entry))
					m_index = 0;
				else
					m_index++;
			}
		}

		Kernel_Object dereference() const
		{
			// Bring the snapshot_entry implementation get_id into scope
			using windows::system::detail::snapshot_entry::get_id;

			auto pid = get_id<Kernel_Object>(m_entry);
			return Kernel_Object(pid);
		}

		bool equal(const snapshot_iterator& other) const
		{
			return m_index == other.m_index;
		}

	private:
		using expose = distant::detail::attorney::to_handle<snapshot_iterator>;

		index_type m_index;
		HANDLE m_native_snap;

		entry_type m_entry;
	};

} // end namespace distant::iterators