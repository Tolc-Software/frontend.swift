#pragma once

#include <string>
#include <vector>

namespace Swift::Proxy {
/**
* An attribute is a global value
*/
class Attribute {
public:
	Attribute(std::string const& name, std::string const& fullyQualifiedName);

	/**
	* Creates a string corresponding to the pybind11 conversion of this attribute.
	* E.g.
	*   attr("i") = &MyNamespace::i;
	*/
	std::string getSwift() const;

private:
	// The user defined name of the enum
	std::string m_name;
	std::string m_fullyQualifiedName;
};
}    // namespace Swift::Proxy
