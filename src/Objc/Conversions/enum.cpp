#include "Objc/Conversions/enum.hpp"
#include "Objc/Proxy/enum.hpp"
#include <fmt/format.h>
#include <spdlog/spdlog.h>

namespace Objc::Conversions {
namespace {
std::string casesFromTo(std::string const& fromPrefix,
                        std::vector<std::string> const& from,
                        std::string const& toPrefix,
                        std::vector<std::string> const& to) {
	if (from.size() != to.size()) {
		spdlog::error(
		    "Something went wrong while converting Enums from C++ to Objective-C. Please open an issue.");
		return "";
	}
	std::string out;
	for (size_t i = 0; i < from.size(); ++i) {
		out += fmt::format(R"(    case {}{}:
      return {}{};
)",
		                   fromPrefix,
		                   from[i],
		                   toPrefix,
		                   to[i]);
	}

	return out;
}
}    // namespace

std::array<std::pair<std::string, std::string>, 2>
getEnumConversions(IR::Enum const& cppEnum, Objc::Proxy::Enum const& objcEnum) {
	std::string toObjcName =
	    fmt::format("convertEnum{}To{}", cppEnum.m_name, objcEnum.getName());
	std::string toCppName =
	    fmt::format("convertEnum{}To{}", objcEnum.getName(), cppEnum.m_name);

	return {std::make_pair(
	            toCppName,
	            fmt::format(
	                R"(
{cppEnum} {toCppName}({objcEnum} e) {{
  switch (e) {{
{cases}
  }}
}})",
	                fmt::arg("cppEnum", cppEnum.m_representation),
	                fmt::arg("toCppName", toCppName),
	                fmt::arg("objcEnum", objcEnum.getName()),
	                fmt::arg("cases",
	                         casesFromTo("",
	                                     objcEnum.getValues(),
	                                     cppEnum.m_isScoped ?
                                             cppEnum.m_representation + "::" :
                                             "",
	                                     cppEnum.m_values)))),
	        std::make_pair(
	            toObjcName,
	            fmt::format(
	                R"(
{objcEnum} {toObjcName}({cppEnum} e) {{
  switch (e) {{
{cases}
  }}
}})",
	                fmt::arg("objcEnum", objcEnum.getName()),
	                fmt::arg("toObjcName", toObjcName),
	                fmt::arg("cppEnum", cppEnum.m_representation),
	                fmt::arg("cases",
	                         casesFromTo(cppEnum.m_isScoped ?
                                             cppEnum.m_representation + "::" :
                                             "",
	                                     cppEnum.m_values,
	                                     "",
	                                     objcEnum.getValues()))))};
}
}    // namespace Objc::Conversions
