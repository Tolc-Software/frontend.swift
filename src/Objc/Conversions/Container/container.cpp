#include "Objc/Conversions/Container/container.hpp"
#include "Objc/Conversions/conversion.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/Conversions/utility.hpp"
#include "Objc/cache.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <queue>
#include <string>
#include <vector>

namespace Objc::Conversions::Container {

std::string addArrayValue(IR::ContainerType arrayType,
                          std::string const& conversionFunction) {
	using IR::ContainerType;
	switch (arrayType) {
		case ContainerType::Vector: {
			return fmt::format("cppArray.push_back({}([v objectAtIndex:i]));",
			                   conversionFunction);
		}
		case ContainerType::Array: {
			return fmt::format("objcArray[i] = {}([v objectAtIndex:i]);",
			                   conversionFunction);
		}
		default: break;
	}
	return "";
}

Objc::Conversions::Conversion
convertArrayType(IR::Type const& type,
                 IR::ContainerType containerType,
                 std::vector<IR::Type> const& containedTypes,
                 std::vector<std::string>& functions,
                 Objc::Cache& cache) {
	auto names = Objc::Conversions::getConversionContainerName(type);
	if (!cache.m_conversions.contains(names.m_toCpp)) {
		cache.m_conversions.insert(names.m_toCpp);
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
  {noQualifiers} cppArray;
  for (size_t i = 0; i < [v count]; i++) {{
    {addArrayValue}
  }}
  return cppArray;
}})",
			    fmt::arg("noQualifiers", noQualifiers),
			    fmt::arg("toCppName", names.m_toCpp),
			    fmt::arg("typeName", type.m_representation),
			    fmt::arg("toCppName", names.m_toCpp),
			    fmt::arg("addArrayValue",
			             addArrayValue(containerType,
			                           containedConversions.m_toCpp))));

			functions.push_back(fmt::format(
			    R"(
NSArray* {toObjcName}({noQualifiers} const& v) {{
  NSMutableArray* objcArray = [NSMutableArray arrayWithCapacity:v.size()];
  for (size_t i = 0; i < v.size(); i++) {{
    [objcArray addObject: {convertToObjc}(v[i])];
  }}
  return objcArray;
}})",
			    fmt::arg("noQualifiers", noQualifiers),
			    fmt::arg("toObjcName", names.m_toObjc),
			    fmt::arg("typeName", type.m_representation),
			    fmt::arg("convertToObjc", containedConversions.m_toObjc)));
		}
	}
	return addNamespace(names, cache.m_extraFunctionsNamespace);
}

Objc::Conversions::Conversion
convertMapType(IR::Type const& type,
               std::vector<IR::Type> const& containedTypes,
               std::vector<std::string>& functions,
               Objc::Cache& cache) {
	auto names = Objc::Conversions::getConversionContainerName(type);
	if (!cache.m_conversions.contains(names.m_toCpp)) {
		cache.m_conversions.insert(names.m_toCpp);
		auto noQualifiers = ObjcSwift::Helpers::removeQualifiers(type);
		// map should have two contained value (plus allocators)
		// TODO: Handle error
		if (containedTypes.size() >= 2) {
			auto keyConversion = Objc::Conversions::getConversionContainerName(
			    containedTypes[0]);
			auto valueConversion =
			    Objc::Conversions::getConversionContainerName(
			        containedTypes[1]);

			functions.push_back(fmt::format(
			    R"(
{noQualifiers} {toCppName}(NSDictionary* m) {{
  {noQualifiers} cppMap;
  for (id key in m) {{
    id value = [m objectForKey:key];
    cppMap[{keyConversion}(key)] = {valueConversion}(value);
  }}
  return cppMap;
}})",
			    fmt::arg("noQualifiers", noQualifiers),
			    fmt::arg("toCppName", names.m_toCpp),
			    fmt::arg("typeName", type.m_representation),
			    fmt::arg("toCppName", names.m_toCpp),
			    fmt::arg("keyConversion", keyConversion.m_toCpp),
			    fmt::arg("valueConversion", valueConversion.m_toCpp)));

			functions.push_back(fmt::format(
			    R"(
NSDictionary* {toObjcName}({noQualifiers} const& m) {{
  NSMutableDictionary* objcMap = [NSMutableDictionary dictionaryWithCapacity:m.size()];
  for (auto const& keyValue : m) {{
    objcMap[{keyConversion}(keyValue.first)] = {valueConversion}(keyValue.second);
  }}
  return objcMap;
}})",
			    fmt::arg("noQualifiers", noQualifiers),
			    fmt::arg("toObjcName", names.m_toObjc),
			    fmt::arg("typeName", type.m_representation),
			    fmt::arg("keyConversion", keyConversion.m_toObjc),
			    fmt::arg("valueConversion", valueConversion.m_toObjc)));
		}
	}
	return addNamespace(names, cache.m_extraFunctionsNamespace);
}

