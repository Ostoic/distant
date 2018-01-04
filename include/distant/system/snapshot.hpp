#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\handle.hpp>

#include <distant\system\snapshot_iterator.hpp>
#include <distant\kernel\object.hpp>

#include <distant\utility\type_traits.hpp>

namespace distant::system {

	// system::snapshot models the ForwardRange concept
	template <typename KernelObject>
	class snapshot : public kernel::object
	{
	public:
		static_assert(
			distant::is_kernel_object<KernelObject>::value,
			"system::snapshot is iterable only for kernel objects."
		);

		using object_type = KernelObject;
		using handle_type = handle<snapshot>;
		
		using iterator = snapshot_iterator<KernelObject>;
		using const_iterator = snapshot_iterator<KernelObject>;

	public: // interface
		// Iterate over the snapshot range via the range-based for loop:
		iterator begin() const;
		iterator end() const;

	public: // {ctor}
		explicit snapshot();
	};

} // end namespace distant::system

#include <distant\system\impl\snapshot.hxx>
