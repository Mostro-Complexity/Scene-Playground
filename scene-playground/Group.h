#pragma once
#include "stdafx.h"

enum CameraGroupType
{
	roamCamera, observeCamera
};

class Group
{
	virtual void init() abstract;
	virtual void render() abstract;

};

