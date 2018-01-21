#pragma once
#include <distant\system\snapshot_iterator.hpp>

namespace distant::system {

// class snapshot_iterator <KernelObject>:
//public:
	template <typename K>
	inline snapshot_iterator<K>::snapshot_iterator(const snapshot_type& snapshot, snapshot_end) noexcept
		: m_native_snap(snapshot.m_handle.native_handle())
		, m_index(0) {}

	template <typename K>
	inline snapshot_iterator<K>::snapshot_iterator(const snapshot_type& snapshot)
		: m_native_snap(snapshot.m_handle.native_handle())
		, m_index(1)
	{
		// Bring the snapshot_entry dispatcher function into scope
		namespace snapshot_entry = system::detail::snapshot_entry;

		m_entry.dwSize = sizeof(entry_type);
		if (!snapshot_entry::first<K>(m_native_snap, &m_entry))
			m_index = 0;
	}

	template <typename K>
	inline snapshot_iterator<K>::snapshot_iterator()
		: m_native_snap(nullptr)
		, m_index(0) {}

//private:

	template <typename K>
	inline void snapshot_iterator<K>::increment()
	{
		// Bring the snapshot_entry dispatcher function into scope
		namespace snapshot_entry = system::detail::snapshot_entry;

		if (m_index)
		{
			if (!snapshot_entry::next<K>(m_native_snap, &m_entry))
				m_index = 0;
			else
				++m_index;
		}
	}

	template <typename K>
	inline K snapshot_iterator<K>::dereference() const
	{
		// Bring the snapshot_entry dispatcher function into scope
		namespace snapshot_entry = system::detail::snapshot_entry;

		const auto id = snapshot_entry::get_id<K>(m_entry);
		return K{id};
	}

	template <typename K>
	inline bool snapshot_iterator<K>::equal(const snapshot_iterator& other) const
	{
		return m_index == other.m_index;
	}

} // end namespace distant::system