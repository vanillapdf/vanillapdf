#ifndef _C_HANDLES_H
#define _C_HANDLES_H

#define DECLARE_OBJECT_HANDLE(x)	typedef struct x##HandleTag *x##Handle

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
		IndirectObjectReference,
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
	DECLARE_OBJECT_HANDLE(IndirectObjectReference);
	DECLARE_OBJECT_HANDLE(File);
	DECLARE_OBJECT_HANDLE(XrefEntry);
	DECLARE_OBJECT_HANDLE(Xref);
	DECLARE_OBJECT_HANDLE(Object);

	DECLARE_OBJECT_HANDLE(Catalog);
	DECLARE_OBJECT_HANDLE(PageTree);
	DECLARE_OBJECT_HANDLE(PageObject);

	DECLARE_OBJECT_HANDLE(ArrayObject);
	DECLARE_OBJECT_HANDLE(IndirectObject);

	DECLARE_OBJECT_HANDLE(DictionaryObject);
	DECLARE_OBJECT_HANDLE(DictionaryObjectIterator);

#ifdef __cplusplus
};
#endif

#endif /* _C_HANDLES_H */
