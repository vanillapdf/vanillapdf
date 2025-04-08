#include "precompiled.h"
#include "utils/unknown_interface.h"

#include "vanillapdf/utils/c_unknown_interface.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION IUnknown_AddRef(IUnknownHandle* handle)
{
    IUnknown* obj = reinterpret_cast<IUnknown*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->AddRef();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION IUnknown_Release(IUnknownHandle* handle)
{
    return ObjectRelease<IUnknown, IUnknownHandle>(handle);
}
