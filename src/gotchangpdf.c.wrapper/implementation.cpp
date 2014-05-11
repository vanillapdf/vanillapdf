#include "gotchangpdf.c.wrapper/interface.h"
#include "gotchangpdf/interface.h"

#pragma region ArrayObject

using namespace gotchangpdf;

GOTCHANG_PDF_WRAPPER_API ObjectHandle CALLING_CONVENTION ArrayObject_At(ArrayObjectHandle handle, int at)
{
	IArrayObject* arr = reinterpret_cast<IArrayObject*>(handle);
	return reinterpret_cast<ObjectHandle>(arr->At(at));
}

GOTCHANG_PDF_WRAPPER_API int CALLING_CONVENTION ArrayObject_Size(ArrayObjectHandle handle)
{
	IArrayObject* arr = reinterpret_cast<IArrayObject*>(handle);
	return arr->Size();
}

GOTCHANG_PDF_WRAPPER_API void CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle handle)
{
	IArrayObject* obj = reinterpret_cast<IArrayObject*>(handle);
	obj->Release();
}

#pragma endregion

#pragma region DictionaryObject

using namespace gotchangpdf;

GOTCHANG_PDF_WRAPPER_API ObjectHandle CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle handle, const char *str, int len)
{
	/*
	gotchangpdf::IDictionaryObject* dictionary = reinterpret_cast<gotchangpdf::IDictionaryObject*>(handle);
	gotchangpdf::Buffer set(str, len);
	gotchangpdf::NameObject name(set);
	gotchangpdf::ObjectReferenceWrapper<gotchangpdf::Object> object = dictionary->Find(name);
	gotchangpdf::Object* ptr = object.AddRefGet();
	//boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<ObjectHandle>(ptr);
	*/
	return nullptr;
}

GOTCHANG_PDF_WRAPPER_API DictionaryObjectIteratorHandle CALLING_CONVENTION DictionaryObject_Iterator(DictionaryObjectHandle handle)
{
	IDictionaryObject* dictionary = reinterpret_cast<IDictionaryObject*>(handle);
	auto it = dictionary->Begin();
	return reinterpret_cast<DictionaryObjectIteratorHandle>(it);
}

GOTCHANG_PDF_WRAPPER_API void CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryObjectIteratorHandle handle)
{
	IDictionaryObject::IIterator* iterator = reinterpret_cast<IDictionaryObject::IIterator*>(handle);
	(*iterator)++;
}

GOTCHANG_PDF_WRAPPER_API void CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryObjectIteratorHandle handle)
{
	IDictionaryObject::IIterator* iterator = reinterpret_cast<IDictionaryObject::IIterator*>(handle);
	delete iterator;
}

GOTCHANG_PDF_WRAPPER_API NameObjectHandle CALLING_CONVENTION DictionaryObjectIterator_GetKey(DictionaryObjectIteratorHandle handle)
{
	IDictionaryObject::IIterator* it = reinterpret_cast <IDictionaryObject::IIterator*>(handle);
	return reinterpret_cast<NameObjectHandle>(it->First());
}

