#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <thread>
#include <future>
#include <condition_variable>

#include <distant/config.hpp>
#include <distant/process.hpp>
#include <distant/virtual_memory.hpp>
#include <distant/system.hpp>
#include <distant/snapshot.hpp>

#include <boost/function.hpp>

int main()
{
	using distant::snapshot;
	using distant::remote_thread;
	using clock_type = std::chrono::steady_clock;

	std::ios_base::sync_with_stdio(false);

	try
	{
		auto thread_id = 0;
		void* thread_handle = nullptr;

		for (auto i = 0; i < 5; ++i)
		{
			remote_thread thread{ std::thread([]()
			{
				std::cout << "Thread is running...\n";
				std::this_thread::sleep_for(std::chrono::seconds(4));
				std::cout << "Thread is exiting...\n";
			}) };

			thread_id = static_cast<unsigned int>(thread.id());
			thread_handle = thread.handle().native_handle();
		}

		const auto start1 = std::chrono::steady_clock::now();
		const auto handle = ::OpenThread(THREAD_ALL_ACCESS, false, thread_id);
		const auto time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start1).count();

		const auto start2 = std::chrono::steady_clock::now();
		const auto id = ::GetThreadId(thread_handle);
		const auto time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start2).count();

		const auto start3 = std::chrono::steady_clock::now();
		const auto id2 = ::GetProcessIdOfThread(thread_handle);
		const auto time3 = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start3).count();

		std::cout << "OpenThread time = " << time1 << '\n';
		std::cout << "GetThreadId time = " << time2 << '\n';
		std::cout << "GetProcessIdOfThread time = " << time3 << '\n';

		const auto current_process = distant::current_process();
		snapshot<remote_thread> current_process_threads(current_process);

		std::cout << "Before join thread list:\n";
		for (const auto& thread : current_process_threads)
			std::cout << "Thread id = " << thread.id() << '\n';

		std::cout << "Joining threads...\n";
		for (auto&& thread : current_process_threads)
		{
			if (thread.id() != std::this_thread::get_id())
			{
				thread.join();
				std::cout << "Thread " << thread.id() << " joined!\n";
			}
		}

		std::cout << "After join thread list:\n";
		for (const auto& thread : current_process_threads)
		{
			std::cout << "Thread " << thread.id() << '\n';
			std::cout << "Alive: " << thread.is_active() << '\n';
		}
	}
	catch (distant::winapi_error& e)
	{
		std::cout << e << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}

	return 0;
}
