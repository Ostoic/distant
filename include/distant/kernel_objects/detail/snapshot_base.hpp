#pragma once

#include <distant/kernel_objects/snapshot_iterator.hpp>
#include <distant/concepts/handleable.hpp>
#include <boost/iterator/filter_iterator.hpp>

namespace distant::kernel_objects::detail
{
	struct id_predicate
	{
	public:
		explicit id_predicate(const uint id) : id_(id) {}

		template <typename T>
		bool operator()(T&& t) const noexcept
		{
			return ::GetProcessIdOfThread(native_handle_of(std::forward<T>(t))) == id_;
		}

	private:
		uint id_;
	};

	template <class KernelObject>
	class snapshot_base
		: public concepts::handleable<snapshot_base<KernelObject>>
	{
	public:
		using iterator = snapshot_iterator<KernelObject>;
		using const_iterator = iterator;

		using snapshot_traits = detail::snapshot_traits<KernelObject>;

		template <class> friend struct concepts::handleable;

	public:
		snapshot_base()
			: handle_(snapshot_traits::create_snapshot_handle())
		{
			if (handle_ == nullptr)
				throw winapi_error("[snapshot::{ctor}] Invalid handle");
		}

		snapshot_base(snapshot_base&&) noexcept = default;

		/// @brief Construct a snapshot of \a KernelObjects owned by the given process.
		template <class KernelObject2>
		explicit snapshot_base(const KernelObject2& owner)
			: handle_([&owner]()
				-> kernel_handle
			{
				if (!is_valid(owner))
					return nullptr;

				return snapshot_traits::create_snapshot_handle(id_of(owner));
			}())
		{}

		/// @brief Retrieve the start of the snapshot.
		/// @return A \a snapshot_iterator pointing to the first element in the snapshot.
		const_iterator begin() const
		{ return const_iterator{ *this }; }

		/// @brief The end of the snapshot.
		/// @return a \a snapshot_iterator indicating an element past-the-end of the snapshot.
		const_iterator end() const
		{ return const_iterator{}; }

		/// @brief Retrieve the start of the snapshot.
		/// @return A \a snapshot_iterator pointing to the first element in the snapshot.
		iterator begin()
		{ return iterator{ *this }; }

		/// @brief The end of the snapshot.
		/// @return a \a snapshot_iterator pointing past-the-end of the last element in the snapshot.
		iterator end()
		{ return iterator{}; }

	private:
		kernel_handle handle_;
	};

	template <>
	class snapshot_base<remote_thread>
		: public concepts::handleable<snapshot_base<remote_thread>>
	{
	public:
		using inner_iterator = snapshot_iterator<remote_thread>;
		using iterator = boost::filter_iterator<id_predicate, inner_iterator>;
		using const_iterator = iterator;

		using snapshot_traits = detail::snapshot_traits<remote_thread>;

		template <class> friend struct concepts::handleable;

	public:
		snapshot_base()
			: handle_(snapshot_traits::create_snapshot_handle())
			, parent_id_(0)
		{
			if (handle_ == nullptr)
				throw winapi_error("[snapshot::{ctor}] Invalid handle");
		}

		snapshot_base(snapshot_base&&) noexcept = default;

		/// @brief Construct a snapshot of \a KernelObjects owned by the given process.
		template <class KernelObject>
		explicit snapshot_base(const KernelObject& owner)
			: handle_([&owner]()
				-> kernel_handle
			{
				if (!is_valid(owner))
					return nullptr;

				return snapshot_traits::create_snapshot_handle(id_of(owner));
			}())
			, parent_id_(id_of(owner))
		{}

		/// @brief Retrieve the start of the snapshot.
		/// @return A \a snapshot_iterator pointing to the first element in the snapshot.
		const_iterator begin() const
		{
			return const_iterator{
				id_predicate{ parent_id_ },
				inner_iterator{ *this },
				inner_iterator{}
			};
		}

		/// @brief The end of the snapshot.
		/// @return a \a snapshot_iterator indicating an element past-the-end of the snapshot.
		const_iterator end() const
		{
			return const_iterator{
				id_predicate{ parent_id_ },
				inner_iterator{},
				inner_iterator{}
			};
		}

		/// @brief Retrieve the start of the snapshot.
		/// @return A \a snapshot_iterator pointing to the first element in the snapshot.
		iterator begin()
		{
			return iterator{
				id_predicate{ parent_id_ },
				inner_iterator{ *this },
				inner_iterator{}
			};
		}

		/// @brief The end of the snapshot.
		/// @return a \a snapshot_iterator pointing past-the-end of the last element in the snapshot.
		iterator end()
		{
			return iterator{
				id_predicate{ parent_id_ },
				inner_iterator{},
				inner_iterator{}
			};
		}

	private:
		kernel_handle handle_;
		uint parent_id_;
	};
}
