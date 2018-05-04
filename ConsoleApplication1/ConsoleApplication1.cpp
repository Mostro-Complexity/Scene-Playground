// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <functional>

int transform(int a)
{
	return a * a;
}

void catcher(int i, const std::function<int(const int&)> &func)
{
	printf("%d\n", func(i));
}


int main()
{
	for (int i = 0; i < 10; i++)
	{
		catcher(i, [&](int ii) {return i + ii; });
	}
	return 0;
}

