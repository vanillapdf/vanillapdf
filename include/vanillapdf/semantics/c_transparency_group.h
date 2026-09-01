#ifndef _C_TRANSPARENCY_GROUP_H
#define _C_TRANSPARENCY_GROUP_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_transparency_group.h
    * \brief This file contains class definitions for \ref TransparencyGroupHandle
    */

    /**
    * \class TransparencyGroupHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * The group attributes dictionary (Group entry) of a page or a form XObject,
    * describing how the group's contents are composited in the transparent
    * imaging model.
    *
    * The group colour space (CS entry) is not modelled yet - it is reachable
    * through \ref TransparencyGroup_GetBaseObject.
    *
    * For more details please visit [Table 147 - Additional entries specific to a transparency group attributes dictionary](PDF32000_2008.pdf#G9.4867965).
    */

    /**
    * \memberof TransparencyGroupHandle
    * @{
    */

    /**
    * \brief
    * Create an empty transparency group dictionary with the mandatory
    * /S /Transparency subtype.
    *
    * The resulting dictionary is a direct object and becomes part of the
    * document once it is attached through \ref PageObject_SetGroup or
    * \ref FormXObject_SetGroup.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_Create(TransparencyGroupHandle** result);

    /**
    * \brief
    * Get whether the group is isolated (I entry).
    *
    * An isolated group is composited against a fully transparent backdrop
    * rather than the backdrop of the group it is painted into.
    * When absent, the group is not isolated.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_GetIsolated(TransparencyGroupHandle* handle, boolean_type* result);

    /**
    * \brief Set whether the group is isolated (I entry)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_SetIsolated(TransparencyGroupHandle* handle, boolean_type value);

    /**
    * \brief
    * Get whether the group is a knockout group (K entry).
    *
    * In a knockout group every element is composited with the group's
    * initial backdrop, overriding the elements painted before it.
    * When absent, the group is not a knockout group.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_GetKnockout(TransparencyGroupHandle* handle, boolean_type* result);

    /**
    * \brief Set whether the group is a knockout group (K entry)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_SetKnockout(TransparencyGroupHandle* handle, boolean_type value);

    /**
    * \brief
    * Switch object to low-level syntax API
    *
    * Useful for cases, where semantic API is not sufficient.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_GetBaseObject(TransparencyGroupHandle* handle, DictionaryObjectHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_ToUnknown(TransparencyGroupHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref TransparencyGroupHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_FromUnknown(IUnknownHandle* handle, TransparencyGroupHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_Release(TransparencyGroupHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_TRANSPARENCY_GROUP_H */
