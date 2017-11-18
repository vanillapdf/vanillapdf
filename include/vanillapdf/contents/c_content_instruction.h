#ifndef _C_CONTENT_INSTRUCTION_H
#define _C_CONTENT_INSTRUCTION_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_content_instruction.h
	* This file contains class definitions for ContentInstructionHandle
	*/

	/**
	* \class ContentInstructionHandle
	* \extends IUnknownHandle
	* \ingroup Contents
	* \brief Base class for all content objects and operations.
	*/

	/**
	* \brief Available content instruction types
	*/
	typedef enum {
		/**
		* \copydoc ContentOperationHandle
		* \see ContentOperationHandle
		*/
		ContentInstructionType_Operation,

		/**
		* \copydoc ContentObjectHandle
		* \see ContentObjectHandle
		*/
		ContentInstructionType_Object
	} ContentInstructionType;

	/**
	* \memberof ContentInstructionHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_GetType(ContentInstructionHandle handle, ContentInstructionType* result);

	/**
	* \brief Reinterpret current object as ContentOperationHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_ToOperation(ContentInstructionHandle handle, ContentOperationHandle* result);

	/**
	* \brief Reinterpret current object as ContentObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_ToObject(ContentInstructionHandle handle, ContentObjectHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_Release(ContentInstructionHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENT_INSTRUCTION_H */
