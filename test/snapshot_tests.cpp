#include "stdafx.h"
#include "CppUnitTest.h"

#include <distant/process.hpp>
#include <distant/snapshot.hpp>

#include <boost/range/algorithm.hpp>

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

			// Ensure MSVC range-based for handles default-constructed ranges properly.
			for (auto p : snapshot<process<>>{}) 
				break;

			snapshot<process<>> snapshot;
			Assert::IsTrue(snapshot.valid());
			Assert::IsTrue(snapshot.handle().valid());

			// Ensure processes returned by the snapshot are active.
			for (auto proc : snapshot)
			{
				Assert::IsTrue(proc.valid());
				Assert::IsTrue(proc.is_active());
				Assert::IsTrue(static_cast<unsigned int>(proc.id()) > 0);
			}

			const auto current = current_process();

			// The current process must be among the processes in the snapshot
			Assert::IsTrue(boost::find_if(snapshot, [&current](const auto& process)
			{
				return process == current;
			}) != snapshot.end());

			// The snapshot can never be empty
			Assert::IsTrue(boost::size(snapshot) > 0);
			Assert::IsTrue(!snapshot.as<std::vector>().empty());
		}
	};
}