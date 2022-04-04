#pragma once

#include "ObjcSwift/Proxy/function.hpp"
#include <string>

namespace ObjcSwift {
std::string
returnValuePolicyToString(ObjcSwift::Proxy::Function::return_value_policy p);
}