Objc::Conversions::Conversion
convertSetType(IR::Type const& type,
               std::vector<IR::Type> const& containedTypes,
               std::vector<std::string>& functions,
               Objc::Cache& cache,
               bool isOrdered = true) {
	auto names = Objc::Conversions::getConversionContainerName(type);
	if (!cache.m_conversions.contains(names.m_toCpp)) {
		cache.m_conversions.insert(names.m_toCpp);
		auto noQualifiers = ObjcSwift::Helpers::removeQualifiers(type);
		// map should have two contained value (plus allocators)
		// TODO: Handle error
		if (!containedTypes.empty()) {
			auto valueConversion =
			    Objc::Conversions::getConversionContainerName(
			        containedTypes[0]);

			functions.push_back(fmt::format(
			    R"(
{noQualifiers} {toCppName}(NS{ordered}Set* s) {{
  {noQualifiers} cppSet;
  for (id value in s) {{
    cppSet.insert({valueConversion}(value));
  }}
  return cppSet;
}})",
			    fmt::arg("noQualifiers", noQualifiers),
			    fmt::arg("toCppName", names.m_toCpp),
			    fmt::arg("ordered", isOrdered ? "Ordered" : ""),
			    fmt::arg("typeName", type.m_representation),
			    fmt::arg("toCppName", names.m_toCpp),
			    fmt::arg("valueConversion", valueConversion.m_toCpp)));

			functions.push_back(fmt::format(
			    R"(
NS{ordered}Set* {toObjcName}({noQualifiers} const& s) {{
  NSMutable{ordered}Set* objcSet = [NSMutable{ordered}Set {initOrdered}WithCapacity:s.size()];
  for (auto const& value : s) {{
    [objcSet addObject:{valueConversion}(value)];
  }}
  return objcSet;
}})",
			    fmt::arg("noQualifiers", noQualifiers),
			    fmt::arg("toObjcName", names.m_toObjc),
			    fmt::arg("ordered", isOrdered ? "Ordered" : ""),
			    fmt::arg("initOrdered", isOrdered ? "orderedSet" : "set"),
			    fmt::arg("typeName", type.m_representation),
			    fmt::arg("valueConversion", valueConversion.m_toObjc)));
		}
	}
	return addNamespace(names, cache.m_extraFunctionsNamespace);
}

Objc::Conversions::Conversion
containerConversion(IR::Type const& type,
                    IR::Type::Container const& container,
                    std::vector<std::string>& functions,
                    std::queue<IR::Type const*>& typesToConvert,
                    Objc::Cache& cache) {
	using IR::ContainerType;
	switch (container.m_container) {
		case ContainerType::Vector: {
			if (!container.m_containedTypes.empty()) {
				// The first one is the Stuff in vector<Stuff>
				typesToConvert.push(&container.m_containedTypes.front());
			}    // TODO: Handle error
			return convertArrayType(type,
			                        container.m_container,
			                        container.m_containedTypes,
			                        functions,
			                        cache);
			break;
		}
		case ContainerType::Array: {
			if (!container.m_containedTypes.empty()) {
				// The first one is the Stuff in array<Stuff>
				typesToConvert.push(&container.m_containedTypes.front());
			}    // TODO: Handle error
			return convertArrayType(type,
			                        container.m_container,
			                        container.m_containedTypes,
			                        functions,
			                        cache);
			break;
		}
		case ContainerType::UnorderedMap:
		case ContainerType::Map: {
			if (container.m_containedTypes.size() >= 2) {
				// The first one is the Stuff, Other in map<Stuff, Other>
				typesToConvert.push(&container.m_containedTypes[0]);
				typesToConvert.push(&container.m_containedTypes[1]);
			}    // TODO: Handle error
			return convertMapType(type,
			                      container.m_containedTypes,
			                      functions,
			                      cache);
			break;
		}
		case ContainerType::UnorderedSet:
		case ContainerType::Set: {
			if (!container.m_containedTypes.empty()) {
				// The first one is the Stuff, Other in map<Stuff, Other>
				typesToConvert.push(&container.m_containedTypes[0]);
			}    // TODO: Handle error
			return convertSetType(type,
			                      container.m_containedTypes,
			                      functions,
			                      cache,
			                      container.m_container == ContainerType::Set);
			break;
		}
		case ContainerType::Deque:
		case ContainerType::List:
		case ContainerType::MultiMap:
		case ContainerType::MultiSet:
		case ContainerType::Optional:
		case ContainerType::Pair:
		case ContainerType::PriorityQueue:
		case ContainerType::Queue:
		case ContainerType::SharedPtr:
		case ContainerType::Stack:
		case ContainerType::Tuple:
		case ContainerType::UniquePtr:
		case ContainerType::UnorderedMultiMap:
		case ContainerType::UnorderedMultiSet:
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

}    // namespace Objc::Conversions::Container
