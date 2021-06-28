#pragma once

#include "pch.h"

namespace SkidBase::Hooking
{

	class VMT
	{
	public:

		explicit VMT() {}

		VMT(void* obj, std::size_t num_funcs, std::string m_name) :
			m_object(static_cast<void***>(obj)),
			m_num_funcs(num_funcs + 1),
			m_original_table(*m_object),
			m_new_table(std::make_unique<void*[]>(m_num_funcs)),
			name(m_name)
		{
			std::copy_n(m_original_table - 1, m_num_funcs, m_new_table.get());
		}

		void hook(std::size_t index, void* func) 
		{ 
			m_new_table[index + 1] = func; 
			g_Logger->LogHook("{} -> hooked at {}", name, index); 
		}
		void unhook(std::size_t index) 
		{
			m_new_table[index + 1] = m_original_table[index]; 
			g_Logger->LogHook("{} -> unhooked at {}", name, index);
		}

		template <typename T>
		T get_original(std::size_t index) 
		{
			return static_cast<T>(m_original_table[index]);
		}

		void enable()
		{
			*m_object = m_new_table.get() + 1;
		}
		void disable() 
		{ 
			*m_object = m_original_table;
		}

	private:

		void*** m_object;
		std::size_t m_num_funcs;

		void** m_original_table;
		std::unique_ptr<void*[]> m_new_table;

		std::string name;
	};


}