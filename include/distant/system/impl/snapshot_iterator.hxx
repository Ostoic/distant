#pragma once
#include <distant\system\snapshot_iterator.hpp>

#include <distant\type_traits.hpp>

namespace distant::system {

// class snapshot_iterator <KernelObject>:
//public:
	template <typename K>
	inline snapshot_iterator<K>::snapshot_iterator(const snapshot_type& snapshot, snapshot_end) noexcept
		: m_native_snap(snapshot.m_handle.native_handle())
		, m_object_handle(nullptr) {}

	template <typename K>
	inline snapshot_iterator<K>::snapshot_iterator(const snapshot_type& snapshot)
		: m_native_snap(snapshot.m_handle.native_handle())
		, m_object_handle(nullptr)
	{
		// Bring the snapshot_entry dispatcher functions into scope
		namespace snapshot_entry = system::detail::snapshot_entry;

		m_entry.dwSize = sizeof(entry_type);
		if (!snapshot_entry::first<K>(m_native_snap, &m_entry))
			m_object_handle = nullptr;

		if (!this->current_valid())
			if (!this->next())
				m_object_handle = nullptr;
	}

	template <typename K>
	inline snapshot_iterator<K>::snapshot_iterator()
		: m_native_snap(nullptr)
		, m_object_handle(nullptr) {}

//private:
	template <typename K>
	inline bool snapshot_iterator<K>::current_valid()
	{
		namespace snapshot_entry = system::detail::snapshot_entry;

		// Open a handle to the kernel object
		m_object_handle = snapshot_entry::open_object<K>(
			snapshot_entry::get_id<K>(m_entry), 
			static_cast<boost::winapi::DWORD_>(
				get_access_rights<process<>>::value));

		CloseHandle(m_object_handle);

		return m_object_handle != nullptr;
	}

	template <typename K>
	inline bool snapshot_iterator<K>::next()
	{
		// Bring the snapshot_entry dispatcher function into scope
		namespace snapshot_entry = system::detail::snapshot_entry;

		// Continue iterating until we obtain a valid object handle, or until the API call fails.
		while (snapshot_entry::next<K>(m_native_snap, &m_entry))
		{
			// Check if we received a valid handle
			if (this->current_valid())
				return true;
		}

		return false;
	}

	template <typename K>
	inline void snapshot_iterator<K>::increment()
	{
		if (!this->next())
			this->m_object_handle = nullptr;
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
		return m_object_handle == other.m_object_handle;
	}

} // end namespace distant::system