#include "ObjcSwift/Builders/attributeBuilder.hpp"
#include "TestUtil/string.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <string>

TEST_CASE("Can build simple attributes", "[attributeBuilder]") {
	IR::Variable v;

	std::string variableName = "i";
	v.m_name = variableName;

	std::string moduleName = "Module";
	auto fullyQualifiedName = moduleName + "::" + variableName;

	auto proxyAttribute = ObjcSwift::Builders::buildAttribute(moduleName, v);
	auto pybind = proxyAttribute.getObjcSwift();
	CAPTURE(pybind);

	auto expectedContains =
	    fmt::format(R"(attr("{name}") = &{fullyQualifiedName})",
	                fmt::arg("fullyQualifiedName", fullyQualifiedName),
	                fmt::arg("name", v.m_name));
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
}
