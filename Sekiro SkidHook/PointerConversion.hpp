#pragma once

#include "pch.h"

namespace SkidBase::Memory
{

	class Ptr
	{

	public:

		Ptr(std::uintptr_t Pointer) : m_Ptr(Pointer) 
		{}

		Ptr Add(unsigned int Value)
		{
			return Ptr(m_Ptr + Value);
		}
		Ptr Sub(unsigned int Value)
		{
			return Ptr(m_Ptr - Value);
		}

		template <typename T>
		T As()
		{
			return reinterpret_cast<T>(m_Ptr);
		}

		Ptr Rip()
		{
			return Ptr(*As<std::int32_t*>()).Add(4U);
		}

		std::uintptr_t Get()
		{
			return m_Ptr;
		}

	private:

		std::uintptr_t m_Ptr;

	};


}