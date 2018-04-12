#pragma once
#include "stdafx.h"


namespace mostro
{
	namespace simulation
	{
		enum CameraGroupType
		{
			roamCamera, observeCamera
		};

		class Group
		{
		public:

			virtual void init() abstract;
			virtual void render() abstract;

		};
	}
}
