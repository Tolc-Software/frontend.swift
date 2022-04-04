#include "ObjcSwift/Helpers/trampolineClass.hpp"
#include "ObjcSwift/Helpers/split.hpp"
#include "ObjcSwift/Proxy/function.hpp"
#include <fmt/format.h>
#include <string>
#include <vector>

namespace ObjcSwift::Helpers {

namespace {
std::string joinVirtual(std::string const& parentClass,
                        std::vector<ObjcSwift::Proxy::Function> const& virtuals,
                        bool isPure) {
	std::string joined;
	auto withArgumentNames = true;
	for (auto const& f : virtuals) {
		joined += fmt::format(
		    R"_tolc_delimiter(
	{returnType} {name}({arguments}) override {{
		PYBIND11_OVERRIDE_{pure}NAME(
			{returnType},
			{parentClass},
			"{pythonName}",
			{name},
			{argumentNames}
		);
	}}
)_tolc_delimiter",
		    fmt::arg("returnType", f.getReturnType()),
		    fmt::arg("name", f.getName()),
		    fmt::arg("arguments", f.getArgumentTypes(withArgumentNames)),
		    fmt::arg("pure", isPure ? "PURE_" : ""),
		    fmt::arg("parentClass", parentClass),
		    fmt::arg("pythonName", f.getPythonName()),
		    fmt::arg("argumentNames", f.getArgumentNames()));
	}
	return joined;
}

std::string getTrampolineClassName(std::string const& fullyQualifiedName) {
	return fmt::format(
	    "Py{}",
	    fmt::join(ObjcSwift::Helpers::split(fullyQualifiedName, "::"), "_"));
}
}    // namespace

std::pair<std::string, std::string> getTrampolineClass(
    std::string const& className,
    std::string const& fullyQualifiedName,
    std::vector<ObjcSwift::Proxy::Function> const& virtualFunctions,
    std::vector<ObjcSwift::Proxy::Function> const& pureVirtualFunctions) {
	std::string trampolineClassName =
	    getTrampolineClassName(fullyQualifiedName);
	return std::make_pair(
	    trampolineClassName,
	    fmt::format(
	        R"_tolc_delimiter(
class {trampolineClassName} : public {fullyQualifiedName} {{
public:
	/* Inherit the constructors */
	using {fullyQualifiedName}::{className};

	/* Virtual trampoline functions */
{virtuals}
	/* Pure virtual trampoline functions */
{pureVirtuals}
}};
)_tolc_delimiter",
	        fmt::arg("trampolineClassName", trampolineClassName),
	        fmt::arg("fullyQualifiedName", fullyQualifiedName),
	        fmt::arg("className", className),
	        fmt::arg("virtuals",
	                 joinVirtual(fullyQualifiedName, virtualFunctions, false)),
	        fmt::arg(
	            "pureVirtuals",
	            joinVirtual(fullyQualifiedName, pureVirtualFunctions, true))));
}
}    // namespace ObjcSwift::Helpers
