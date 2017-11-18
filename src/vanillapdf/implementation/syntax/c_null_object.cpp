#include "precompiled.h"

#include "vanillapdf/syntax/c_null_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION NullObject_Release(NullObjectHandle handle)
{
	return ObjectRelease<NullObject, NullObjectHandle>(handle);
}
