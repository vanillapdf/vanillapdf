#ifndef _C_LIBRARY_INFO_H
#define _C_LIBRARY_INFO_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_library_info.h
	* This file contains definitions for LibraryInfo.
	*/

	/**
	* \class LibraryInfo
	* \ingroup Utils
	* \brief
	* Informations about library version and author.
	*/

	/**
	* \memberof LibraryInfo
	* @{
	*/

	/**
	* \brief Get library major version.
	*
	* A change in the library major version indicates backward incompatibility.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionMajor(out_integer_type result);

	/**
	* \brief Get library minor version
	*
	* A change in the library minor version indicates
	* backward compatibility, while some new features were added.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionMinor(out_integer_type result);

	/**
	* \brief Get library patch version
	*
	* A change in the library minor version
	* indicates no interface changes, only bugfixes.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionPatch(out_integer_type result);

	/**
	* \brief Get library author name
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetAuthor(out_string_type result);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_LIBRARY_INFO_H */
