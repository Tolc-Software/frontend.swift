#include "Objc/Builders/classBuilder.hpp"
#include "Objc/Builders/attributeBuilder.hpp"
#include "Objc/Builders/enumBuilder.hpp"
#include "Objc/Builders/functionBuilder.hpp"
#include "Objc/Builders/typeBuilder.hpp"
#include "Objc/Conversions/userDefined.hpp"
#include "Objc/Proxy/attribute.hpp"
#include "Objc/Proxy/function.hpp"
#include "Objc/cache.hpp"
#include "Objc/getName.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/Helpers/operatorNames.hpp"
#include "ObjcSwift/Helpers/typeToStringBuilder.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "ObjcSwift/getOverloadedFunctions.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <numeric>
#include <optional>
#include <set>
#include <string>
#include <string_view>

namespace Objc::Builders {

namespace {

void buildMemberFunction(Objc::Proxy::Function& objcFunction,
                         IR::Function const& cppFunction,
                         std::set<std::string> const& overloadedFunctions) {
	if (cppFunction.m_isStatic) {
		objcFunction.setAsStatic();
	}

	if (overloadedFunctions.find(cppFunction.m_representation) !=
	    overloadedFunctions.end()) {
		objcFunction.setAsOverloaded();
	}
}

std::string declareClassCategory(std::string_view objcName,
                                 std::string_view cppName) {
	return fmt::format(R"(
@interface {objcName}(e_{objcName}_private)
-(instancetype) Tolc_initWithCppObject:({cppName} const&)cppClass;
-({cppName}&) Tolc_getCppObject;
@end
)",
	                   fmt::arg("objcName", objcName),
	                   fmt::arg("cppName", cppName));
}

std::string getConversions(std::string_view objcName,
                           std::string_view cppName) {
	// No need for conversions for an object that cannot be instantiated
	auto conversions =
	    Objc::Conversions::getUserDefinedConversionNames(cppName, "");
	return fmt::format(R"(
{objcName}* {toObjcName}({cppName} const& cppClass) {{
  return [[{objcName} alloc] Tolc_initWithCppObject:cppClass];
}}

{cppName}& {toCppName}({objcName}* objcClass) {{
  return [objcClass Tolc_getCppObject];
}}
)",
	                   fmt::arg("objcName", objcName),
	                   fmt::arg("toObjcName", conversions.m_toObjc),
	                   fmt::arg("toCppName", conversions.m_toCpp),
	                   fmt::arg("cppName", cppName));
}

}    // namespace

std::optional<Objc::Proxy::Class> buildClass(IR::Struct const& cppClass,
                                             Objc::Cache& cache) {
	Objc::Proxy::Class objcClass(
	    Objc::getClassName(cppClass, cache.m_moduleName),
	    cppClass.m_representation);

	for (auto const& e : cppClass.m_public.m_enums) {
		objcClass.addEnum(buildEnum(e, cache));
	}

	objcClass.setDocumentation(cppClass.m_documentation);

	objcClass.setInherited(cppClass.m_public.m_inherited);

	cache.m_extraClassCategories.push_back(
	    declareClassCategory(objcClass.getName(), cppClass.m_representation));

	cache.m_extraClassConversions.push_back(
	    getConversions(objcClass.getName(), cppClass.m_representation));

	// Ignore private functions
	auto overloadedFunctions =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_functions);
	for (auto const& function : cppClass.m_public.m_functions) {
		if (auto maybeObjcFunction = buildFunction(objcClass.getName(),
		                                           cppClass.m_representation,
		                                           function,
		                                           cache)) {
			auto& objcFunction = maybeObjcFunction.value();

			buildMemberFunction(objcFunction, function, overloadedFunctions);

			objcClass.addFunction(objcFunction);
		} else {
			return std::nullopt;
		}
	}

	auto overloadedOperators =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_operators);
	for (auto const& [op, function] : cppClass.m_public.m_operators) {
		if (auto maybeObjcFunction = buildFunction(objcClass.getName(),
		                                           cppClass.m_representation,
		                                           function,
		                                           cache)) {
			if (auto maybeName = ObjcSwift::Helpers::getOperatorName(op)) {
				auto& objcFunction = maybeObjcFunction.value();

				buildMemberFunction(
				    objcFunction, function, overloadedOperators);

				objcClass.addFunction(objcFunction);
			}
		} else {
			return std::nullopt;
		}
	}

	for (auto const& constructor : cppClass.m_public.m_constructors) {
		if (auto maybeObjcFunction = buildFunction(objcClass.getName(),
		                                           cppClass.m_representation,
		                                           constructor,
		                                           cache,
		                                           true)) {
			auto& objcFunction = maybeObjcFunction.value();

			if (constructor.m_isStatic) {
				objcFunction.setAsStatic();
			}

			if (cppClass.m_public.m_constructors.size() > 1) {
				objcFunction.setAsOverloaded();
			}

			objcFunction.setAsConstructor();

			objcClass.addConstructor(objcFunction);
		}
	}

	for (auto const& variable : cppClass.m_public.m_memberVariables) {
		objcClass.addMemberVariable(Objc::Builders::buildAttribute(
		    objcClass.getName(), cppClass.m_representation, variable, cache));
	}

	// Add default constructor
	if (cppClass.m_hasImplicitDefaultConstructor) {
		auto constructor = Objc::Proxy::Function("init",
		                                         objcClass.getName(),
		                                         objcClass.getName(),
		                                         cppClass.m_representation);
		Objc::Proxy::Type returnType;
		returnType.m_name = "instancetype";
		returnType.m_conversions.m_toObjc = "";
		returnType.m_conversions.m_toCpp = "";
		constructor.setReturnType(returnType);
		constructor.setAsConstructor();
		objcClass.addConstructor(constructor);
	}

	return objcClass;
}
}    // namespace Objc::Builders
