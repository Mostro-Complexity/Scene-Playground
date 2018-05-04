#include "stdafx.h"
#include <memory>
#include <vector>

struct shitshit
{
	int aa;
};

class shit
{
public:
	shit(shitshit * s, shitshit *nake, const std::vector<std::shared_ptr<shitshit>>& l)
	{
		ptr = std::shared_ptr<shitshit>(s);
		list = l;
	}
	~shit() {}

private:
	std::shared_ptr<shitshit> ptr;
	std::vector<std::shared_ptr<shitshit>> list;
	shitshit *nake;
};

int main()
{
	shit s(new shitshit(), new shitshit(),
		std::vector<std::shared_ptr<shitshit>>{
		std::shared_ptr<shitshit>(new shitshit()),
			std::shared_ptr<shitshit>(new shitshit())
	}
	);
	return 0;
}