#ifndef _EXPORT_H
#define _EXPORT_H

#define CALLING_CONVENTION __stdcall

#ifdef PROJECT_CONFIGURATION_DLL
	#ifdef PDF_EXPORTS
		#define GOTCHANG_PDF_API __declspec(dllexport)
	#else
		#define GOTCHANG_PDF_API __declspec(dllimport)
	#endif
#else
	#define GOTCHANG_PDF_API
#endif

#define DECLARE_OBJECT_HANDLE(x)	typedef struct x##HandleTag *x##Handle

#define GOTCHANG_PDF_RV_FALSE		0
#define GOTCHANG_PDF_RV_TRUE		1

#ifdef __cplusplus
extern "C"
{
#endif

	// TODO i have no idea how export enum without clone
	enum ObjectType
	{
		Unknown = 0,
		ArrayObject,
		Boolean,
		DictionaryObject,
		Function,
		IntegerObject,
		NameObject,
		//NameTree,
		NullObject,
		//NumberTree,
		RealObject,
		//Rectangle,
		StreamObject,
		HexadecimalString,
		LiteralString,
		IndirectReference,
		IndirectObject
	};

	DECLARE_OBJECT_HANDLE(Boolean);
	DECLARE_OBJECT_HANDLE(NameObject);
	DECLARE_OBJECT_HANDLE(NameTree);
	DECLARE_OBJECT_HANDLE(NullObject);
	DECLARE_OBJECT_HANDLE(ObjectStream);
	DECLARE_OBJECT_HANDLE(RealObject);
	DECLARE_OBJECT_HANDLE(IntegerObject);
	DECLARE_OBJECT_HANDLE(StreamObject);
	DECLARE_OBJECT_HANDLE(HexadecimalString);
	DECLARE_OBJECT_HANDLE(LiteralString);
	DECLARE_OBJECT_HANDLE(IndirectReference);
	DECLARE_OBJECT_HANDLE(File);
	DECLARE_OBJECT_HANDLE(XrefEntry);
	DECLARE_OBJECT_HANDLE(Xref);
	DECLARE_OBJECT_HANDLE(Object);
	
	DECLARE_OBJECT_HANDLE(ArrayObject);
	DECLARE_OBJECT_HANDLE(IndirectObject);

	DECLARE_OBJECT_HANDLE(DictionaryObject);
	DECLARE_OBJECT_HANDLE(DictionaryObjectIterator);
	DECLARE_OBJECT_HANDLE(DictionaryObjectPair);
	
	GOTCHANG_PDF_API int CALLING_CONVENTION XrefEntry_In_Use(XrefEntryHandle handle);
	GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION XrefEntry_Reference(XrefEntryHandle handle);
	GOTCHANG_PDF_API void CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle);
	
	GOTCHANG_PDF_API int CALLING_CONVENTION Xref_Size(XrefHandle);
	GOTCHANG_PDF_API XrefEntryHandle CALLING_CONVENTION Xref_At(XrefHandle, int at);
	GOTCHANG_PDF_API void CALLING_CONVENTION Xref_Release(XrefHandle);
	
	GOTCHANG_PDF_API enum ObjectType CALLING_CONVENTION Object_Type(ObjectHandle);
	GOTCHANG_PDF_API const char* Object_TypeName(enum ObjectType type);
	GOTCHANG_PDF_API void CALLING_CONVENTION Object_Release(ObjectHandle);

	GOTCHANG_PDF_API NameObjectHandle CALLING_CONVENTION DictionaryObjectPair_GetKey(DictionaryObjectPairHandle);
	GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObjectPair_GetValue(DictionaryObjectPairHandle);
	GOTCHANG_PDF_API int CALLING_CONVENTION DictionaryObjectPair_IsValid(DictionaryObjectPairHandle);
	GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectPair_Release(DictionaryObjectPairHandle);

	GOTCHANG_PDF_API DictionaryObjectPairHandle CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryObjectIteratorHandle, DictionaryObjectHandle dictionaryHandle);
	GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryObjectIteratorHandle);

	GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle, const char *str, int len);
	GOTCHANG_PDF_API DictionaryObjectIteratorHandle CALLING_CONVENTION DictionaryObject_Iterator(DictionaryObjectHandle);
	GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObject_Release(DictionaryObjectHandle);

	GOTCHANG_PDF_API int CALLING_CONVENTION ArrayObject_Size(ArrayObjectHandle);
	GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION ArrayObject_At(ArrayObjectHandle, int at);
	GOTCHANG_PDF_API void CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle);

	GOTCHANG_PDF_API long long CALLING_CONVENTION IntegerObject_Value(IntegerObjectHandle);
	GOTCHANG_PDF_API void CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle);

	GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION IndirectObject_GetObject(IndirectObjectHandle);
	GOTCHANG_PDF_API long long CALLING_CONVENTION IndirectObject_GetOffset(IndirectObjectHandle);
	GOTCHANG_PDF_API void CALLING_CONVENTION IndirectObject_Release(IndirectObjectHandle);

	GOTCHANG_PDF_API FileHandle CALLING_CONVENTION File_Create(const char *filename);
	GOTCHANG_PDF_API void CALLING_CONVENTION File_Release(FileHandle);
	GOTCHANG_PDF_API int CALLING_CONVENTION File_Initialize(FileHandle);
	GOTCHANG_PDF_API XrefHandle CALLING_CONVENTION File_Xref(FileHandle);
	GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION File_GetIndirectObject(FileHandle, int objNumber, int genNumber);
	GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION File_GetDocumentCatalog(FileHandle);

#ifdef __cplusplus
};
#endif

#endif /* _EXPORT_H */
