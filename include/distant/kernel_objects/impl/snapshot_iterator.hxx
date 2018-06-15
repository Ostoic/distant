// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/kernel_objects/snapshot_iterator.hpp>

#include <distant/type_traits.hpp>

namespace distant::kernel_objects
{
//class snapshot_iterator <KernelObject>:
//public:
	template <typename K>
	snapshot_iterator<K>::snapshot_iterator(const snapshot_type& snapshot)
		: native_snap_(snapshot.handle_.native_handle())
		, object_handle_(nullptr)
		, entry_(std::make_shared<entry_type>())
	{
		// Bring the snapshot_entry dispatcher functions into scope
		namespace snapshot_entry = detail::snapshot_entry;

		entry_->dwSize = sizeof(entry_type);
		if (!snapshot_entry::first<K>(native_snap_, *entry_))
			object_handle_ = nullptr;

		if (!this->current_valid())
			if (!this->next())
				object_handle_ = nullptr;
	}

	template <typename K>
	constexpr snapshot_iterator<K>::snapshot_iterator()
		: native_snap_(boost::winapi::INVALID_HANDLE_VALUE_)
		, object_handle_(nullptr) 
		, entry_(nullptr)
	{}

//private:
	template <typename K>
	bool snapshot_iterator<K>::current_valid()
	{
		namespace snapshot_entry = detail::snapshot_entry;

		// Open a handle to the kernel_object
		object_handle_ = snapshot_entry::open_object<K>(
			snapshot_entry::get_id<K>(*entry_),
			static_cast<boost::winapi::DWORD_>(
				get_access_rights<K>::value)
		);

		// If the handle is valid, then close it, since this only tests the validity of the object.
		boost::winapi::CloseHandle(object_handle_);
		return object_handle_ != nullptr;
	}

	template <typename K>
	bool snapshot_iterator<K>::next()
	{
		// Bring the snapshot_entry dispatcher function into scope
		namespace snapshot_entry = detail::snapshot_entry;

		// Continue iterating until we obtain a valid kernel_object handle, or until the API call fails.
		// Note: entry_ is reused internally, so a new \a entry_type is not created after each iterator increment.
		while (snapshot_entry::next<K>(native_snap_, *entry_))
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
			object_handle_ = nullptr;
	}

	template <typename K>
	K snapshot_iterator<K>::dereference() const
	{
		// Bring the snapshot_entry dispatcher function into scope
		namespace snapshot_entry = detail::snapshot_entry;

		const auto id = snapshot_entry::get_id<K>(*this->entry_);
		return K{id};
	}

	template <typename K>
	bool snapshot_iterator<K>::equal(const snapshot_iterator& other) const
	{
		return object_handle_ == other.object_handle_;
	}
} // end namespace distant::system
