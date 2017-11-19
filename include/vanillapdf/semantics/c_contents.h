#ifndef _C_CONTENTS_H
#define _C_CONTENTS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_contents.h
	* This file contains class definitions for working with compressed page contents
	*/

	/**
	* \class ContentsHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* A content stream is a PDF stream object whose data consists of
	* a sequence of instructions describing the graphical elements
	* to be painted on a page.
	* \see \ref PageObjectHandle
	*/

	/**
	* \memberof ContentsHandle
	* @{
	*/

	/**
	* \brief Return size of a collection
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Contents_GetInstructionsSize(ContentsHandle* handle, size_type* result);

	/**
	* \brief Get instruction at location \p at
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Contents_GetInstructionAt(ContentsHandle* handle, size_type at, ContentInstructionHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Contents_Release(ContentsHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENTS_H */
