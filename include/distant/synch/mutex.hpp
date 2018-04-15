#pragma once

#include <distant/handle.hpp>

#include <boost/winapi/wait.hpp>

namespace distant {
namespace synch   {
	/// @brief A system-wide mutex satisfying the [Lockable](http://en.cppreference.com/w/cpp/concept/Lockable) concept.
	class mutex
	{
	public:
		using native_handle_type = boost::winapi::HANDLE_;

	public:
		/// @brief Default construct an anonymous mutex.
		/// The mutex is not initially locked.
		/// @exception Throws a std::system_error in the event an API call fails.
		mutex();

		/// @brief Construct a named mutex that can be newly created, or opens an existing mutex.
		/// The mutex is not initially locked.
		/// @param name the name of the mutex.
		/// @param create_new specifies whether or not to create a new mutex, or to open an existing one.
		/// @exception Throws a std::system_error in the event an API call fails.
		explicit mutex(const std::wstring& name, bool create_new = true);

		/// @brief Lock the mutex.
		/// @exception Throws a std::system_error in the event an API call fails.
		void lock();

		/// @brief Unlock the mutex.
		void unlock() noexcept;

		/// @brief Nonblocking lock that immediately returns.
		/// @return true if the lock was successful, false otherwise.
		/// @exception Throws a std::system_error in the event an API call fails.
		bool try_lock();

		/// @brief Get the handle to the mutex.
		/// @return the handle.
		const distant::handle<mutex>& handle() const noexcept;

		mutex(const mutex&) = delete;
		mutex& operator=(const mutex&) = delete;

	private:
		distant::handle<mutex> handle_;
	};

	template <typename Mutex>
	class lock_guard
	{
	public:
		/// @brief Construct a guard that will attempt to lock the mutex, or will do nothing if the mutex is adopted.
		/// @param mutex the mutex to guard.
		/// @param adopt_mutex determines whether the mutex will be locked or not.
		lock_guard(Mutex& mutex, bool adopt_mutex = false);

		/// @brief Unlock the mutex upon destruction of the guard.
		~lock_guard() noexcept;

		lock_guard(const lock_guard&) = delete;
		lock_guard& operator=(const lock_guard&) = delete;
	private:
		Mutex& mutex_;
	};

} // namespace synch

	using synch::mutex;

} // namespace distant

// Implementation
#include "impl/mutex.hxx"