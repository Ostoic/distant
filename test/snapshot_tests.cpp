#include "stdafx.h"
#include "CppUnitTest.h"

#include <distant/process.hpp>
#include <distant/snapshot.hpp>

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace distant_unit_tests
{
	TEST_CLASS(snapshot_tests)
	{
	public:
		TEST_METHOD(snapshot_test)
		{
			using namespace distant;

			snapshot<process<>> snapshot;
			// Ensure processes returned by the snapshot are active.
			for (auto proc : snapshot)
			{
				Assert::IsTrue(proc.valid());
				Assert::IsTrue(proc.is_active());
				Assert::IsTrue(proc.id() > 0);
			}

			//auto proc = *snapshot.begin();
			//proc.is_64bit();

			const auto current = current_process();

			// The current process must be among the processes in the snapshot
			/*const auto it = std::find_if(snapshot.begin(), snapshot.end(), [&current](const auto& process)
			{
				return process == current;
			});

			Assert::IsTrue(it != snapshot.end());*/

			// The snapshot can never be empty
			Assert::IsTrue(!snapshot.as<std::vector>().empty());
		}
	};
}