#ifndef _C_HANDLES_H
#define _C_HANDLES_H

#define DECLARE_OBJECT_HANDLE(x)	typedef struct x##HandleTag *x##Handle, **P##x##Handle

#ifdef __cplusplus
extern "C"
{
#endif

	/* Basic */
	DECLARE_OBJECT_HANDLE(Object);

	DECLARE_OBJECT_HANDLE(Array);
	DECLARE_OBJECT_HANDLE(Boolean);
	DECLARE_OBJECT_HANDLE(Name);
	DECLARE_OBJECT_HANDLE(Null);
	DECLARE_OBJECT_HANDLE(Real);
	DECLARE_OBJECT_HANDLE(Integer);
	DECLARE_OBJECT_HANDLE(Stream);
	DECLARE_OBJECT_HANDLE(HexadecimalString);
	DECLARE_OBJECT_HANDLE(LiteralString);
	DECLARE_OBJECT_HANDLE(IndirectReference);
	DECLARE_OBJECT_HANDLE(Function);
	DECLARE_OBJECT_HANDLE(Dictionary);

	DECLARE_OBJECT_HANDLE(DictionaryIterator);

	/* File */
	DECLARE_OBJECT_HANDLE(File);
	DECLARE_OBJECT_HANDLE(FileHolder);
	DECLARE_OBJECT_HANDLE(XrefEntry);
	DECLARE_OBJECT_HANDLE(XrefFreeEntry);
	DECLARE_OBJECT_HANDLE(XrefUsedEntry);
	DECLARE_OBJECT_HANDLE(XrefCompressedEntry);
	DECLARE_OBJECT_HANDLE(Xref);
	DECLARE_OBJECT_HANDLE(XrefChain);
	DECLARE_OBJECT_HANDLE(XrefSubsection);

	DECLARE_OBJECT_HANDLE(XrefChainIterator);

	/* Document */
	DECLARE_OBJECT_HANDLE(Document);
	DECLARE_OBJECT_HANDLE(Catalog);
	DECLARE_OBJECT_HANDLE(PageTree);
	DECLARE_OBJECT_HANDLE(PageObject);
	
	/* Contents */
	DECLARE_OBJECT_HANDLE(Contents);

	/* Content instructions */
	DECLARE_OBJECT_HANDLE(ContentInstruction);
	DECLARE_OBJECT_HANDLE(ContentObject);
	DECLARE_OBJECT_HANDLE(ContentOperation);

	/* Content operations */
	DECLARE_OBJECT_HANDLE(ContentOperationGeneric);

	/* Content operators */
	DECLARE_OBJECT_HANDLE(ContentOperator);
	DECLARE_OBJECT_HANDLE(ContentOperatorUnknown);

	/* Content objects */
	DECLARE_OBJECT_HANDLE(ContentObjectText);

	/* Utils */
	DECLARE_OBJECT_HANDLE(Buffer);

#ifdef __cplusplus
};
#endif

#endif /* _C_HANDLES_H */
