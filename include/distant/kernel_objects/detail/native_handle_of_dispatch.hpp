#pragma once

#include <boost/mp11/mpl.hpp>

namespace distant::kernel_objects
{
	template <class KernelObject>
	struct native_handle_traits;

	namespace detail
	{
		using namespace boost::mp11;

		template <class Object>
		using native_handle_member_t = decltype(std::declval<Object>().native_handle());

		template <class Object>
		using handle_member_t = decltype(std::declval<Object>().handle());

		template <class Object>
		using has_native_handle_member = mp_valid<native_handle_member_t, Object>;

		template <class Object>
		using has_handle_member = mp_valid<handle_member_t, Object>;

		template <class T>
		using is_handle_type = std::is_scalar<T>;

		struct handle_member_impl
		{
			template <class Object>
			static auto native_handle(Object&& object) noexcept
			{
				using handle_return_t = utility::meta::remove_cvref<decltype(std::declval<Object>().handle())>;
				return native_handle_of(std::forward<Object>(object).handle());
			}
		};

		struct native_handle_member_impl
		{
			template <class Object>
			static auto native_handle(Object&& object) noexcept
			{
				return std::forward<Object>(object).native_handle();
			}
		};

		struct handle_type_impl
		{
			template <class Handle>
			static constexpr auto native_handle(const Handle& handle) noexcept
			{
				return handle;
			}
		};

		template <class T>
		using native_handle_impl = mp_cond<
			mp_bool<has_handle_member<T>::value>,		 handle_member_impl,		// handle member function		 -> .handle().native_handle()
			mp_bool<has_native_handle_member<T>::value>, native_handle_member_impl,	// native_handle member function -> .native_handle
			mp_bool<is_handle_type<T>::value>,			 handle_type_impl,			// handle type -> identity
			mp_bool<true>,								 void						// default case -> no native_handle
		>;

	} // namespace detail
	
} // namespace distant::kernel_objects 
