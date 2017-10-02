#include "precompiled.h"

#include "vanillapdf/c_null_object.h"
#include "c_helper.h"

using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION NullObject_Release(NullObjectHandle handle)
{
	return ObjectRelease<NullObject, NullObjectHandle>(handle);
}
