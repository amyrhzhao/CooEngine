#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Coo::Core;

namespace CoreTest
{
	TEST_CLASS(TypedAllocatorTest)
	{
	public:
		class Bar
		{
		public:
			static int counter;
			Bar() { counter = 0; }
			Bar(const Bar&) { counter = 1; }
			Bar(Bar&&) { counter = 2; }
		};

		class Foo
		{
		public:
			int a, b;
			Foo() : a(42), b(7) {}
			Foo(int i, int j) : a(i), b(j) {}
			Foo(Bar b) : a(12), b(34) {}
			Foo(std::string str) : a(12), b(34) {}
			~Foo() { a = 0xfeeefeee;	b = 0xdeadbeef; }
		};

		TEST_METHOD(NewTest) 
		{
			Foo foo("Peter");

			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New();
			Assert::IsNotNull(ptr);
			Assert::IsTrue(ptr->a == 42);
			Assert::IsTrue(ptr->b == 7);

			Foo* ptr2 = typedAllocator.New();
			Assert::IsNull(ptr2);
		}

		TEST_METHOD(OverloadedNewTest)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New(36, 48);
			Assert::IsNotNull(ptr);
			Assert::IsTrue(ptr->a == 36);
			Assert::IsTrue(ptr->b == 48);

			Foo* ptr2 = typedAllocator.New();
			Assert::IsNull(ptr2);
		}

		TEST_METHOD(ForwardingTest) 
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New(Bar());
			Assert::IsNotNull(ptr);
			Assert::AreEqual(Bar::counter, 2);
		}

		TEST_METHOD(DeleteTest)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New();
			Assert::IsNotNull(ptr);

			typedAllocator.Delete(ptr);
			Assert::AreEqual((unsigned int)ptr->a, 0xfeeefeee);
			Assert::AreEqual((unsigned int)ptr->b, 0xdeadbeef);
		}

		TEST_METHOD(RecyclingTest)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New();
			Assert::IsNotNull(ptr);

			typedAllocator.Delete(ptr);

			Foo* ptr2 = typedAllocator.New();
			Assert::IsNotNull(ptr2);
			Assert::IsTrue(ptr == ptr2);
		}
	};

	int TypedAllocatorTest::Bar::counter = 0;
}