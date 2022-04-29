#include "Objc/Conversions/container.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/Conversions/utility.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <array>
#include <fmt/format.h>
#include <string>
#include <vector>

namespace Objc::Conversions {

namespace {

Conversion convertVector(IR::Type const& type, Objc::Cache& cache) {
	auto names = Objc::Conversions::getConversionContainerName(type);
	if (!cache.m_baseConversions.m_toCpp.contains(names.m_toCpp)) {
		cache.m_baseConversions.m_toCpp.insert(names.m_toCpp);

		cache.m_extraFunctions.push_back(fmt::format(
		    R"(
{typeName} {toCppName}(NSArray* v) {{
  {typeName} array
  return array;
}})",

		    fmt::arg("typeName", type.m_representation),
		    fmt::arg("toCppName", names.m_toCpp),
		    fmt::arg("typeName", type.m_representation),
		    fmt::arg("toCppName", names.m_toCpp)));

		cache.m_extraFunctions.push_back(fmt::format(
		    R"(
NSArray* {toObjcName}({typeName} v) {{
  NSArray* array = [NSArray arrayWithCapacity:v.size()];
  for (auto const& value : v) {{
    [array addObject: value];
  }}
  return array;
}})",
		    fmt::arg("typeName", type.m_representation),
		    fmt::arg("toObjcName", names.m_toObjc),
		    fmt::arg("typeName", type.m_representation)));
	}
	return addNamespace(names, cache.m_extraFunctionsNamespace);
}

}    // namespace

Conversion getContainerTypeConversions(IR::Type const& type,
                                       Objc::Cache& cache) {
	Conversion c;
	c.m_toCpp = "";
	c.m_toObjc = "";
	if (auto container = std::get_if<IR::Type::Container>(&type.m_type)) {
		using IR::ContainerType;
		switch (container->m_container) {
			case ContainerType::Vector: return convertVector(type, cache);
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
			case ContainerType::Less: return c;
		}
	}
	return c;
}
}    // namespace Objc::Conversions

