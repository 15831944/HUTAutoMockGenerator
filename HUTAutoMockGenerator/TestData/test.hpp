/* this is comment */

class Test
{
private:
	int a;
	char b;

	void privateMethod(int x, int y) {}
public:
	Test() {}
	virtual ~Test() {}

	virtual int publicVirtualMehtod(int a, int b, int c) = 0;

	static float statFloat;
	static void staticMethod() {}
};
