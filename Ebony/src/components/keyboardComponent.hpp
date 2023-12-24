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
#include "../entity.hpp"

namespace components
{
	class KeyboardInput : public PolymorphicComparable<Component, KeyboardInput>
	{
	public:
		KeyboardInput() {};

		std::unordered_map<int, std::string> bindings{}; // This is what defines our key bindings. i.e. pressing space gives the string_view "jump"
		std::unordered_map<std::string_view, std::function<void()>> onPressActions{}; // This defines our on initial press actions. i.e. "jump" causes the player to... jump...
		std::unordered_map<std::string_view, std::function<void()>> onHeldActions{}; // This defines our on held actions. i.e. holding w keeps going forward
		std::unordered_map<std::string_view, std::function<void()>> onReleaseActions{}; // This defines our on released actions. i.e. releasing at the correct time to time something
	


		// Breaking ECS a bit, but this is fine :)	
	
		void saveKeyBindings(std::string_view filepath)
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
			std::ofstream ofs(filepath.data());
			rapidjson::OStreamWrapper osw(ofs);

			rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
			saveDocument.Accept(writer);


			// Iterate through the bindings first
			// iter->first = Key
			// iter->second = functionString
			// Json Structure:
			// bindings: {
			//	key: string,
			//	key: string,
			// },


			// TODO: THIS MAY NEED TO BE ADDING MEMBERS TO THE OBJECT AND NOT THE VALUE

			// TODO: FOR SUBARRAYS. I DON'T THINK I NEED MORE THAN JUST THE BINDINGS.
			//rapidjson::Value bindingNestedObject(rapidjson::kObjectType);
			//document.AddMember("bindings", bindingNestedObject, allocator);
			//rapidjson::Value& bindingNestedObjectValue = document["bindings"];

			//for (auto iter = this->bindings.begin(); iter != this->bindings.end(); iter++)
			//{
			//	std::string intAsString = std::to_string(iter->first);
			//	rapidjson::Value index(intAsString.c_str(), intAsString.size(), allocator);

			//	// Luckily, this save method won't happen often, so even though it is terrible to fully copy a string_view, it will be okay
			//	rapidjson::Value v(std::string(iter->second), allocator);
			//	bindingNestedObjectValue.AddMember(index, v, allocator);
			//}


			//rapidjson::Value bindingNestedObject(rapidjson::kObjectType);
			//document.AddMember("bindings", bindingNestedObject, allocator);
			//rapidjson::Value& bindingNestedObjectValue = document["bindings"];




			//rapidjson::Value onPressNestedObject(rapidjson::kObjectType);
			//document.AddMember("onPress", onPressNestedObject, allocator);
			//rapidjson::Value& onPressNestedObjectValue = document["onPress"];
			//// Iterate through the onPressActions
			//for (auto iter = this->onPressActions.begin(); iter != this->onPressActions.end(); iter++)
			//{
			//	rapidjson::Value index(std::string(iter->first), allocator);

			//	// Luckily, this save method won't happen often, so even though it is terrible to fully copy a string_view, it will be okay
			//	rapidjson::Value v(std::string(iter->second), allocator);
			//	bindingNestedObjectValue.AddMember(index, v, allocator);

			//	onPressNestedObjectValue.AddMember(rapidjson::StringRef((char*)(iter->first.data())), iter->second, allocator);
			//}

			////// Iterate through the onReleaseActions
			//rapidjson::Value onReleaseNestedObject(rapidjson::kObjectType);
			//document.AddMember("onRelease", onReleaseNestedObject, allocator);
			//rapidjson::Value& onReleaseNestedObjectValue = document["onRelease"];

			////for (auto iter = this->onReleaseActions.begin(); iter != this->onReleaseActions.end(); iter++)
			////{
			////	onReleaseNestedObjectValue.AddMember(rapidjson::StringRef((char*)(iter->first.data())), iter->second, allocator);
			////}

			////// Iterate through the onHeldActions
			//rapidjson::Value onHeldNestedObject(rapidjson::kObjectType);
			//document.AddMember("onHeld", onHeldNestedObject, allocator);
			//rapidjson::Value& onHeldNestedObjectValue = document["onHeld"];

			//for (auto iter = this->onHeldActions.begin(); iter != this->onHeldActions.end(); iter++)
			//{
			//	onHeldNestedObjectValue.AddMember(rapidjson::StringRef((char*)(iter->first.data())), iter->second, allocator);
			//}
		}

		void loadKeyBindings(std::string_view filepath)
		{
			rapidjson::Document document;
			std::ifstream ifs(filepath.data());
			rapidjson::IStreamWrapper isw(ifs);

			document.ParseStream(isw);

			// We now have the bindings, so it is our job to load them in.
			// int -> string
			for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); itr++)
			{
				this->bindings.insert({ std::stoi(itr->name.GetString()), itr->value.GetString()});
			}
		}
		


	};
}