#ifndef _C_HANDLES_H
#define _C_HANDLES_H

/**
* \def DECLARE_OBJECT_HANDLE(x)
* Declares opaque pointer to struct allocated by library.
*/
#define DECLARE_OBJECT_HANDLE(x) \
typedef struct x##HandleTag *x##Handle; \
/** \brief Pointer to x##Handle.
*/ \
typedef x##Handle *P##x##Handle

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_handles.h
	* This file contains all type definitions returned by the library API.
	*/

	/**
	* \class IUnknownHandle
	* \brief Base class for reference counting
	*/
	DECLARE_OBJECT_HANDLE(IUnknown);

	#pragma region Syntax

	#pragma region Objects

	/**
	* \defgroup Objects Objects
	* \ingroup Files
	* \brief Classes used as syntactic parsing tokens.
	* @{
	*/

	/**
	* \class ObjectHandle
	* \extends IUnknownHandle
	* \brief Base class for syntactic tokens
	*/
	DECLARE_OBJECT_HANDLE(Object);

	/**
	* \class ArrayObjectHandle
	* \extends ObjectHandle
	* \brief Represents array of mixed type elements.
	*/
	DECLARE_OBJECT_HANDLE(ArrayObject);

	/**
	* \class BooleanObjectHandle
	* \extends ObjectHandle
	* \brief Represents boolean type
	*/
	DECLARE_OBJECT_HANDLE(BooleanObject);

	/**
	* \class NameObjectHandle
	* \extends ObjectHandle
	* \brief Represents unique name references in document.
	* \see NameConstants
	*/
	DECLARE_OBJECT_HANDLE(NameObject);

	/**
	* \class NullObjectHandle
	* \extends ObjectHandle
	* \brief Used as missing value.
	*/
	DECLARE_OBJECT_HANDLE(NullObject);

	/**
	* \class RealObjectHandle
	* \extends ObjectHandle
	* \brief Represents floating point numbers.
	*/
	DECLARE_OBJECT_HANDLE(RealObject);

	/**
	* \class IntegerObjectHandle
	* \extends ObjectHandle
	* \brief Represents integer values
	*/
	DECLARE_OBJECT_HANDLE(IntegerObject);

	/**
	* \class StreamObjectHandle
	* \extends ObjectHandle
	* \brief Represents compressed data inside document.
	*/
	DECLARE_OBJECT_HANDLE(StreamObject);

	/**
	* \class StringObjectHandle
	* \extends ObjectHandle
	* \brief Reprsents human readable text.
	*/
	DECLARE_OBJECT_HANDLE(StringObject);

	/**
	* \class HexadecimalStringObjectHandle
	* \extends StringObjectHandle
	* \brief Text stored in hexadecimal notation.
	*/
	DECLARE_OBJECT_HANDLE(HexadecimalStringObject);

	/**
	* \class LiteralStringObjectHandle
	* \extends StringObjectHandle
	* \brief Text stored in raw format.
	*/
	DECLARE_OBJECT_HANDLE(LiteralStringObject);

	/**
	* \class IndirectObjectReferenceHandle
	* \extends ObjectHandle
	* \brief Represents reference to other object
	*/
	DECLARE_OBJECT_HANDLE(IndirectObjectReference);

	/**
	* \class DictionaryObjectHandle
	* \extends ObjectHandle
	* \brief Represents key-value map.
	*/
	DECLARE_OBJECT_HANDLE(DictionaryObject);

	/**
	* \class DictionaryObjectIteratorHandle
	* \memberof DictionaryObjectHandle
	* \brief Used for accessing Dictionary elements
	* through iterator interface.
	*/
	DECLARE_OBJECT_HANDLE(DictionaryObjectIterator);

	/** @} */

	#pragma endregion

	#pragma region Files

	/**
	* \defgroup Files Files
	* \ingroup GotchangPDF
	* \brief Low-level file access.
	*
	* @{
	*/

	/**
	* \class FileHandle
	* \extends IUnknownHandle
	* \brief Represents file access handle
	*/
	DECLARE_OBJECT_HANDLE(File);

	/**
	* \class FileWriterHandle
	* \extends IUnknownHandle
	* \brief Implements serialization of Files to destination stream
	*/
	DECLARE_OBJECT_HANDLE(FileWriter);

	/**
	* \class XrefHandle
	* \extends IUnknownHandle
	* \brief Represents either cross-reference table or stream
	*/
	DECLARE_OBJECT_HANDLE(Xref);

	/**
	* \class XrefIteratorHandle
	* \extends IUnknownHandle
	* \brief Represents pointer to cross-reference entry collection
	*
	* This method is useful for iterating over whole entry collecion.
	* \see XrefIterator_Next
	* \see XrefIterator_GetValue
	*/
	DECLARE_OBJECT_HANDLE(XrefIterator);

	/**
	* \class XrefEntryHandle
	* \extends IUnknownHandle
	* \brief Represents base entry type within XrefHandle
	*/
	DECLARE_OBJECT_HANDLE(XrefEntry);

	/**
	* \class XrefFreeEntryHandle
	* \extends XrefEntryHandle
	* \brief Represents free entry within cross-reference section.
	*
	* Free entry means, that this object is not used in the document.
	* It can be reused in new cross-reference section.
	*/
	DECLARE_OBJECT_HANDLE(XrefFreeEntry);

	/**
	* \class XrefUsedEntryHandle
	* \extends XrefEntryHandle
	* \brief Represents used entry within cross-reference section.
	*/
	DECLARE_OBJECT_HANDLE(XrefUsedEntry);

	/**
	* \class XrefCompressedEntryHandle
	* \extends XrefEntryHandle
	* \brief Represents compressed entry within cross-reference section.
	*
	* Compressed entry means that the object is located within (7.5.7 Object streams) compressed object stream.
	* This entry type can be only found in cross-reference streams.
	*/
	DECLARE_OBJECT_HANDLE(XrefCompressedEntry);

	/**
	* \class XrefChainHandle
	* \extends IUnknownHandle
	* \brief Represents collection of XrefHandle
	*/
	DECLARE_OBJECT_HANDLE(XrefChain);

	/**
	* \class XrefChainIteratorHandle
	* \extends IUnknownHandle
	* \brief Represents pointer to cross-reference collection
	*
	* This method is useful for iterating over whole entry collecion.
	* \see XrefChainIterator_Next
	* \see XrefChainIterator_GetValue
	*/
	DECLARE_OBJECT_HANDLE(XrefChainIterator);

	/** @} */

	/**
	* \class EncryptionKeyHandle
	* \extends IUnknownHandle
	* \brief Represents encryption key other than password
	*/
	DECLARE_OBJECT_HANDLE(EncryptionKey);

	#pragma endregion

	#pragma region Contents

	/* Content instructions */
	DECLARE_OBJECT_HANDLE(ContentInstruction);
	DECLARE_OBJECT_HANDLE(ContentObject);
	DECLARE_OBJECT_HANDLE(ContentOperation);

	/* Content operations */
	DECLARE_OBJECT_HANDLE(ContentOperationGeneric);
	DECLARE_OBJECT_HANDLE(ContentOperationTextFont);
	DECLARE_OBJECT_HANDLE(ContentOperationTextShow);
	DECLARE_OBJECT_HANDLE(ContentOperationTextShowArray);
	DECLARE_OBJECT_HANDLE(ContentOperationEndText);

	/* Content operators */
	DECLARE_OBJECT_HANDLE(ContentOperator);
	DECLARE_OBJECT_HANDLE(ContentOperatorUnknown);

	/* Content objects */
	DECLARE_OBJECT_HANDLE(ContentObjectText);
	DECLARE_OBJECT_HANDLE(ContentObjectInlineImage);

	#pragma endregion

	#pragma region Utils

	/**
	* \class BufferHandle
	* \extends IUnknownHandle
	* \brief Represents memory stored data
	*/
	DECLARE_OBJECT_HANDLE(Buffer);

	#pragma endregion

	#pragma endregion

	#pragma region Documents

	/**
	* \defgroup Documents Documents
	* \ingroup GotchangPDF
	* \brief Classes representing document's properties
	* @{
	*/

	/**
	* \class DocumentHandle
	* \extends IUnknownHandle
	* \brief Represents high-level file access handle
	*/
	DECLARE_OBJECT_HANDLE(Document);
	DECLARE_OBJECT_HANDLE(Catalog);
	DECLARE_OBJECT_HANDLE(DocumentInfo);
	DECLARE_OBJECT_HANDLE(PageTree);
	DECLARE_OBJECT_HANDLE(PageObject);
	DECLARE_OBJECT_HANDLE(DeveloperExtensions);
	DECLARE_OBJECT_HANDLE(DeveloperExtensionsIterator);
	DECLARE_OBJECT_HANDLE(DeveloperExtension);
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
	DECLARE_OBJECT_HANDLE(ResourceDictionary);
	DECLARE_OBJECT_HANDLE(NamedDestinations);
	DECLARE_OBJECT_HANDLE(Destination);
	DECLARE_OBJECT_HANDLE(PageAnnotations);
	DECLARE_OBJECT_HANDLE(Annotation);
	DECLARE_OBJECT_HANDLE(LinkAnnotation);

	/* Fonts */
	DECLARE_OBJECT_HANDLE(Font);
	DECLARE_OBJECT_HANDLE(CompositeFont);
	DECLARE_OBJECT_HANDLE(FontMap);
	DECLARE_OBJECT_HANDLE(CharacterMap);
	DECLARE_OBJECT_HANDLE(EmbeddedCharacterMap);
	DECLARE_OBJECT_HANDLE(UnicodeCharacterMap);

	/* Common data structures */

	/**
	* \class DateHandle
	* \extends IUnknownHandle
	* \brief Represents a reference to a particular day represented within a calendar system
	*/
	DECLARE_OBJECT_HANDLE(Date);

	/**
	* \class RectangleHandle
	* \extends IUnknownHandle
	* \brief Represents rectangle in 2D space
	*/
	DECLARE_OBJECT_HANDLE(Rectangle);
	
	/* Contents */
	DECLARE_OBJECT_HANDLE(Contents);

	/** @} */

	#pragma endregion

#ifdef __cplusplus
};
#endif

#endif /* _C_HANDLES_H */
