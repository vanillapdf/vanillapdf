#ifndef _C_APPEARANCE_CHARACTERISTICS_H
#define _C_APPEARANCE_CHARACTERISTICS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_appearance_characteristics.h
    * \brief This file contains class definitions for \ref AppearanceCharacteristicsHandle
    */

    /**
    * \class AppearanceCharacteristicsHandle
    * \extends IUnknownHandle
    * \ingroup group_annotations
    * \brief
    * The appearance characteristics dictionary (MK entry) of a widget
    * annotation, describing how the widget shall be presented when its
    * appearance streams are regenerated.
    *
    * Only a subset of the entries is modelled - the remainder is reachable
    * through \ref AppearanceCharacteristics_GetBaseObject.
    *
    * For more details please visit [Table 189 - Entries in an appearance characteristics dictionary](PDF32000_2008.pdf#G11.2093142).
    */

    /**
    * \memberof AppearanceCharacteristicsHandle
    * @{
    */

    /**
    * \brief
    * Create an empty appearance characteristics dictionary.
    *
    * The resulting dictionary is a direct object and becomes part of the
    * document once it is attached through \ref WidgetAnnotation_SetAppearanceCharacteristics.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_Create(AppearanceCharacteristicsHandle** result);

    /**
    * \brief
    * Get the number of degrees by which the widget is rotated
    * counterclockwise relative to the page (R entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetRotation(AppearanceCharacteristicsHandle* handle, bigint_type* result);

    /**
    * \brief
    * Set the number of degrees by which the widget is rotated
    * counterclockwise relative to the page (R entry).
    * \param handle a handle to the appearance characteristics dictionary
    * \param data the rotation, which shall be a multiple of 90
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetRotation(AppearanceCharacteristicsHandle* handle, bigint_type data);

    /**
    * \brief Get the colour of the widget's border (BC entry)
    *
    * A transparent colour means that no border is painted, see
    * \ref Color_GetColorSpace for the number of components.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetBorderColor(AppearanceCharacteristicsHandle* handle, ColorHandle** result);

    /**
    * \brief Set the colour of the widget's border (BC entry)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetBorderColor(AppearanceCharacteristicsHandle* handle, ColorHandle* value);

    /**
    * \brief Get the colour of the widget's background (BG entry)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetBackgroundColor(AppearanceCharacteristicsHandle* handle, ColorHandle** result);

    /**
    * \brief Set the colour of the widget's background (BG entry)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetBackgroundColor(AppearanceCharacteristicsHandle* handle, ColorHandle* value);

    /**
    * \brief Get the caption displayed when the widget is not interacting with the user (CA entry)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetNormalCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the caption displayed when the widget is not interacting with the user (CA entry)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetNormalCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the caption displayed when the cursor enters the widget (RC entry)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetRolloverCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the caption displayed when the cursor enters the widget (RC entry)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetRolloverCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the caption displayed when the mouse button is pressed within the widget (AC entry)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetDownCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the caption displayed when the mouse button is pressed within the widget (AC entry)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetDownCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle* value);

    /**
    * \brief
    * Switch object to low-level syntax API
    *
    * Useful for cases, where semantic API is not sufficient.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetBaseObject(AppearanceCharacteristicsHandle* handle, DictionaryObjectHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_ToUnknown(AppearanceCharacteristicsHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref AppearanceCharacteristicsHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_FromUnknown(IUnknownHandle* handle, AppearanceCharacteristicsHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_Release(AppearanceCharacteristicsHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_APPEARANCE_CHARACTERISTICS_H */
