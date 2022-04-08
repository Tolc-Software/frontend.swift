#include "ObjcSwift/Builders/typeBuilder.hpp"
#include "Swift/types.hpp"

namespace ObjcSwift::Builders {
ObjcSwift::Type toObjcSwiftType(IR::Type const& type) {
	ObjcSwift::Type out;

	out.cppName = type.m_representation;
	out.swiftName = Swift::toSwiftType(type);
	return out;
}
}    // namespace ObjcSwift::Builders
