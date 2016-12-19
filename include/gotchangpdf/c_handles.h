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
typedef struct x##Handle *P##x##Handle

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_handles.h
	* This file contains all type definitions returned by the library API.
	*/

	#pragma region Syntax

	#pragma region Objects

	/**
	* \defgroup Objects Objects
	* \ingroup Files
	* \brief Classes used as syntactic parsing tokens.
	*
	* @{
	*/

	/**
	* \brief Opaque pointer for accessing derived types
	*/
	DECLARE_OBJECT_HANDLE(Object);

	/** @} */

	/**
	* \defgroup ArrayObject Array object
	* \ingroup Objects
	* \brief Represents array of mixed type elements.
	*
	* @{
	*/

	/**
	* \brief Opaque pointer for accessing internal object.
	*/
	DECLARE_OBJECT_HANDLE(Array);

	/** @} */

	/**
	* \defgroup BooleanObject Boolean object
	* \ingroup Objects
	* \brief Represents boolean type
	*
	* @{
	*/

	/**
	* \brief Opaque pointer for accessing internal object.
	*/
	DECLARE_OBJECT_HANDLE(Boolean);

	/** @} */

	/**
	* \defgroup NameObject Name object
	* \ingroup Objects
	* \brief Represents unique name references in document.
	*
	* @{
	*/

	/**
	* \brief Opaque pointer for accessing internal object.
	*/
	DECLARE_OBJECT_HANDLE(Name);

	/** @} */

	/**
	* \defgroup NullObject Null object
	* \ingroup Objects
	* \brief Used as missing value
	*
	* @{
	*/

	/**
	* \brief Opaque pointer for accessing internal object.
	*/
	DECLARE_OBJECT_HANDLE(Null);

	/** @} */

	/**
	* \defgroup RealObject Real object
	* \ingroup Objects
	* \brief Represents floating point numbers
	*
	* @{
	*/

	/**
	* \brief Opaque pointer for accessing internal object.
	*/
	DECLARE_OBJECT_HANDLE(Real);

	/** @} */

	/**
	* \defgroup IntegerObject Integer object
	* \ingroup Objects
	* \brief Represents integer values
	*
	* @{
	*/

	/**
	* \brief Opaque pointer for accessing internal object.
	*/
	DECLARE_OBJECT_HANDLE(Integer);

	/** @} */

	/**
	* \defgroup StreamObject Stream object
	* \ingroup Objects
	* \brief Represents compressed data inside document
	*
	* @{
	*/

	/**
	* \brief Opaque pointer for accessing internal object.
	*/
	DECLARE_OBJECT_HANDLE(Stream);

	/** @} */

	/**
	* \defgroup StringObject String object
	* \ingroup Objects
	* \brief Reprsents human readable text
	*
	* @{
	*/

	/**
	* \brief Opaque pointer for accessing internal object.
	*/
	DECLARE_OBJECT_HANDLE(String);

	/**
	* \brief Text stored in hexadecimal notation.
	*/
	DECLARE_OBJECT_HANDLE(HexadecimalString);

	/**
	* \brief Text stored in raw format.
	*/
	DECLARE_OBJECT_HANDLE(LiteralString);

	/** @} */

	/**
	* \defgroup IndirectReferenceObject Indirect reference object
	* \ingroup Objects
	* \brief Represents reference to other object
	*
	* @{
	*/

	/**
	* \brief Opaque pointer for accessing internal object.
	*/
	DECLARE_OBJECT_HANDLE(IndirectReference);

	/** @} */

	/**
	* \defgroup DictionaryObject Dictionary object
	* \ingroup Objects
	* \brief Represents key-value map
	*
	* @{
	*/

	/**
	* \brief Opaque pointer for accessing internal object.
	*/
	DECLARE_OBJECT_HANDLE(Dictionary);

	/**
	* \brief Used for accessing Dictionary elements
	* through iterator interface.
	*/
	DECLARE_OBJECT_HANDLE(DictionaryIterator);

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
	* \brief Represents file access handle
	*/
	DECLARE_OBJECT_HANDLE(File);

	/**
	* \brief Implements serialization of Files to destination stream
	*/
	DECLARE_OBJECT_HANDLE(FileWriter);

	/** @} */

	DECLARE_OBJECT_HANDLE(XrefEntry);
	DECLARE_OBJECT_HANDLE(XrefFreeEntry);
	DECLARE_OBJECT_HANDLE(XrefUsedEntry);
	DECLARE_OBJECT_HANDLE(XrefCompressedEntry);
	DECLARE_OBJECT_HANDLE(Xref);
	DECLARE_OBJECT_HANDLE(XrefIterator);
	DECLARE_OBJECT_HANDLE(XrefChain);
	DECLARE_OBJECT_HANDLE(XrefChainIterator);

	/**
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

	DECLARE_OBJECT_HANDLE(Buffer);

	#pragma endregion

	#pragma endregion

	#pragma region Documents

	/* Document */
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
	DECLARE_OBJECT_HANDLE(Date);
	DECLARE_OBJECT_HANDLE(Rectangle);
	
	/* Contents */
	DECLARE_OBJECT_HANDLE(Contents);

	#pragma endregion

#ifdef __cplusplus
};
#endif

#endif /* _C_HANDLES_H */
