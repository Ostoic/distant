// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/config.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/winapi/basic_types.hpp>

#include <distant/kernel_objects/detail/snapshot_traits.hpp>
#include <distant/kernel_objects/fwd.hpp>

#include <memory>

namespace distant::kernel_objects
{
	/// @brief An iterator that lazily traverses the specifeid system snapshot upon being incremented.
	/// @remark snapshot_iterator models the InputIterator concept
	template <typename KernelObject>
	class snapshot_iterator :
		public boost::iterator_facade<
			snapshot_iterator<KernelObject> // Derived class
			, KernelObject					// Value type
			, std::input_iterator_tag		// Iterator traversal category
			, KernelObject					// Reference type (otherwise we return address of local variable)
		>									// The rest is use_default
	{
	private: // subtypes
		using snapshot_type = snapshot<KernelObject>;
		using snapshot_traits = detail::snapshot_traits<KernelObject>;
		using entry_t = typename snapshot_traits::entry_t;

	public: // {ctor}
		explicit snapshot_iterator(const snapshot_type& snapshot);

		constexpr snapshot_iterator();

	private:
		friend class boost::iterator_core_access;

		struct enabler {};

		bool check_current_validity();

		bool next();

		// interface for boost::iterator_facade
		void increment();

		KernelObject dereference() const;

		bool equal(const snapshot_iterator& other) const;

	private: // data
		boost::winapi::HANDLE_ native_snap_;
		boost::winapi::HANDLE_ object_handle_;

		/// ToolHelp entry type
		std::shared_ptr<entry_t> entry_;
	};
} // end namespace distant::system

// Implementation:
#include "impl/snapshot_iterator.hxx"
