#include "precompiled.h"
#include "semantics/objects/font.h"

#include "vanillapdf/semantics/c_font.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Font_GetFontType(FontHandle* handle, FontType* result)
{
	FontBase* obj = reinterpret_cast<FontBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetFontType()) {
	case FontBase::Type::Type0:
		*result = FontType_Type0; break;
	case FontBase::Type::Type1:
		*result = FontType_Type1; break;
	case FontBase::Type::MMType1:
		*result = FontType_MMType1; break;
	case FontBase::Type::Type3:
		*result = FontType_Type3; break;
	case FontBase::Type::TrueType:
		*result = FontType_TrueType; break;
	case FontBase::Type::CIDFontType0:
		*result = FontType_CIDFontType0; break;
	case FontBase::Type::CIDFontType2:
		*result = FontType_CIDFontType2; break;
	default:
		return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Font_ToUnknown(FontHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<FontBase, IUnknown, FontHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Font_FromUnknown(IUnknownHandle* handle, FontHandle** result) {
	return SafeObjectConvert<IUnknown, FontBase, IUnknownHandle, FontHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Font_Release(FontHandle* handle) {
	return ObjectRelease<FontBase, FontHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION Type0Font_GetUnicodeMap(Type0FontHandle* handle, UnicodeCharacterMapHandle** result)
{
	Type0Font* obj = reinterpret_cast<Type0Font*>(handle);
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

VANILLAPDF_API error_type CALLING_CONVENTION Type0Font_ToFont(Type0FontHandle* handle, FontHandle** result) {
	return SafeObjectConvert<Type0Font, FontBase, Type0FontHandle, FontHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Type0Font_FromFont(FontHandle* handle, Type0FontHandle** result) {
	return SafeObjectConvert<FontBase, Type0Font, FontHandle, Type0FontHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Type0Font_Release(Type0FontHandle* handle) {
	return ObjectRelease<Type0Font, Type0FontHandle>(handle);
}
