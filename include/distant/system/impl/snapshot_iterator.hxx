#pragma once
#include <distant/system/snapshot_iterator.hpp>

#include <distant/type_traits.hpp>

namespace distant::system
{
// class snapshot_iterator <KernelObject>:
//public:
	template <typename K>
	snapshot_iterator<K>::snapshot_iterator(const snapshot_type& snapshot, snapshot_end) noexcept
		: native_snap_(snapshot.handle_.native_handle())
		, object_handle_(nullptr) {}

	template <typename K>
	snapshot_iterator<K>::snapshot_iterator(const snapshot_type& snapshot)
		: native_snap_(snapshot.handle_.native_handle())
		, object_handle_(nullptr)
	{
		// Bring the snapshot_entry dispatcher functions into scope
		namespace snapshot_entry = detail::snapshot_entry;

		this->entry_.dwSize = sizeof(entry_type);
		if (!snapshot_entry::first<K>(this->native_snap_, &this->entry_))
			this->object_handle_ = nullptr;

		if (!this->current_valid())
			if (!this->next())
				this->object_handle_ = nullptr;
	}

	template <typename K>
	snapshot_iterator<K>::snapshot_iterator()
		: native_snap_(nullptr)
		, object_handle_(nullptr) {}

//private:
	template <typename K>
	bool snapshot_iterator<K>::current_valid()
	{
		namespace snapshot_entry = detail::snapshot_entry;

		// Open a handle to the kernel object
		this->object_handle_ = snapshot_entry::open_object<K>(
			snapshot_entry::get_id<K>(this->entry_),
			static_cast<boost::winapi::DWORD_>(
				get_access_rights<process<>>::value));

		CloseHandle(this->object_handle_);

		return this->object_handle_ != nullptr;
	}

	template <typename K>
	bool snapshot_iterator<K>::next()
	{
		// Bring the snapshot_entry dispatcher function into scope
		namespace snapshot_entry = detail::snapshot_entry;

		// Continue iterating until we obtain a valid object handle, or until the API call fails.
		while (snapshot_entry::next<K>(this->native_snap_, &this->entry_))
		{
			// Check if we received a valid handle
			if (this->current_valid())
				return true;
		}

		return false;
	}

	template <typename K>
	void snapshot_iterator<K>::increment()
	{
		if (!this->next())
			this->object_handle_ = nullptr;
	}

	template <typename K>
	K snapshot_iterator<K>::dereference() const
	{
		// Bring the snapshot_entry dispatcher function into scope
		namespace snapshot_entry = detail::snapshot_entry;

		const auto id = snapshot_entry::get_id<K>(this->entry_);
		return K{id};
	}

	template <typename K>
	bool snapshot_iterator<K>::equal(const snapshot_iterator& other) const
	{
		return this->object_handle_ == other.object_handle_;
	}
} // end namespace distant::system
