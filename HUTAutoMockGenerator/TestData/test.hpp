/* this is comment */
#include <iostream>
#include "testinc.hpp"

namespace np
{
	class Test
	{
	private:
		int a;
		char b;

		void* privateMethod(const int& x, int y) const {
			int xz = 0;
		}
	public:
		Test() {}
		virtual ~Test() {}

		virtual int __stdcall publicVirtualMehtod(int a, int b, int c) = 0;

		static float statFloat;
		static void staticMethod() {}
	};
}

class Test1
{
private:
	int a1;
	char b1;

	void privateMethod1(int x1, int y1) {
		int xz1 = 0;
	}
public:
	Test1() {}
	virtual ~Test1() {}

	virtual int publicVirtualMehtod1(int a1, int b1, int c1) = 0;

	static float statFloat1;
	static void staticMethod1() {}
};
