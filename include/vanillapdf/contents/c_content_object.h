#ifndef _C_CONTENT_OBJECT_H
#define _C_CONTENT_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_content_object.h
	* \brief This file contains class definitions for \ref ContentObjectHandle
	*/

	/**
	* \class ContentObjectHandle
	* \extends ContentInstructionHandle
	* \ingroup group_contents
	* \brief A sequence of content instructions grouped within a single object.
	*/

	/**
	* \class ContentObjectTextHandle
	* \extends ContentObjectHandle
	* \ingroup group_contents
	* \brief
	* A PDF text object consists of operators that may show text strings,
	* move the text position, and set text state and certain other parameters.
	*/

	/**
	* \class ContentObjectInlineImageHandle
	* \extends ContentObjectHandle
	* \ingroup group_contents
	* \brief
	* As an alternative to the image XObjects described in
	* [section 8.9.5 - Image Dictionaries](PDF32000_2008.pdf#G7.3816169), a sampled image may be specified in
	* the form of an inline image.
	*/


	/**
	* \brief Derived types of \ref ContentObjectHandle
	* \ingroup group_contents
	*/
	typedef enum {
		ContentObjectType_Undefined = 0,
		/**
		* \copydoc ContentObjectTextHandle
		* \see \ref ContentObjectTextHandle
		*/
		ContentObjectType_Text,

		/**
		* \copydoc ContentObjectInlineImageHandle
		* \see \ref ContentObjectInlineImageHandle
		*/
		ContentObjectType_InlineImage
	} ContentObjectType;

	/**
	* \memberof ContentObjectHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_GetObjectType(ContentObjectHandle* handle, ContentObjectType* result);

	/**
	* \brief Reinterpret current object as \ref ContentObjectInlineImageHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_ToInlineImage(ContentObjectHandle* handle, ContentObjectInlineImageHandle** result);

	/**
	* \brief Reinterpret current object as \ref ContentInstructionHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_ToInstruction(ContentObjectHandle* handle, ContentInstructionHandle** result);

	/**
	* \brief Convert \ref ContentInstructionHandle to \ref ContentObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_FromInstruction(ContentInstructionHandle* handle, ContentObjectHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_Release(ContentObjectHandle* handle);

	/** @} */

	/**
	* \memberof ContentObjectTextHandle
	* @{
	*/

	/**
	* \brief Get number of operations within current text object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationsSize(ContentObjectTextHandle* handle, size_type* result);

	/**
	* \brief Get operation at location \p at
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationAt(ContentObjectTextHandle* handle, size_type at, ContentOperationHandle** result);

	/**
	* \brief Reinterpret current object as \ref ContentObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_ToContentObject(ContentObjectTextHandle* handle, ContentObjectHandle** result);

	/**
	* \brief Convert \ref ContentObjectTextHandle to \ref ContentObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_FromContentObject(ContentObjectHandle* handle, ContentObjectTextHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_Release(ContentObjectTextHandle* handle);

	/** @} */

	/**
	* \memberof ContentObjectInlineImageHandle
	* @{
	*/

	/**
	* \brief Get meta-data dictionary for inline image
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetDictionary(ContentObjectInlineImageHandle* handle, DictionaryObjectHandle** result);

	/**
	* \brief Get raw image data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetData(ContentObjectInlineImageHandle* handle, BufferHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_Release(ContentObjectInlineImageHandle* handle);

	/** @} */


#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENT_OBJECT_H */
