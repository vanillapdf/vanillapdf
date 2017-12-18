#ifndef _C_FONT_MAP_H
#define _C_FONT_MAP_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_font_map.h
	* \brief This file contains contains class definitions for \ref FontMapHandle
	*/

	/**
	* \class FontMapHandle
	* \extends IUnknownHandle
	* \ingroup group_fonts
	* \brief
	* A dictionary that maps resource names to font dictionaries.
	* \see \ref ResourceDictionaryHandle
	*/

	/**
	* \memberof FontMapHandle
	* @{
	*/

	/**
	* \brief Determine if collection contains key \p key_handle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Contains(FontMapHandle* handle, const NameObjectHandle* key_handle, boolean_type* result);

	/**
	* \brief Find mapped value for key \p key_handle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Find(FontMapHandle* handle, const NameObjectHandle* key_handle, FontHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Release(FontMapHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FONT_MAP_H */
