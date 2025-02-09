#ifndef _C_INTERACTIVE_FORMS_H
#define _C_INTERACTIVE_FORMS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_interactive_forms.h
    * \brief This file contains class definitions for \ref InteractiveFormHandle
    */

    /**
    * \class InteractiveFormHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * An interactive form (PDF 1.2) - sometimes referred to as an AcroForm - is
    * a collection of fields for gathering information interactively from the user.
    *
    * For more details please visit [section 12.7 - Interactive Forms](PDF32000_2008.pdf#G11.2110737).
    */

    /**
    * \memberof InteractiveFormHandle
    * @{
    */

    /**
    * \brief
    * An array of references to the document's root fields.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetFields(InteractiveFormHandle* handle, FieldCollectionHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_Release(InteractiveFormHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INTERACTIVE_FORMS_H */
