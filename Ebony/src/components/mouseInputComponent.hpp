#pragma once

#include <functional>

#include "component.hpp"
#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include <fstream>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include "../Ebony/src/singletons/inputManager.hpp"

namespace components
{
	class MouseInput : public PolymorphicComparable<Component, MouseInput>
	{
	public:
		MouseInput() {};

		float positionX = 0;
		float positionY = 0;
		float previousX = 0;
		float previousY = 0;



		std::unordered_map<int, std::string> bindings{}; // This is what defines our key bindings. i.e. pressing space gives the string_view "jump"
		std::unordered_map<std::string_view, std::function<void([[maybe_unused]] entities::EntityPtr& entity, Ebony::MousePress& mousePress)>> onPressActions{}; // This defines our on initial press actions. i.e. "jump" causes the player to... jump...
		std::unordered_map<std::string_view, std::function<void([[maybe_unused]] entities::EntityPtr& entity, Ebony::MousePress& mousePress)>> onHeldActions{}; // This defines our on held actions. i.e. holding w keeps going forward
		std::unordered_map<std::string_view, std::function<void([[maybe_unused]] entities::EntityPtr& entity, Ebony::MousePress& mousePress)>> onReleaseActions{}; // This defines our on released actions. i.e. releasing at the correct time to time something


		void saveMouseBindings(std::string_view filepathBindings)
		{
			
			rapidjson::Document saveDocument{};
			rapidjson::Document::AllocatorType& allocator = saveDocument.GetAllocator();
			saveDocument.SetObject();


			for (auto iter = this->bindings.begin(); iter != this->bindings.end(); iter++)
			{
				std::string intAsString = std::to_string(iter->first);
				rapidjson::Value index(intAsString.c_str(), static_cast<rapidjson::SizeType>(intAsString.size()), allocator);

				// Luckily, this save method won't happen often, so even though it is terrible to fully copy a string_view, it will be okay
				rapidjson::Value v(std::string(iter->second), allocator);
				saveDocument.AddMember(index, v, allocator);
			}

			// https://rapidjson.org/md_doc_stream.html
			std::ofstream ofs(filepathBindings.data());
			rapidjson::OStreamWrapper osw(ofs);

			rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
			saveDocument.Accept(writer);
		}


		void loadMouseBindings(std::string_view filepathBindings)
		{	
			rapidjson::Document document;
			std::ifstream ifs(filepathBindings.data());
			rapidjson::IStreamWrapper isw(ifs);

			document.ParseStream(isw);

			// We now have the bindings, so it is our job to load them in.
			// int -> string
			for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); itr++)
			{
				this->bindings.insert({ std::stoi(itr->name.GetString()), itr->value.GetString() });
			}
		}

	};
}