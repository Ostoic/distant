// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/config.hpp>
#include <distant/scoped_handle.hpp>

#include <distant/kernel_objects/detail/snapshot_traits.hpp>

#include <distant/kernel_objects/detail/snapshot_base.hpp>
#include <distant/concepts/boolean_validator.hpp>

namespace distant
{
	namespace kernel_objects
	{
		/// @brief A snapshot is a read-only copy of the current state of one or more of the following lists that reside in system memory: processes, threads, modules, and heaps.
		/// snapshot is a range modelling [InputRange](http://en.cppreference.com/w/cpp/experimental/ranges/range/InputRange) whose elements consist of valid instances of the specified
		/// \a KernelObject.
		/// @tparam KernelObject must be one of the following: process, thread, heap, module.
		template <class KernelObject>
		class snapshot
			: private detail::snapshot_base<KernelObject>
			, public concepts::boolean_validator<snapshot<KernelObject>>
		{
			using base = detail::snapshot_base<KernelObject>;

		public:
			using object_t = KernelObject;

			using base::iterator;
			using base::const_iterator;

			using base::begin;
			using base::end;

		public: // interface
			/// @brief Store a permanent copy of the snapshot as a container
			template <template <class, class> class OutContainer>
			OutContainer<KernelObject, std::allocator<KernelObject>> as() const;

			/// @brief Store a permanent copy of the snapshot as a container whose elements satisfy the given \a Predicate.
			/// @tparam Predicate the \a Predicate function \a KernelObjects of the \a OutContainer must satisfy.
			/// @tparam OutContainer the container in which to store the \a KernelObjects.
			template <template <class, class> class OutContainer, class Predicate>
			OutContainer<KernelObject, std::allocator<KernelObject>> as(Predicate) const;

			bool valid() const noexcept
			{ return base::handle.valid(); }

		public: // {ctor}

			/// @brief Default construct a snapshot of all \a KernelObjects at the current time.
			snapshot() = default;
			snapshot(snapshot&& other) noexcept = default;

			/// @brief Construct a snapshot of \a KernelObjects owned by the given process.
			template <class OwnerObject>
			explicit snapshot(const OwnerObject& owner);
		};

	} // namespace system

	using kernel_objects::snapshot;

} // namespace distant

// Implementation:
#include "impl/snapshot.hxx"
