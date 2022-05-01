#include "TestUtil/parse.hpp"
#include "TestUtil/parserConfig.hpp"
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <catch2/catch.hpp>
#include <string>

namespace TestUtil {
IR::Namespace parse(std::string code) {
	auto parsed = Parser::parseString(code, TestUtil::getParserConfig());
	REQUIRE(parsed.has_value());
	return parsed.value().first;
}
}    // namespace TestUtil

