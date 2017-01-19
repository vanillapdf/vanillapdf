#ifndef _C_INTERACTIVE_FORMS_H
#define _C_INTERACTIVE_FORMS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_interactive_forms.h
	* This file contains class definitions for InteractiveFormHandle
	*/

	/**
	* \class InteractiveFormHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* An interactive form (PDF 1.2)—sometimes referred to as an AcroForm—is
	* a collection of fields for gathering information interactively from the user.
	*/

	/**
	* \memberof InteractiveFormHandle
	* @{
	*/

	GOTCHANG_PDF_API error_type CALLING_CONVENTION InteractiveForm_GetFields(InteractiveFormHandle handle, FieldCollectionHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION InteractiveForm_Release(InteractiveFormHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INTERACTIVE_FORMS_H */
