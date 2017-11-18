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
	* This file contains class definitions for ContentObjectHandle
	*/

	/**
	* \class ContentObjectHandle
	* \extends ContentInstructionHandle
	* \ingroup Contents
	* \brief Composed non-atomic content object
	*/

	/**
	* \class ContentObjectTextHandle
	* \extends ContentObjectHandle
	* \ingroup Contents
	* \brief
	* A PDF text object consists of operators that may show text strings,
	* move the text position, and set text state and certain other parameters.
	*/

	/**
	* \class ContentObjectInlineImageHandle
	* \extends ContentObjectHandle
	* \ingroup Contents
	* \brief
	* As an alternative to the image XObjects described in 8.9.5,
	* "Image Dictionaries", a sampled image may be specified in
	* the form of an inline image.
	*/


	/**
	* \brief Available content object types
	*/
	typedef enum {
		/**
		* \copydoc ContentObjectTextHandle
		* \see ContentObjectTextHandle
		*/
		ContentObjectType_Text,

		/**
		* \copydoc ContentObjectInlineImageHandle
		* \see ContentObjectInlineImageHandle
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
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_GetType(ContentObjectHandle handle, ContentObjectType* result);

	/**
	* \brief Reinterpret current object as ContentObjectTextHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_ToText(ContentObjectHandle handle, ContentObjectTextHandle* result);

	/**
	* \brief Reinterpret current object as ContentObjectInlineImageHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_ToInlineImage(ContentObjectHandle handle, ContentObjectInlineImageHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_Release(ContentObjectHandle handle);

	/** @} */

	/**
	* \memberof ContentObjectTextHandle
	* @{
	*/

	/**
	* \brief Return size of a collection
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationsSize(ContentObjectTextHandle handle, size_type* result);

	/**
	* \brief Get operation at location \p at
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationAt(ContentObjectTextHandle handle, size_type at, ContentOperationHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_Release(ContentObjectTextHandle handle);

	/** @} */

	/**
	* \memberof ContentObjectInlineImageHandle
	* @{
	*/

	/**
	* \brief Get meta-data dictionary for inline image
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetDictionary(ContentObjectInlineImageHandle handle, DictionaryObjectHandle* result);

	/**
	* \brief Get raw image data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetData(ContentObjectInlineImageHandle handle, BufferHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_Release(ContentObjectInlineImageHandle handle);

	/** @} */


#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENT_OBJECT_H */
