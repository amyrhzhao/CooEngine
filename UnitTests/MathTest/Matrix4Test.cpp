#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Coo::Math;

namespace MathTest 
{
	TEST_CLASS(Matrix4Test) 
	{
	public:
		TEST_METHOD(TestConstructor)
		{
			Matrix4 m0;
			Assert::AreEqual(1.0f, m0._11);
			Assert::AreEqual(0.0f, m0._12);
			Assert::AreEqual(0.0f, m0._13);
			Assert::AreEqual(0.0f, m0._14);
			Assert::AreEqual(0.0f, m0._21);
			Assert::AreEqual(1.0f, m0._22);
			Assert::AreEqual(0.0f, m0._23);
			Assert::AreEqual(0.0f, m0._24);
			Assert::AreEqual(0.0f, m0._31);
			Assert::AreEqual(0.0f, m0._32);
			Assert::AreEqual(1.0f, m0._33);
			Assert::AreEqual(0.0f, m0._34);
			Assert::AreEqual(0.0f, m0._41);
			Assert::AreEqual(0.0f, m0._42);
			Assert::AreEqual(0.0f, m0._43);
			Assert::AreEqual(1.0f, m0._44);

			Matrix4 m1{ 0.0f };
			Assert::AreEqual(0.0f, m1._11);
			Assert::AreEqual(0.0f, m1._12);
			Assert::AreEqual(0.0f, m1._13);
			Assert::AreEqual(0.0f, m1._14);
			Assert::AreEqual(0.0f, m1._21);
			Assert::AreEqual(0.0f, m1._22);
			Assert::AreEqual(0.0f, m1._23);
			Assert::AreEqual(0.0f, m1._24);
			Assert::AreEqual(0.0f, m1._31);
			Assert::AreEqual(0.0f, m1._32);
			Assert::AreEqual(0.0f, m1._33);
			Assert::AreEqual(0.0f, m1._34);
			Assert::AreEqual(0.0f, m1._41);
			Assert::AreEqual(0.0f, m1._42);
			Assert::AreEqual(0.0f, m1._43);
			Assert::AreEqual(0.0f, m1._44);

			Matrix4 m2
			{
				 0.0f,  1.0f,  2.0f,  3.0f,
				 4.0f,  5.0f,  6.0f,  7.0f,
				 8.0f,  9.0f, 10.0f, 11.0f,
				12.0f, 13.0f, 14.0f, 15.0f
			};
			Assert::AreEqual(0.0f, m2._11);
			Assert::AreEqual(1.0f, m2._12);
			Assert::AreEqual(2.0f, m2._13);
			Assert::AreEqual(3.0f, m2._14);
			Assert::AreEqual(4.0f, m2._21);
			Assert::AreEqual(5.0f, m2._22);
			Assert::AreEqual(6.0f, m2._23);
			Assert::AreEqual(7.0f, m2._24);
			Assert::AreEqual(8.0f, m2._31);
			Assert::AreEqual(9.0f, m2._32);
			Assert::AreEqual(10.0f, m2._33);
			Assert::AreEqual(11.0f, m2._34);
			Assert::AreEqual(12.0f, m2._41);
			Assert::AreEqual(13.0f, m2._42);
			Assert::AreEqual(14.0f, m2._43);
			Assert::AreEqual(15.0f, m2._44);
		}
	};
} // namespace MathTest