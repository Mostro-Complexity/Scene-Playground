#pragma once
#include <iostream>
#include <xtensor\xio.hpp>
#include <xtensor\xarray.hpp>

namespace mostro
{
	namespace math
	{
		template<typename T> inline
			xt::xarray<T> gravity(T t, const xt::xarray<T>& r,
				const std::vector<xt::xarray<T>>& args)
		{
			T G = 6.67e-11f;
			xt::xarray<T> m = args[0];

			xt::xarray<T> dy = xt::zeros<T>({ r.size() });

			for (size_t i = 0; i < r.size(); i += 6)
			{
				xt::xarray<T> r2 = xt::zeros<T>({ 3 });
				for (size_t j = 0; j < r.size(); j += 6)
				{
					if (i != j)
					{
						xt::xarray<T> r__1 = xt::view(r, xt::range(j, j + 3));
						xt::xarray<T> r__2 = xt::view(r, xt::range(i, i + 3));

						r2 += G * m[j / 6] * (r__1 - r__2) / xt::pow(xt::sqrt(xt::sum(xt::pow(r__1 - r__2, 2))), 3);
					}
					xt::view(dy, xt::range(i, i + 6)) = xt::concatenate(xt::xtuple(
						xt::view(r, xt::range(i + 3, i + 6)),
						r2), 0);
				}
			}
			return dy;
		}

		//template<typename T, typename S> inline
		//	xt::xarray<T> rungeKuttaGenerator(const xt::xarray<T> &y0, S x0, S h,
		//		xt::xarray<T> func(T, const xt::xarray<T>&, const std::vector<xt::xarray<T>>&))
		//{
		//	static xt::xarray<T> x = x0; // TODO: static 多个对象使用？
		//	static xt::xarray<T> y = y0;
		//	xt::xarray<T> k1 = func(x0, y, args);
		//	xt::xarray<T> k2 = func(x0 + h / 2, y + h * k1 / 2, args);
		//	xt::xarray<T> k3 = func(x0 + h / 2, y + h * k2 / 2, args);
		//	xt::xarray<T> k4 = func(x0 + h, y + h * k3, args);
		//	y = y + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
		//	x += h;
		//	return y;
		//}


		template<typename T>
		struct RungeKuttaGenerator
		{
			RungeKuttaGenerator() {}

			RungeKuttaGenerator(const xt::xarray<T>& y0, T x0, T h,
				std::function<xt::xarray<T>(T, const xt::xarray<T>&, const std::vector<xt::xarray<T>>&)> func,
				const std::vector<xt::xarray<T>>& args = std::vector<xt::xarray<T>>())
			{
				this->x0 = x0;
				this->y0 = y0;
				this->x = x0;
				this->y = y0;
				this->func = func;
				this->h = h;
				this->args = args;
			}

			xt::xarray<T> next()
			{
				xt::xarray<T> k1 = func(x0, y, this->args);
				xt::xarray<T> k2 = func(x0 + h / 2, y + h * k1 / 2, this->args);
				xt::xarray<T> k3 = func(x0 + h / 2, y + h * k2 / 2, this->args);
				xt::xarray<T> k4 = func(x0 + h, y + h * k3, this->args);
				y += h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
				x += h;
				return y;
			}

			xt::xarray<T> y;
			xt::xarray<T> y0;
			T x0;
			T x;
			T h;
			std::function<xt::xarray<T>(T, const xt::xarray<T>&, const std::vector<xt::xarray<T>>&)> func;
			std::vector<xt::xarray<T>> args;
		};
	}
}

//int main()
//{
//	xt::xarray<double> a
//	{
//		0, 1e2, 1.2e1, 0, 0, 0,
//		1.8e1, 0, 1.32e2, 0, 0, 0,
//		1.54e3, 1.86e2, 0,0, 0, 0
//	};
//	for (size_t i = 0; i < 100000; i++)
//	{
//		std::cout << rungeKuttaGenerator(a, 0.0, 1.0, gravity<double>) << std::endl;
//	}
//
//	return 0;
//}