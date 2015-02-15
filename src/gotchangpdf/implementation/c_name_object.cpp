#include "precompiled.h"

#include "c_name_object.h"
#include "c_helper.h"

using namespace gotchangpdf;

NameHandle Length = reinterpret_cast<NameHandle>(constant::Name::Length.Content.get());
NameHandle Type = reinterpret_cast<NameHandle>(constant::Name::Type.Content.get());
NameHandle ObjStm = reinterpret_cast<NameHandle>(constant::Name::ObjStm.Content.get());
NameHandle N = reinterpret_cast<NameHandle>(constant::Name::N.Content.get());
NameHandle First = reinterpret_cast<NameHandle>(constant::Name::First.Content.get());
NameHandle Extends = reinterpret_cast<NameHandle>(constant::Name::Extends.Content.get());
NameHandle Filter = reinterpret_cast<NameHandle>(constant::Name::Filter.Content.get());
NameHandle FlateDecode = reinterpret_cast<NameHandle>(constant::Name::FlateDecode.Content.get());
NameHandle Catalog = reinterpret_cast<NameHandle>(constant::Name::Catalog.Content.get());
NameHandle Pages = reinterpret_cast<NameHandle>(constant::Name::Pages.Content.get());
NameHandle Page = reinterpret_cast<NameHandle>(constant::Name::Page.Content.get());
NameHandle Count = reinterpret_cast<NameHandle>(constant::Name::Count.Content.get());
NameHandle Kids = reinterpret_cast<NameHandle>(constant::Name::Kids.Content.get());
NameHandle Container = reinterpret_cast<NameHandle>(constant::Name::Container.Content.get());
NameHandle MediaBox = reinterpret_cast<NameHandle>(constant::Name::MediaBox.Content.get());
NameHandle Resources = reinterpret_cast<NameHandle>(constant::Name::Resources.Content.get());
NameHandle Parent = reinterpret_cast<NameHandle>(constant::Name::Parent.Content.get());
NameHandle Root = reinterpret_cast<NameHandle>(constant::Name::Root.Content.get());
NameHandle W = reinterpret_cast<NameHandle>(constant::Name::W.Content.get());

GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_Value(NameHandle handle, PBufferHandle result)
{
	NameObject* obj = reinterpret_cast<NameObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->Value();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_Release(NameHandle handle)
{
	NameObject* obj = reinterpret_cast<NameObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
