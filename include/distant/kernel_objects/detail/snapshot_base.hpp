#pragma once

#include <distant/kernel_objects/snapshot_iterator.hpp>
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
	{
	public:
		using iterator = snapshot_iterator<KernelObject>;
		using const_iterator = iterator;

		using snapshot_traits = detail::snapshot_traits<KernelObject>;

	public:
		snapshot_base()
			: handle(snapshot_traits::create_snapshot_handle())
		{
			if (handle == nullptr)
				throw winapi_error("[snapshot::{ctor}] Invalid handle");
		}

		snapshot_base(snapshot_base&& other) noexcept = default;

		/// @brief Construct a snapshot of \a KernelObjects owned by the given process.
		template <class KernelObject2>
		explicit snapshot_base(const KernelObject2& owner)
			: handle([&owner]()
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

		auto native_handle() const { return handle.native_handle(); }

		mutable kernel_handle handle;
	};

	template <>
	class snapshot_base<distant::thread>
	{
	public:
		using inner_iterator = snapshot_iterator<distant::thread>;
		using iterator = boost::filter_iterator<id_predicate, inner_iterator>;
		using const_iterator = iterator;

		using snapshot_traits = detail::snapshot_traits<distant::thread>;

	public:
		snapshot_base()
			: handle(snapshot_traits::create_snapshot_handle())
			, parent_id(0)
		{
			if (handle == nullptr)
				throw winapi_error("[snapshot::{ctor}] Invalid handle");
		}

		snapshot_base(snapshot_base&& other) noexcept = default;

		/// @brief Construct a snapshot of \a KernelObjects owned by the given process.
		template <class KernelObject>
		explicit snapshot_base(const KernelObject& owner)
			: handle([&owner]()
				-> kernel_handle
			{
				if (!is_valid(owner))
					return nullptr;

				return snapshot_traits::create_snapshot_handle(id_of(owner));
			}())
			, parent_id(id_of(owner))
		{}

		/// @brief Retrieve the start of the snapshot.
		/// @return A \a snapshot_iterator pointing to the first element in the snapshot.
		const_iterator begin() const
		{
			return const_iterator{
				id_predicate{ parent_id },
				inner_iterator{ *this },
				inner_iterator{}
			};
		}

		/// @brief The end of the snapshot.
		/// @return a \a snapshot_iterator indicating an element past-the-end of the snapshot.
		const_iterator end() const
		{
			return const_iterator{
				id_predicate{ parent_id },
				inner_iterator{},
				inner_iterator{}
			};
		}

		/// @brief Retrieve the start of the snapshot.
		/// @return A \a snapshot_iterator pointing to the first element in the snapshot.
		iterator begin()
		{
			return iterator{
				id_predicate{ parent_id },
				inner_iterator{ *this },
				inner_iterator{}
			};
		}

		/// @brief The end of the snapshot.
		/// @return a \a snapshot_iterator pointing past-the-end of the last element in the snapshot.
		iterator end()
		{
			return iterator{
				id_predicate{ parent_id },
				inner_iterator{},
				inner_iterator{}
			};
		}

		auto native_handle() const { return handle.native_handle(); }

		mutable kernel_handle handle;
		uint parent_id;
	};
}
