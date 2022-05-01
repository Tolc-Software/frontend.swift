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
			return fmt::format("array.push_back({}([v objectAtIndex:i]);",
			                   conversionFunction);
		}
		case ContainerType::Array: {
			return fmt::format("array[i] = {}([v objectAtIndex:i]);",
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
  for (size_t i = 0; i < [v count]; i++) {{
    {addArrayValue}
  }}
  return array;
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
  NSMutableArray* array = [NSMutableArray arrayWithCapacity:v.size()];
  for (size_t i = 0; i < v.size(); i++) {{
    [array addObject: {convertToObjc}(v[i])];
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
				typesToConvert.push(&container.m_containedTypes.front());
			}    // TODO: Handle error
			return convertArrayType(type,
			                        container.m_container,
			                        container.m_containedTypes,
			                        functions,
			                        cache);
			break;
		}
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

}    // namespace Objc::Conversions::Container
