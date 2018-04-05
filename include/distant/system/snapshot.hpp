#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant/handle.hpp>

#include <distant/system/snapshot_iterator.hpp>

#include <distant/utility/boolean_validator.hpp>

#include <vector>

namespace distant 
{
	namespace system  
	{
		// system::snapshot models the ForwardRange concept
		template <typename KernelObject>
		class snapshot : public utility::boolean_validator<snapshot<KernelObject>>
		{
		public:
			using object_type = KernelObject;
			using handle_type = handle<snapshot>;

			using iterator = snapshot_iterator<KernelObject>;
			using const_iterator = snapshot_iterator<KernelObject>;

		public: // interface
			// Iterate over the snapshot range via the range-based for loop:
			iterator begin() const;
			iterator end() const;

			template <template <typename, typename> class OutContainer>
			OutContainer<KernelObject, std::allocator<KernelObject>> as() const;

			template <template <typename, typename> class OutContainer, typename Predicate>
			OutContainer<KernelObject, std::allocator<KernelObject>> as(Predicate) const;

		public: // {ctor}
			snapshot();

		protected:
			friend class iterator;

			handle<snapshot> handle_;
		};

	} // namespace system

	using system::snapshot;

} // namespace distant

// Implementation:
#include <distant/system/impl/snapshot.hxx>
