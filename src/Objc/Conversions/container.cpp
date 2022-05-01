#include "Objc/Conversions/container.hpp"
#include "Objc/Conversions/Container/value.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/Conversions/utility.hpp"
#include "Objc/cache.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include <IR/ir.hpp>
#include <array>
#include <cctype>
#include <fmt/format.h>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

namespace Objc::Conversions {

namespace {

Conversion convertVector(IR::Type const& type,
                         std::vector<IR::Type> const& containedTypes,
                         std::vector<std::string>& functions,
                         Objc::Cache& cache) {
	auto names = Objc::Conversions::getConversionContainerName(type);
	if (!cache.m_baseConversions.m_toCpp.contains(names.m_toCpp)) {
		cache.m_baseConversions.m_toCpp.insert(names.m_toCpp);
		auto noQualifiers = ObjcSwift::Helpers::removeQualifiers(type);
		// Vector should have one contained value (plus allocators)
		// TODO: Handle error
		if (!containedTypes.empty()) {
			auto containedConversions =
			    Objc::Conversions::getConversionContainerName(
			        containedTypes.front());

			functions.push_back(fmt::format(
			    R"(
{noQualifiers} {toCppName}(NSArray* v) {{
  {noQualifiers} array;
  for (id value in v) {{
    array.push_back({convertToCpp}(value));
  }}
  return array;
}})",
			    fmt::arg("noQualifiers", noQualifiers),
			    fmt::arg("toCppName", names.m_toCpp),
			    fmt::arg("typeName", type.m_representation),
			    fmt::arg("toCppName", names.m_toCpp),
			    fmt::arg("convertToCpp", containedConversions.m_toCpp)));

			functions.push_back(fmt::format(
			    R"(
NSArray* {toObjcName}({noQualifiers} const& v) {{
  NSMutableArray* array = [NSMutableArray arrayWithCapacity:v.size()];
  for (auto const& value : v) {{
    [array addObject: {convertToObjc}(value)];
  }}
  return array;
}})",
			    fmt::arg("noQualifiers", noQualifiers),
			    fmt::arg("toObjcName", names.m_toObjc),
			    fmt::arg("typeName", type.m_representation),
			    fmt::arg("convertToObjc", containedConversions.m_toObjc)));
		}
	}
	return addNamespace(names, cache.m_extraFunctionsNamespace);
}

Conversion containerConversion(IR::Type const& type,
                               IR::Type::Container const& container,
                               std::vector<std::string>& functions,
                               std::queue<IR::Type const*>& typesToConvert,
                               Objc::Cache& cache) {
	using IR::ContainerType;
	switch (container.m_container) {
		case ContainerType::Vector: {
			if (!container.m_containedTypes.empty()) {
				typesToConvert.push(&container.m_containedTypes.front());
			}    // TODO: Handle error
			return convertVector(
			    type, container.m_containedTypes, functions, cache);
			break;
		}
		case ContainerType::Array:
		case ContainerType::Deque:
		case ContainerType::List:
		case ContainerType::Map:
		case ContainerType::MultiMap:
		case ContainerType::MultiSet:
		case ContainerType::Optional:
		case ContainerType::Pair:
		case ContainerType::PriorityQueue:
		case ContainerType::Queue:
		case ContainerType::Set:
		case ContainerType::SharedPtr:
		case ContainerType::Stack:
		case ContainerType::Tuple:
		case ContainerType::UniquePtr:
		case ContainerType::UnorderedMap:
		case ContainerType::UnorderedMultiMap:
		case ContainerType::UnorderedMultiSet:
		case ContainerType::UnorderedSet:
		case ContainerType::Valarray:
		case ContainerType::Variant:
		case ContainerType::Allocator:
		case ContainerType::EqualTo:
		case ContainerType::ForwardList:
		case ContainerType::Greater:
		case ContainerType::Hash:
		case ContainerType::Less: return {};
	}
	return {};
}

}    // namespace

Conversion getContainerTypeConversions(IR::Type const& type,
                                       IR::Type::Container const& container,
                                       Objc::Cache& cache) {
	// Types that need conversion
	std::queue<IR::Type const*> typesToConvert;
	// Conversion functions that has been defined so far
	// Since vector<int> will first convert vector, then int
	// We need to lay these out in the file in the reverse order
	// so that convertVector can call convertInt
	std::vector<std::string> extraFunctions;

	auto rootConversion = containerConversion(
	    type, container, extraFunctions, typesToConvert, cache);

	std::cout << type.m_representation << '\n';
	std::cout << "Root conversions are:" << '\n';
	std::cout << rootConversion.m_toCpp << '\n';
	std::cout << rootConversion.m_toObjc << '\n';

	while (!typesToConvert.empty()) {
		auto* currentType = typesToConvert.front();
		if (auto currentContainer =
		        std::get_if<IR::Type::Container>(&currentType->m_type)) {
			containerConversion(*currentType,
			                    *currentContainer,
			                    extraFunctions,
			                    typesToConvert,
			                    cache);
		} else if (auto value =
		               std::get_if<IR::Type::Value>(&currentType->m_type)) {
			Container::valueConversion(
			    *currentType, value->m_base, extraFunctions, cache);
		}
		typesToConvert.pop();
	}

	cache.m_extraFunctions.insert(cache.m_extraFunctions.end(),
	                              extraFunctions.rbegin(),
	                              extraFunctions.rend());
	return rootConversion;
}
}    // namespace Objc::Conversions

