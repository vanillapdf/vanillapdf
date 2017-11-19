#include "precompiled.h"
#include "semantics/objects/font.h"

#include "vanillapdf/semantics/c_font.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Font_Type(FontHandle* handle, FontType* result)
{
	FontBase* obj = reinterpret_cast<FontBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetType()) {
	case FontBase::Type::Composite:
		*result = FontType_Composite; break;
	case FontBase::Type::Type1:
		*result = FontType_Type1; break;
	case FontBase::Type::Type3:
		*result = FontType_Type3; break;
	default:
		return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Font_Release(FontHandle* handle)
{
	return ObjectRelease<FontBase, FontHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION Font_ToComposite(FontHandle* handle, CompositeFontHandle** result)
{
	return SafeObjectConvert<FontBase, CompositeFont, FontHandle, CompositeFontHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION CompositeFont_GetUnicodeMap(CompositeFontHandle* handle, UnicodeCharacterMapHandle** result)
{
	CompositeFont* obj = reinterpret_cast<CompositeFont*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OuputUnicodeCharacterMapPtr map;
		bool contains = obj->ToUnicode(map);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = map.AddRefGet();
		*result = reinterpret_cast<UnicodeCharacterMapHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION CompositeFont_Release(CompositeFontHandle* handle)
{
	return ObjectRelease<CompositeFont, CompositeFontHandle>(handle);
}