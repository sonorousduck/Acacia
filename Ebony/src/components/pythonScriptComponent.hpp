#pragma once

#include "component.hpp"
#include "../entity.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <filesystem>

namespace components
{
	// https://stackoverflow.com/questions/16545297/c-python-import-class-call-methods
	class PythonScript : public PolymorphicComparable<Component, PythonScript>
	{
	public:
		PythonScript(const char* pythonFile)
		{

			filename = "pythonScripts.";
			filename += pythonFile;


			/*if (sys)
			{
				Start = sys.attr("Start");
				Reset = sys.attr("Reset");
				Update = sys.attr("Update");
				OnDestroy = sys.attr("OnDestroy");
				OnCollision = sys.attr("OnCollision");
				OnCollisionEnd = sys.attr("OnCollisionEnd");
				OnCollisionStart = sys.attr("OnCollisionStart");
				OnMouseDown = sys.attr("OnMouseDown");
				OnMouseUp = sys.attr("OnMouseUp");
				OnMouseEnter = sys.attr("OnMouseEnter");
				OnMouseOver = sys.attr("OnMouseOver");
			}*/

			



			//pyName = PyUnicode_FromString(pythonFolder.c_str());
			//pyModule = PyImport_Import(pyName);
			//pyModule = PyImport_ImportModule(pythonFolder.c_str());



			//if (pyModule)
			//{
			//	// Check for each type of function to see if they are existant so we can call them when appropriate
			//	
			//}
		}

		std::string filename;

		/*Ebony::CPyObject pyName;
		Ebony::CPyObject pyModule; */

		pybind11::module_ pyModule;
		/*pybind11::object Start;
		pybind11::object Reset;
		pybind11::object Update;
		pybind11::object OnDestroy;
		pybind11::object OnCollision;
		pybind11::object OnCollisionEnd;
		pybind11::object OnCollisionStart;
		pybind11::object OnMouseDown;
		pybind11::object OnMouseUp;
		pybind11::object OnMouseEnter;
		pybind11::object OnMouseOver;*/




	};
}