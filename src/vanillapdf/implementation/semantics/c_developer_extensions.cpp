#include "precompiled.h"

#include "semantics/objects/developer_extensions.h"

#include "vanillapdf/semantics/c_developer_extensions.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtension_GetBaseVersion(DeveloperExtensionHandle* handle, PDFVersion* result)
{
	DeveloperExtension* obj = reinterpret_cast<DeveloperExtension*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		switch (obj->BaseVersion()) {
		case Version::PDF10:
			*result = PDFVersion_10; break;
		case Version::PDF11:
			*result = PDFVersion_11; break;
		case Version::PDF12:
			*result = PDFVersion_12; break;
		case Version::PDF13:
			*result = PDFVersion_13; break;
		case Version::PDF14:
			*result = PDFVersion_14; break;
		case Version::PDF15:
			*result = PDFVersion_15; break;
		case Version::PDF16:
			*result = PDFVersion_16; break;
		case Version::PDF17:
			*result = PDFVersion_17; break;
		case Version::PDF20:
			*result = PDFVersion_20; break;
		default:
			return VANILLAPDF_ERROR_NOT_SUPPORTED;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtension_GetExtensionLevel(DeveloperExtensionHandle* handle, IntegerObjectHandle** result)
{
	DeveloperExtension* obj = reinterpret_cast<DeveloperExtension*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto level = obj->ExtensionLevel();
		auto ptr = level.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtension_Release(DeveloperExtensionHandle* handle)
{
	return ObjectRelease<DeveloperExtension, DeveloperExtensionHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensions_GetIterator(DeveloperExtensionsHandle* handle, DeveloperExtensionsIteratorHandle** result)
{
	DeveloperExtensions* obj = reinterpret_cast<DeveloperExtensions*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto it = obj->Begin();
		auto ptr = it.AddRefGet();
		*result = reinterpret_cast<DeveloperExtensionsIteratorHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensions_Release(DeveloperExtensionsHandle* handle)
{
	return ObjectRelease<DeveloperExtensions, DeveloperExtensionsHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_GetKey(DeveloperExtensionsIteratorHandle* handle, NameObjectHandle** result)
{
	DeveloperExtensions::Iterator* obj = reinterpret_cast<DeveloperExtensions::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto name = obj->First();
		auto ptr = name.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_GetValue(DeveloperExtensionsIteratorHandle* handle, DeveloperExtensionHandle** result)
{
	DeveloperExtensions::Iterator* obj = reinterpret_cast<DeveloperExtensions::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto extension = obj->Second();
		auto ptr = extension.AddRefGet();
		*result = reinterpret_cast<DeveloperExtensionHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_IsValid(DeveloperExtensionsIteratorHandle* handle, boolean_type* result)
{
	DeveloperExtensions::Iterator* iterator = reinterpret_cast<DeveloperExtensions::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		if (iterator->IsValid()) {
			*result = VANILLAPDF_RV_TRUE;
		}
		else {
			*result = VANILLAPDF_RV_FALSE;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_Next(DeveloperExtensionsIteratorHandle* handle)
{
	DeveloperExtensions::Iterator* obj = reinterpret_cast<DeveloperExtensions::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		++(*obj);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_Release(DeveloperExtensionsIteratorHandle* handle)
{
	return ObjectRelease<DeveloperExtensions::Iterator, DeveloperExtensionsIteratorHandle>(handle);
}