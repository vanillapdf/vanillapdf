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
	DECLARE_OBJECT_HANDLE(String);
	DECLARE_OBJECT_HANDLE(IndirectReference);
	DECLARE_OBJECT_HANDLE(Dictionary);

	DECLARE_OBJECT_HANDLE(DictionaryIterator);

	DECLARE_OBJECT_HANDLE(HexadecimalString);
	DECLARE_OBJECT_HANDLE(LiteralString);

	/* File */
	DECLARE_OBJECT_HANDLE(File);
	DECLARE_OBJECT_HANDLE(XrefEntry);
	DECLARE_OBJECT_HANDLE(XrefFreeEntry);
	DECLARE_OBJECT_HANDLE(XrefUsedEntry);
	DECLARE_OBJECT_HANDLE(XrefCompressedEntry);
	DECLARE_OBJECT_HANDLE(Xref);
	DECLARE_OBJECT_HANDLE(XrefChain);

	DECLARE_OBJECT_HANDLE(XrefChainIterator);

	/* Document */
	DECLARE_OBJECT_HANDLE(Document);
	DECLARE_OBJECT_HANDLE(Catalog);
	DECLARE_OBJECT_HANDLE(PageTree);
	DECLARE_OBJECT_HANDLE(PageObject);
	DECLARE_OBJECT_HANDLE(DeveloperExtensions);
	DECLARE_OBJECT_HANDLE(PageLabels);
	DECLARE_OBJECT_HANDLE(PageLabel);
	DECLARE_OBJECT_HANDLE(ViewerPreferences);
	DECLARE_OBJECT_HANDLE(PageRange);
	DECLARE_OBJECT_HANDLE(PageSubRange);
	DECLARE_OBJECT_HANDLE(OutlineBase);
	DECLARE_OBJECT_HANDLE(Outline);
	DECLARE_OBJECT_HANDLE(OutlineItem);
	DECLARE_OBJECT_HANDLE(OutlineItemColor);
	DECLARE_OBJECT_HANDLE(OutlineItemFlags);
	
	/* Contents */
	DECLARE_OBJECT_HANDLE(Contents);

	/* Content instructions */
	DECLARE_OBJECT_HANDLE(ContentInstruction);
	DECLARE_OBJECT_HANDLE(ContentObject);
	DECLARE_OBJECT_HANDLE(ContentOperation);

	/* Content operations */
	DECLARE_OBJECT_HANDLE(ContentOperationGeneric);
	DECLARE_OBJECT_HANDLE(ContentOperationTextShow);

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
