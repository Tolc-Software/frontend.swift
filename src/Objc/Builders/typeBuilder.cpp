#include "Objc/Builders/typeBuilder.hpp"
#include "Objc/Conversions/base.hpp"
#include "Objc/Conversions/container.hpp"
#include "Objc/Conversions/conversion.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/Conversions/userDefined.hpp"
#include "Objc/cache.hpp"
#include "Objc/getName.hpp"
#include <IR/ir.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <variant>

namespace Objc::Builders {

std::string
getNullable(IR::BaseType base, bool hasPointers, Objc::Proxy::Type& type) {
	using IR::BaseType;
	switch (base) {
		case BaseType::String:
		case BaseType::StringView:
		case BaseType::FilesystemPath: {
			if (hasPointers) {
				type.m_isNullable = true;
				return "nullable ";
			}
			type.m_isNullable = false;
			return "nonnull ";
		}
		case BaseType::Bool:
		case BaseType::Char16_t:
		case BaseType::Char32_t:
		case BaseType::Char:
		case BaseType::Complex:
		case BaseType::Double:
		case BaseType::Float:
		case BaseType::Int:
		case BaseType::LongDouble:
		case BaseType::LongInt:
		case BaseType::LongLongInt:
		case BaseType::ShortInt:
		case BaseType::SignedChar:
		case BaseType::UnsignedChar:
		case BaseType::UnsignedInt:
		case BaseType::UnsignedLongInt:
		case BaseType::UnsignedLongLongInt:
		case BaseType::UnsignedShortInt:
		case BaseType::Void:
		case BaseType::Wchar_t: {
			return "";
		}
	}
}
std::string getNullable(IR::ContainerType container,
                        bool hasPointers,
                        Objc::Proxy::Type& type) {
	using IR::ContainerType;
	switch (container) {
		case ContainerType::Array:
		case ContainerType::Deque:
		case ContainerType::List:
		case ContainerType::Pair:
		case ContainerType::Tuple:
		case ContainerType::Valarray:
		case ContainerType::Vector:
		case ContainerType::Map:
		case ContainerType::UnorderedMap:
		case ContainerType::Set:
		case ContainerType::UnorderedSet: {
			if (hasPointers) {
				type.m_isNullable = true;
				return "nullable ";
			}
			type.m_isNullable = false;
			return "nonnull ";
		}
		case ContainerType::Optional:
		case ContainerType::SharedPtr:
		case ContainerType::UniquePtr: {
			type.m_isNullable = true;
			return "nullable ";
		}
		case ContainerType::UnorderedMultiSet:
		case ContainerType::Queue:
		case ContainerType::MultiMap:
		case ContainerType::MultiSet:
		case ContainerType::PriorityQueue:
		case ContainerType::Stack:
		case ContainerType::UnorderedMultiMap:
		case ContainerType::Variant:
		case ContainerType::Allocator:
		case ContainerType::EqualTo:
		case ContainerType::ForwardList:
		case ContainerType::Greater:
		case ContainerType::Hash:
		case ContainerType::Less: {
			return "";
		}
	}
}

Objc::Proxy::Type buildType(IR::Type const& type, Objc::Cache& cache) {
	Objc::Proxy::Type t;
	t.m_cppType = &type;
	if (auto baseType = std::get_if<IR::Type::Value>(&type.m_type)) {
		auto name = getNullable(baseType->m_base, type.m_numPointers != 0, t) +
		            Objc::getBaseName(baseType->m_base);
		t.m_name = [name]() {
			return name;
		};
		t.m_conversions =
		    Objc::Conversions::getBaseTypeConversions(baseType->m_base, cache);
	} else if (auto enumType = std::get_if<IR::Type::EnumValue>(&type.m_type)) {
		t.m_isNullable = type.m_numPointers != 0;
		auto name =
		    Objc::getEnumName(enumType->m_representation, cache.m_moduleName);
		t.m_name = [name]() {
			return name;
		};
		t.m_conversions = Objc::Conversions::getEnumConversionNames(
		    cache.m_moduleName,
		    enumType->m_representation,
		    cache.m_extraFunctionsNamespace);
	} else if (auto container =
	               std::get_if<IR::Type::Container>(&type.m_type)) {
		auto name =
		    getNullable(container->m_container, type.m_numPointers != 0, t) +
		    Objc::getContainerName(*container, cache.m_moduleName);
		t.m_name = [name]() {
			return name;
		};
		t.m_conversions = Objc::Conversions::getContainerTypeConversions(
		    type, *container, cache);
	} else if (auto userDefined =
	               std::get_if<IR::Type::UserDefined>(&type.m_type)) {
		t.m_isNullable = type.m_numPointers != 0;
		t.m_name = [nullable = t.m_isNullable,
		            cppClass = userDefined->m_representation,
		            &cache]() {
			if (auto objcClass = cache.m_cppToObjcClassNames.find(cppClass);
			    objcClass != cache.m_cppToObjcClassNames.end()) {
				return nullable ? "nullable " + objcClass->second + '*' :
                                  "nonnull " + objcClass->second + '*';
			}
			spdlog::error(
			    "Couldn't find the Objective-C class name of C++ class {}",
			    cppClass);
			return std::string();
		};
		t.m_conversions = Objc::Conversions::getUserDefinedConversionNames(
		    userDefined->m_representation, cache.m_extraFunctionsNamespace);
	}

	t.m_dereference = "";
	for (int i = 0; i < type.m_numPointers; ++i) {
		t.m_dereference.push_back('*');
	}
	return t;
}

}    // namespace Objc::Builders
