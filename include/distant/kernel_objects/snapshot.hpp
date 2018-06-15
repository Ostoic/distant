// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/config.hpp>
#include <distant/handle.hpp>

#include <distant/kernel_objects/snapshot_iterator.hpp>

#include <distant/utility/boolean_validator.hpp>

namespace distant 
{
	namespace kernel_objects  
	{
		/// @brief A snapshot is a read-only copy of the current state of one or more of the following lists that reside in system memory: processes, threads, modules, and heaps.
		/// snapshot is a range modelling [InputRange](http://en.cppreference.com/w/cpp/experimental/ranges/range/InputRange) whose elements consist of valid instances of the specified
		/// \a KernelObject.
		/// @tparam KernelObject must be one of the following: process, thread, heap, module.
		template <typename KernelObject>
		class snapshot : public utility::boolean_validator<snapshot<KernelObject>>
		{
		public:
			using object_type = KernelObject;
			using handle_type = handle<snapshot>;

			using iterator		 = snapshot_iterator<KernelObject>;
			using const_iterator = snapshot_iterator<KernelObject>;

		public: // interface
			/// @brief Retrieve the start of the snapshot.
			/// @return A \a snapshot_iterator pointing to the first element in the snapshot.
			iterator begin() const;

			/// @brief The end of the snapshot.
			/// @return a \a snapshot_iterator indicating an element past-the-end of the snapshot.
			iterator end() const;

			/// @brief Retrieve the start of the snapshot.
			/// @return A \a snapshot_iterator pointing to the first element in the snapshot.
			iterator begin();

			/// @brief The end of the snapshot.
			/// @return a \a snapshot_iterator pointing past-the-end of the last element in the snapshot.
			iterator end();

			/// @brief Store a permanent copy of the snapshot as a container
			template <template <typename, typename> class OutContainer>
			OutContainer<KernelObject, std::allocator<KernelObject>> as() const;

			/// @brief Store a permanent copy of the snapshot as a container whose elements satisfy the given \a Predicate.
			/// @tparam Predicate the \a Predicate function \a KernelObjects of the \a OutContainer must satisfy.
			/// @tparam OutContainer the container in which to store the \a KernelObjects.
			template <template <typename, typename> class OutContainer, typename Predicate>
			OutContainer<KernelObject, std::allocator<KernelObject>> as(Predicate) const;

		public: // {ctor}

			/// @brief Default construct a snapshot of all \a KernelObjects at the current time.
			snapshot();

			/// @brief Construct a snapshot of \a KernelObjects owned by the given process.
			snapshot(const process<>&);

		private:
			friend class iterator;

			handle<snapshot> handle_;
		};

	} // namespace system

	using kernel_objects::snapshot;

} // namespace distant

// Implementation:
#include "impl/snapshot.hxx"
