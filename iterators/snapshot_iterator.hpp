#pragma once

#include <boost\iterator\iterator_facade.hpp>

#include <distant\utility\type_traits.hpp>

#include <distant\windows\kernel\process.hpp>
#include <distant\windows\system\detail\snapshot_traits.hpp>
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

		snapshot_iterator(const snapshot_type& snapshot, snapshot_end)
			: m_native_snap(expose::native_handle(snapshot.get_handle()))
			, m_index(0) {}

		snapshot_iterator(const snapshot_type& snapshot)
			: m_native_snap(expose::native_handle(snapshot.get_handle()))
			, m_index(1)
		{
			m_entry.dwSize = sizeof(entry_type);
			if (!Process32First(m_native_snap, &m_entry))
				m_index = 0;
		}

		snapshot_iterator()
			: m_native_snap(NULL)
			, m_index(0) {}

	private:
		friend class boost::iterator_core_access;

		void increment() 
		{
			if (m_index)
			{
				if (!Process32Next(m_native_snap, &m_entry))
					m_index = 0;
				else
					m_index++;
			}
		}

		Kernel_Object dereference() const
		{
			auto pid = m_entry.th32ProcessID;
			return Kernel_Object(pid);
		}

		bool equal(const snapshot_iterator& other) const
		{
			return m_index == other.m_index;
		}

	private:
		using expose = distant::detail::attorney::to_handle<snapshot_iterator>;

		index_type m_index;
		const HANDLE m_native_snap;

		entry_type m_entry;
	};

} // end namespace distant::iterators