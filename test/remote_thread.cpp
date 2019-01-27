#include "stdafx.h"
#define WIN32_LEAN_AND_MEAN

#include "CppUnitTest.h"

#include <iostream>
#include <set>
#include <distant/process.hpp>
#include <distant/virtual_memory.hpp>
#include <distant/system.hpp>
#include <distant/snapshot.hpp>

#include <boost/range/algorithm.hpp>
#include <boost/range.hpp>
#include <VersionHelpers.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace distant_unit_tests
{
	TEST_CLASS(remote_thread_tests)
	{
	private:
		void check_comparison_sanity(const distant::remote_thread& lhs, const distant::remote_thread& rhs)
		{
			const auto run = [](const auto& lhs, const auto& rhs)
			{
				Assert::IsTrue(lhs == rhs);
				Assert::IsTrue(!(lhs != rhs));
				Assert::IsFalse(lhs != rhs);
			};

			run(lhs, rhs);
			run(rhs, lhs);
		}

		void test_thread_snapshot_and_std_thread()
		{
			using distant::snapshot;
			using distant::remote_thread;
			bool good = false;

			try
			{
				const auto current_process = distant::current_process();

				std::vector<remote_thread> threads_vector;

				snapshot<remote_thread> before_snapshot(current_process);
				for (auto i = 0; i < 5; ++i)
				{
					threads_vector.emplace_back(std::thread([&good]()
					{
						while (!good)
							std::this_thread::sleep_for(std::chrono::milliseconds(0));
					}));
				}
				snapshot<remote_thread> after_snapshot(current_process);

				for (const auto& thread : threads_vector)
				{
					Assert::IsTrue(boost::find(before_snapshot, thread) == before_snapshot.end());
					Assert::IsTrue(boost::find(after_snapshot, thread) != after_snapshot.end());
				}

				good = true;
				for (auto&& thread : threads_vector)
				{
					if (thread.id() != std::this_thread::get_id())
						thread.join();
				}

				for (const auto& thread : threads_vector)
					Assert::IsTrue(boost::find(after_snapshot, thread) == after_snapshot.end());

			}
			catch (distant::winapi_error& e)
			{
				std::wstringstream out; out << e;
				Assert::Fail(out.str().c_str());
			}
			catch (std::exception& e)
			{
				std::wstringstream out; out << e.what();
				Assert::Fail(out.str().c_str());
			}
		}

	public:
		TEST_METHOD(remote_thread)
		{
			this->test_thread_snapshot_and_std_thread();

			const auto current_thread = distant::current_thread();
			this->check_comparison_sanity(current_thread, current_thread);
		}
	};
}