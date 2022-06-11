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

Objc::Proxy::Type buildType(IR::Type const& type, Objc::Cache& cache) {
	Objc::Proxy::Type t;
	if (auto baseType = std::get_if<IR::Type::Value>(&type.m_type)) {
		auto name = Objc::getBaseName(baseType->m_base);
		t.m_name = [name]() {
			return name;
		};
		t.m_conversions =
		    Objc::Conversions::getBaseTypeConversions(baseType->m_base, cache);
	} else if (auto enumType = std::get_if<IR::Type::EnumValue>(&type.m_type)) {
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
		auto name = Objc::getContainerName(*container, cache.m_moduleName);
		t.m_name = [name]() {
			return name;
		};
		t.m_conversions = Objc::Conversions::getContainerTypeConversions(
		    type, *container, cache);
	} else if (auto userDefined =
	               std::get_if<IR::Type::UserDefined>(&type.m_type)) {
		t.m_name = [cppClass = userDefined->m_representation, &cache]() {
			if (auto objcClass = cache.m_cppToObjcClassNames.find(cppClass);
			    objcClass != cache.m_cppToObjcClassNames.end()) {
				return objcClass->second + '*';
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
