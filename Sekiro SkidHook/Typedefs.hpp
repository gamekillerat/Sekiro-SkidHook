#pragma once

#include "pch.h"

namespace SkidBase
{

	struct Vector3
	{

		float x, y, z;

		explicit Vector3(float X, float Y, float Z) :
			x(X), y(Y), z(Z)
		{}

		Vector3() :
			x(0.f), y(0.f), z(0.f)
		{}


		void operator= (Vector3 b)
		{
			x = b.x;
			y = b.y;
			z = b.z;
		}
		void operator= (float b)
		{
			x = b;
			y = b;
			z = b;
		}

	};

	struct Vector4
	{

		float x, y, z, w;

		explicit Vector4(float X, float Y, float Z, float W) :
			x(X), y(Y), z(Z), w(W)
		{}

		Vector4() :
			x(0.f), y(0.f), z(0.f), w(0.f)
		{}


		void operator= (Vector4 b)
		{
			x = b.x;
			y = b.y;
			z = b.z;
			w = b.w;
		}
		void operator= (float b)
		{
			x = b;
			y = b;
			z = b;
			w = b;
		}

	};



}