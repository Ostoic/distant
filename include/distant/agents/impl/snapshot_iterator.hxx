// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/agents/snapshot_iterator.hpp>

#include <distant/type_traits.hpp>
#include <distant/agents/interface.hpp>

namespace distant::agents
{
//class snapshot_iterator <Agent>:
//public:
	template <class K>
	template <class Snapshot>
	snapshot_iterator<K>::snapshot_iterator(const Snapshot& snapshot)
		: native_snap_(native_handle_of(snapshot))
		, object_handle_(nullptr)
		, entry_(std::make_shared<entry_t>())
	{
		entry_->dwSize = sizeof(entry_t);
		if (!snapshot_traits::first(native_snap_, *entry_))
			object_handle_ = nullptr;

		if (!this->check_current_validity())
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
	bool snapshot_iterator<K>
		::check_current_validity()
	{
		using object_traits = kernel_object_traits<K>;
		using handle_traits = handle_traits<typename object_traits::handle_t>;

		// Open a handle to the kernel_object
		object_handle_ = object_traits::open(snapshot_traits::get_id(*entry_));

		const bool result = object_traits::is_valid_handle(object_handle_);

		if (result)
			handle_traits::close(object_handle_);

		return result;
	}

	template <typename K>
	bool snapshot_iterator<K>
		::next()
	{
		// Continue iterating until we obtain a valid kernel_object handle, or until the API call fails.
		// Note: entry_ is reused internally, so a new \a entry_type is not created after each iterator increment.
		while (snapshot_traits::next(native_snap_, *entry_))
		{
			// Check if we received a valid handle
			if (this->check_current_validity())
				return true;
		}

		return false;
	}

	template <typename K>
	void snapshot_iterator<K>
		::increment()
	{
		if (!this->next())
			object_handle_ = nullptr;
	}

	template <typename K>
	K snapshot_iterator<K>
		::dereference() const
	{
		using id_t = typename kernel_object_traits<K>::id_t;
		return K{ id_t{ snapshot_traits::get_id(*entry_) } };
	}

	template <typename K>
	bool snapshot_iterator<K>
		::equal(const snapshot_iterator& other) const
	{
		return object_handle_ == other.object_handle_;
	}
} // end namespace distant::system
