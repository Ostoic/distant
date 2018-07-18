#include "stdafx.h"
#include "CppUnitTest.h"

#include <distant/virtual_memory.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace distant_unit_tests
{
	TEST_CLASS(virtual_memory_test)
	{
	public:
		TEST_METHOD(virtual_ptr_arithmetic)
		{
			using namespace distant;

			int array[] = {1, 2, 3};
			auto current = current_process();
			auto ptr = make_virtual_ptr<int>(current, array);
			*ptr = 3; ++ptr; *ptr = 4;

			Assert::IsTrue(array[0] == 3);
			Assert::IsTrue(array[1] == 4);
			Assert::IsTrue(array[2] == 3);
		}

		TEST_METHOD(virtual_ptr_semantics)
		{
			using namespace distant;

			int array[] = { 1, 2, 3 };
			auto current = current_process();
			auto ptr = make_virtual_ptr<int>(current, array);

			Assert::IsTrue(
				array == reinterpret_cast<int*>(static_cast<distant::dword>(
						 ptr.get())
			));

			address x = 2;
			Assert::IsTrue(ptr == ptr);
			Assert::IsTrue((++ptr).get() != 0);
			Assert::IsTrue((--ptr).get() != 0);
			Assert::IsTrue(ptr + 4 != ptr);

			ptr = nullptr;
			Assert::IsTrue(ptr == nullptr);
		}

		TEST_METHOD(virtual_ref_basic)
		{
			using namespace distant;

			auto current = current_process();

			int a = 1, b = 2;
			auto ref = make_virtual_reference<int>(current, &a);

			ref = 3;
			Assert::IsTrue(a == 3);
		}

		TEST_METHOD(virtual_malloc_free)
		{
			using namespace distant;

			try
			{
				auto current = current_process();
				const auto ptr = virtual_malloc<int>(current);

				//Assert::IsTrue(ptr != nullptr);

				*ptr = 3;
				Assert::IsTrue(*ptr == 3);
				virtual_free(current, ptr);
			}
			catch (std::system_error& e)
			{
				std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
				std::wstring message = converter.from_bytes(e.what());
				message += L" ";
				message += converter.from_bytes(e.code().message().data());
				Assert::Fail((message).data());
			}
		}
	};
}