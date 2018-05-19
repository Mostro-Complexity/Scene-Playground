#pragma once
#include "utility\Group.h"
#include "utility\ModelGroup.h"
#include "math\Math.h"
#include "utility\TrackGroup.h"

namespace mostro
{
	struct Planet
	{
		Planet(std::shared_ptr<utility::ModelGroup> model, float quality,
			const glm::vec3 &location, const glm::vec3 &velocity)
		{
			this->velocity = velocity;
			this->location = location;
			this->quality = quality;
			modelGroup = model;
		}

		void setModel(std::shared_ptr<utility::ModelGroup> model)
		{
			modelGroup = model;
		}

		void setLocation(float x, float y, float z)
		{
			location.x = x;
			location.y = y;
			location.z = z;
		}

		void setVelocity(float x, float y, float z)
		{
			velocity.x = x;
			velocity.y = y;
			velocity.z = z;
		}

		std::shared_ptr<utility::ModelGroup> modelGroup;
		glm::vec3 location; // 位置
		glm::vec3 velocity; // 速度
		float quality; // 质量

	};


	class Universe : public utility::ModelGroup
	{
	public:
		Universe(const std::vector<std::shared_ptr<Planet>> &planets, std::shared_ptr<modeling::Shader> &shader)
			: planets(planets), utility::ModelGroup()
		{
			xt::xarray<float> entrance = xt::zeros<float>({ (int)planets.size() * 6 });
			xt::xarray<float> quality = xt::zeros<float>({ (int)planets.size() });

			size_t i = 0, j = 0;
			while (j < entrance.size())
			{
				entrance[j++] = planets[i]->location.x;
				entrance[j++] = planets[i]->location.y;
				entrance[j++] = planets[i]->location.z;

				entrance[j++] = planets[i]->velocity.x;
				entrance[j++] = planets[i]->velocity.y;
				entrance[j++] = planets[i]->velocity.z;

				quality[i] = planets[i]->quality;
				i++;
			}

			gen = math::RungeKuttaGenerator<float>(
				entrance, 0.0, 1.0, math::gravity<float>,
				std::vector<xt::xarray<float>>{ quality });

			tracks = std::vector<std::shared_ptr<utility::TrackGroup>>(
				planets.size(),
				std::shared_ptr<utility::TrackGroup>(new utility::TrackGroup(shader)));
		}

		void render() override
		{
			xt::xarray<float> output = gen.next();
			//std::cout << output << std::endl;
			for (size_t i = 0; i < planets.size(); i++)
			{
				getElementData(output, i);

				planets[i]->modelGroup->render([&](const glm::mat4 &model)
				{
					glm::mat4 r = glm::translate(model, glm::vec3(planets[i]->location.x,
						planets[i]->location.y,
						planets[i]->location.z));
					return r;
				});

				tracks[i]->addVertex(planets[i]->location.x,
					planets[i]->location.y,
					planets[i]->location.z);

				tracks[i]->render();
			}
		}

		const std::vector<std::shared_ptr<utility::TrackGroup>> &getTracks()
		{
			return tracks;
		}
	private:
		std::vector<std::shared_ptr<Planet>> planets;
		std::vector<std::shared_ptr<utility::TrackGroup>> tracks;
		math::RungeKuttaGenerator<float> gen;

		void getElementData(const xt::xarray<float> &arr, size_t &i)
		{
			planets[i]->location.x = arr[i * 6];
			planets[i]->location.y = arr[i * 6 + 1];
			planets[i]->location.z = arr[i * 6 + 2];

			planets[i]->velocity.x = arr[i * 6 + 3];
			planets[i]->velocity.y = arr[i * 6 + 4];
			planets[i]->velocity.z = arr[i * 6 + 5];
		}
	};

}
