// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/config.hpp>
#include <distant/scoped_handle.hpp>

#include <distant/agents/detail/snapshot_traits.hpp>

#include <distant/agents/detail/snapshot_base.hpp>
#include <distant/concepts/boolean_validator.hpp>

namespace distant
{
	namespace agents
	{
		/// @brief A snapshot is a read-only copy of the current state of one or more of the following lists that reside in system memory: processes, threads, modules, and heaps.
		/// snapshot is a range modelling [InputRange](http://en.cppreference.com/w/cpp/experimental/ranges/range/InputRange) whose elements consist of valid instances of the specified
		/// \a Agent.
		/// @tparam Agent must be one of the following: process, thread, heap, module.
		template <class Agent>
		class snapshot
			: private detail::snapshot_base<Agent>
		{
			using base = detail::snapshot_base<Agent>;

		public:
			using object_t = Agent;

			using base::iterator;
			using base::const_iterator;

			using base::end;
			using base::begin;

			using base::operator bool;
			using base::valid;
			using base::handle;

		public: // {ctor}
			/// @brief Default construct a snapshot of all \a Agents at the current time.
			// Note: Using = default will result in MSVC attempting to call the copy ctor for some reason.
			snapshot() {};
			snapshot(snapshot&&) noexcept = default;

			/// @brief Construct a snapshot of \a Agents owned by the given object.
			template <class OwnerObject>
			explicit snapshot(const OwnerObject& owner);

		public: // interface
			/// @brief Store a permanent copy of the snapshot as a container
			template <template <class, class> class OutContainer>
			OutContainer<Agent, std::allocator<Agent>> as() const;

			/// @brief Store a permanent copy of the snapshot as a container whose elements satisfy the given \a Predicate.
			/// @tparam Predicate the \a Predicate function \a Agents of the \a OutContainer must satisfy.
			/// @tparam OutContainer the container in which to store the \a Agents.
			template <template <class, class> class OutContainer, class Predicate>
			OutContainer<Agent, std::allocator<Agent>> as(Predicate) const;
		};

	} // namespace system

	using agents::snapshot;

	using process_snapshot = snapshot<distant::process<>>;
	using thread_snapshot = snapshot<distant::thread>;

} // namespace distant

// Implementation:
#include "impl/snapshot.hxx"
