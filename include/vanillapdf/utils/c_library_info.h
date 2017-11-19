#ifndef _C_LIBRARY_INFO_H
#define _C_LIBRARY_INFO_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_library_info.h
	* This file contains definitions for \ref LibraryInfo.
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
	VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionMajor(integer_type* result);

	/**
	* \brief Get library minor version
	*
	* A change in the library minor version indicates
	* backward compatibility, while some new features were added.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionMinor(integer_type* result);

	/**
	* \brief Get library patch version
	*
	* A change in the library minor version
	* indicates no interface changes, only bugfixes.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionPatch(integer_type* result);

	/**
	* \brief Get library build version
	*
	* A change in the library build version
	* should not affect any kind of functionality.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionPatch(integer_type* result);

	/**
	* \brief Get library author name
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetAuthor(string_type* result);

	/**
	* \brief Get day of month, when the library was built
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetBuildDay(integer_type* result);

	/**
	* \brief Get month of the year, when the library was built
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetBuildMonth(integer_type* result);

	/**
	* \brief Get year, when the library was built
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetBuildYear(integer_type* result);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_LIBRARY_INFO_H */