GOTCHANG_PDF_WRAPPER_API ObjectHandle CALLING_CONVENTION DictionaryObjectIterator_GetValue(DictionaryObjectIteratorHandle handle)
{
	IDictionaryObject::IIterator* it = reinterpret_cast <IDictionaryObject::IIterator*>(handle);
	IObject* ptr = it->Second();
	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_WRAPPER_API int CALLING_CONVENTION DictionaryObjectIterator_IsValid(DictionaryObjectIteratorHandle handle, DictionaryObjectHandle dictionaryHandle)
{
	IDictionaryObject::IIterator* iterator = reinterpret_cast<IDictionaryObject::IIterator*>(handle);
	IDictionaryObject* dictionary = reinterpret_cast<IDictionaryObject*>(dictionaryHandle);
	if (*dictionary->End() == *iterator)
		return GOTCHANG_PDF_RV_FALSE;
	else
		return GOTCHANG_PDF_RV_TRUE;
}

GOTCHANG_PDF_WRAPPER_API void CALLING_CONVENTION DictionaryObject_Release(DictionaryObjectHandle handle)
{
	IDictionaryObject* obj = reinterpret_cast<IDictionaryObject*>(handle);
	obj->Release();
}

#pragma endregion

#pragma region Object

using namespace gotchangpdf;

GOTCHANG_PDF_WRAPPER_API const char* Object_TypeName(ObjectType type)
{
	return IObject::TypeName(static_cast<IObject::Type>(type));
}

GOTCHANG_PDF_WRAPPER_API ObjectType CALLING_CONVENTION Object_Type(ObjectHandle handle)
{
	IObject* obj = reinterpret_cast<IObject*>(handle);
	return static_cast<ObjectType>(obj->GetType());
}

GOTCHANG_PDF_WRAPPER_API void CALLING_CONVENTION Object_Release(ObjectHandle handle)
{
	IObject* obj = reinterpret_cast<IObject*>(handle);
	obj->Release();
}

#pragma endregion

#pragma region File

using namespace gotchangpdf::files;

GOTCHANG_PDF_WRAPPER_API FileHandle CALLING_CONVENTION File_Create(const char *filename)
{
	return reinterpret_cast<FileHandle>(IFile::Create(filename));
}

GOTCHANG_PDF_WRAPPER_API void CALLING_CONVENTION File_Release(FileHandle handle)
{
	IFile* file = reinterpret_cast<IFile*>(handle);
	delete file;
}

GOTCHANG_PDF_WRAPPER_API int CALLING_CONVENTION File_Initialize(FileHandle handle)
{
	IFile* file = reinterpret_cast<IFile*>(handle);

	file->Initialize();
	return 0;
}

GOTCHANG_PDF_WRAPPER_API XrefHandle CALLING_CONVENTION File_Xref(FileHandle handle)
{
	IFile* file = reinterpret_cast<IFile*>(handle);
	auto table = file->GetXref();
	return reinterpret_cast<XrefHandle>(table);
}

GOTCHANG_PDF_WRAPPER_API IndirectObjectHandle CALLING_CONVENTION File_GetIndirectObject(FileHandle handle, int objNumber, int genNumber)
{
	IFile* file = reinterpret_cast<IFile*>(handle);
	IIndirectObject* item = file->GetIndirectObject(objNumber, genNumber);
	return reinterpret_cast<IndirectObjectHandle>(item);
}
/*
GOTCHANG_PDF_WRAPPER_API IndirectObjectHandle CALLING_CONVENTION File_GetDocumentCatalog(FileHandle handle)
{
	IFile* file = reinterpret_cast<IFile*>(handle);

	auto item = file->GetDocumentCatalog();
	auto ptr = item.AddRefGet();

	return reinterpret_cast<IndirectObjectHandle>(ptr);
}
*/
#pragma endregion

#pragma region IndirectObject

using namespace gotchangpdf;

GOTCHANG_PDF_WRAPPER_API long long CALLING_CONVENTION IndirectObject_GetOffset(IndirectObjectHandle handle)
{
	IIndirectObject *obj = reinterpret_cast<IIndirectObject*>(handle);
	return obj->GetOffset();
}

GOTCHANG_PDF_WRAPPER_API ObjectHandle CALLING_CONVENTION IndirectObject_GetObject(IndirectObjectHandle handle)
{
	IIndirectObject *obj = reinterpret_cast<IIndirectObject*>(handle);
	IObject *item = obj->GetObject();
	return reinterpret_cast<ObjectHandle>(item);
}

GOTCHANG_PDF_WRAPPER_API void CALLING_CONVENTION IndirectObject_Release(IndirectObjectHandle handle)
{
	IIndirectObject *obj = reinterpret_cast<IIndirectObject*>(handle);
	obj->Release();
}

#pragma endregion

#pragma region IntegerObject

using namespace gotchangpdf;

GOTCHANG_PDF_WRAPPER_API long long CALLING_CONVENTION IntegerObject_Value(IntegerObjectHandle handle)
{
	IIntegerObject* obj = reinterpret_cast<IIntegerObject*>(handle);
	return obj->Value();
}

GOTCHANG_PDF_WRAPPER_API void CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle handle)
{
	IIntegerObject* obj = reinterpret_cast<IIntegerObject*>(handle);
	obj->Release();
}

#pragma endregion

#pragma region Xref

using namespace gotchangpdf;

GOTCHANG_PDF_WRAPPER_API int CALLING_CONVENTION Xref_Size(XrefHandle handle)
{
	IXref* table = reinterpret_cast<IXref*>(handle);
	return table->Size();
}

GOTCHANG_PDF_WRAPPER_API XrefEntryHandle CALLING_CONVENTION Xref_At(XrefHandle handle, int at)
{
	IXref* table = reinterpret_cast<IXref*>(handle);
	IXref::IEntry* entry = new IXref::IEntry(table->At(at));
	return reinterpret_cast<XrefEntryHandle>(entry);
}

GOTCHANG_PDF_WRAPPER_API void CALLING_CONVENTION Xref_Release(XrefHandle handle)
{
	IXref* table = reinterpret_cast<IXref*>(handle);
	table->Release();
}

GOTCHANG_PDF_WRAPPER_API void CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle)
{
	IXref::IEntry* entry = reinterpret_cast<IXref::IEntry*>(handle);
	delete entry;
}

GOTCHANG_PDF_WRAPPER_API IndirectObjectHandle CALLING_CONVENTION XrefEntry_Reference(XrefEntryHandle handle)
{
	IXref::IEntry* entry = reinterpret_cast<IXref::IEntry*>(handle);

	IIndirectObject *ptr = entry->_reference;
	//boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<IndirectObjectHandle>(ptr);
}

GOTCHANG_PDF_WRAPPER_API int CALLING_CONVENTION XrefEntry_In_Use(XrefEntryHandle handle)
{
	IXref::IEntry* entry = reinterpret_cast<IXref::IEntry*>(handle);

	if (entry->_in_use)
		return GOTCHANG_PDF_RV_TRUE;
	else
		return GOTCHANG_PDF_RV_FALSE;
}

#pragma endregion