#ifndef _C_ACTIONS_H
#define _C_ACTIONS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_actions.h
    * \brief This file contains functions for working with PDF actions.
    */

    /**
    * \class ActionHandle
    * \extends IUnknownHandle
    * \ingroup group_actions
    * \brief
    * An action to be performed when a link annotation, outline item,
    * or other trigger is activated.
    */

    /**
    * \class GoToActionHandle
    * \extends ActionHandle
    * \ingroup group_actions
    * \brief
    * A go-to action changes the view to a specified destination
    * within the same document.
    */

    /**
    * \class URIActionHandle
    * \extends ActionHandle
    * \ingroup group_actions
    * \brief
    * A URI action causes a URI to be resolved.
    */

    /**
    * \class GoToRemoteActionHandle
    * \extends ActionHandle
    * \ingroup group_actions
    * \brief
    * A remote go-to action is similar to a go-to action
    * but jumps to a destination in another PDF file.
    */

    /**
    * \class NamedActionHandle
    * \extends ActionHandle
    * \ingroup group_actions
    * \brief
    * A named action provides a way to execute a predefined
    * action such as NextPage, PrevPage, FirstPage, or LastPage.
    */

    /**
    * \brief Derived types of \ref ActionHandle
    * \ingroup group_actions
    */
    typedef enum {

        /**
        * \brief Undefined unitialized default value, triggers error when used
        */
        ActionType_Undefined = 0,

        /**
        * A go-to action changes the view to a specified destination
        * within the same document.
        */
        ActionType_GoTo,

        /**
        * A remote go-to action jumps to a destination in another PDF file.
        */
        ActionType_GoToRemote,

        /**
        * A URI action causes a URI to be resolved.
        */
        ActionType_URI,

        /**
        * A launch action launches an application or opens a document.
        */
        ActionType_Launch,

        /**
        * A named action executes a predefined action.
        */
        ActionType_Named,

        /**
        * A JavaScript action causes a script to be compiled and executed.
        */
        ActionType_JavaScript
    } ActionType;

    /**
    * \memberof ActionHandle
    * @{
    */

    /**
    * \brief Get action type of object \p handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Action_GetActionType(ActionHandle* handle, ActionType* result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Action_ToUnknown(ActionHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref ActionHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Action_FromUnknown(IUnknownHandle* handle, ActionHandle** result);

    /**
    * \brief Create an \ref ActionHandle from a \ref DictionaryObjectHandle.
    *
    * The dictionary should contain a valid action type (S entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Action_CreateFromDictionary(DictionaryObjectHandle* handle, ActionHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Action_Release(ActionHandle* handle);

    /** @} */

    /**
    * \memberof GoToActionHandle
    * @{
    */

    /**
    * \brief A destination to be displayed when this action is performed
    */
    VANILLAPDF_API error_type CALLING_CONVENTION GoToAction_GetDestination(GoToActionHandle* handle, DestinationHandle** result);

    /**
    * \brief Reinterpret current object as \ref ActionHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION GoToAction_ToAction(GoToActionHandle* handle, ActionHandle** result);

    /**
    * \brief Convert \ref ActionHandle to \ref GoToActionHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION GoToAction_FromAction(ActionHandle* handle, GoToActionHandle** result);

    /**
    * \copydoc Action_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION GoToAction_Release(GoToActionHandle* handle);

    /** @} */

    /**
    * \memberof URIActionHandle
    * @{
    */

    /**
    * \brief The uniform resource identifier to resolve
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION URIAction_GetURI(URIActionHandle* handle, LiteralStringObjectHandle** result);

    /**
    * \brief Get the URI as a \ref PdfTextStringHandle (encoding-aware).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION URIAction_GetURIText(URIActionHandle* handle, PdfTextStringHandle** result);

    /**
    * \brief Reinterpret current object as \ref ActionHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION URIAction_ToAction(URIActionHandle* handle, ActionHandle** result);

    /**
    * \brief Convert \ref ActionHandle to \ref URIActionHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION URIAction_FromAction(ActionHandle* handle, URIActionHandle** result);

    /**
    * \copydoc Action_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION URIAction_Release(URIActionHandle* handle);

    /** @} */

    /**
    * \memberof GoToRemoteActionHandle
    * @{
    */

    /**
    * \brief A destination to be displayed when this action is performed
    */
    VANILLAPDF_API error_type CALLING_CONVENTION GoToRemoteAction_GetDestination(GoToRemoteActionHandle* handle, DestinationHandle** result);

    /**
    * \brief The file in which the destination shall be found
    */
    VANILLAPDF_API error_type CALLING_CONVENTION GoToRemoteAction_GetFile(GoToRemoteActionHandle* handle, ObjectHandle** result);

    /**
    * \brief Reinterpret current object as \ref ActionHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION GoToRemoteAction_ToAction(GoToRemoteActionHandle* handle, ActionHandle** result);

    /**
    * \brief Convert \ref ActionHandle to \ref GoToRemoteActionHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION GoToRemoteAction_FromAction(ActionHandle* handle, GoToRemoteActionHandle** result);

    /**
    * \copydoc Action_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION GoToRemoteAction_Release(GoToRemoteActionHandle* handle);

    /** @} */

    /**
    * \memberof NamedActionHandle
    * @{
    */

    /**
    * \brief The name of the action that shall be performed
    */
    VANILLAPDF_API error_type CALLING_CONVENTION NamedAction_GetName(NamedActionHandle* handle, NameObjectHandle** result);

    /**
    * \brief Reinterpret current object as \ref ActionHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION NamedAction_ToAction(NamedActionHandle* handle, ActionHandle** result);

    /**
    * \brief Convert \ref ActionHandle to \ref NamedActionHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION NamedAction_FromAction(ActionHandle* handle, NamedActionHandle** result);

    /**
    * \copydoc Action_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION NamedAction_Release(NamedActionHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_ACTIONS_H */
