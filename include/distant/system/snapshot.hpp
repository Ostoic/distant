#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\handle.hpp>

#include <distant\system\snapshot_iterator.hpp>

#include <distant\type_traits.hpp>
#include <distant\utility\boolean_validator.hpp>

#include <vector>

namespace distant {
namespace system  {

	// system::snapshot models the ForwardRange concept
	template <typename KernelObject>
	class snapshot : public utility::boolean_validator<snapshot<KernelObject>>
	{
	public:/*
		static_assert(
			is_kernel_object<KernelObject>::value,
			"system::snapshot is iterable only for kernel objects.");*/

		using object_type = KernelObject;
		using handle_type = handle<snapshot>;
		
		using iterator = snapshot_iterator<KernelObject>;
		using const_iterator = snapshot_iterator<KernelObject>;

	public: // interface
		// Iterate over the snapshot range via the range-based for loop:
		iterator begin() const;
		iterator end() const;

		template <typename OutContainer = std::vector<KernelObject>>
		OutContainer get() const;

		template <typename Predicate, typename OutContainer = std::vector<KernelObject>>
		OutContainer get(Predicate) const;

		//operator output_type() const;

	public: // {ctor}
		snapshot();

	protected:
		friend class iterator;

		handle<snapshot> m_handle;
	};

} // end namespace system

using system::snapshot;

} // end namespace distant

#include <distant\system\impl\snapshot.hxx>
