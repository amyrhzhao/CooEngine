#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Coo::Core;

namespace CoreTest 
{
	TEST_CLASS(MetaTest)
	{
	public:
		TEST_METHOD(TestMetaType) 
		{
			auto metaType = Meta::DeduceType<int>();
			Assert::AreEqual(metaType->GetName(), "Integer");
			Assert::IsTrue(metaType->GetCategory() == Meta::MetaType::Category::Primitive);
			Assert::IsTrue(metaType->GetSize() == sizeof(int));
		}

		TEST_METHOD(TestMetaPointer) 
		{
			float* intPtr = nullptr;
			auto metaType = Meta::DeduceType<float*>();

			Assert::AreEqual(metaType->GetName(), "Pointer");
			Assert::IsTrue(metaType->GetCategory() == Meta::MetaType::Category::Pointer);
			Assert::IsTrue(metaType->GetSize() == sizeof(nullptr));

			auto metaPointer = static_cast<const Meta::MetaPointer*>(metaType);
			auto metaPointerType = metaPointer->GetPointerType();
			Assert::AreEqual(metaPointerType->GetName(), "Number");
			Assert::IsTrue(metaPointerType->GetCategory() == Meta::MetaType::Category::Primitive);
			Assert::IsTrue(metaPointerType->GetSize() == sizeof(float));
		}

		TEST_METHOD(TestMetaArray) 
		{
			using IntVec = std::vector<int>;
			auto metaType = Meta::DeduceType<IntVec>();

			Assert::AreEqual(metaType->GetName(), "Array");
			Assert::IsTrue(metaType->GetCategory() == Meta::MetaType::Category::Array);
			Assert::IsTrue(metaType->GetSize() == sizeof(IntVec));

			IntVec myInts;
			myInts.push_back(1);
			myInts.push_back(5);
			myInts.push_back(17);
			
			auto metaArray = static_cast<const Meta::MetaArray*>(metaType);
			Assert::IsTrue(metaArray->GetElementType() == Meta::DeduceType<int>());
			for (size_t i = 0; i < metaArray->GetElementCount(&myInts); ++i) 
			{
				Assert::IsTrue(*(int*)(metaArray->GetElement(&myInts, i)) == myInts[i]);
			}
		}
	};
}