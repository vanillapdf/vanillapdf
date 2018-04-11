#ifndef _C_LICENSE_INFO_H
#define _C_LICENSE_INFO_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_license_info.h
	* \brief This file contains class definitions for \ref LicenseInfo
	*/

	/**
	* \class LicenseInfo
	* \ingroup group_utils
	* \brief Licensing utilities, determining whether this library is running in trial mode.
	*/

	/**
	* \memberof LicenseInfo
	* @{
	*/

	/**
	* \brief Set path to license file
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LicenseInfo_SetLicenseFile(string_type filename);

	/**
	* \brief Set content of the license file in case it is used as embedded resource
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LicenseInfo_SetLicenseBuffer(BufferHandle* data);

	/**
	* \brief Determine whether a valid license has been presented
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LicenseInfo_IsValid(boolean_type* result);

	/**
	* \brief Determine whether the current license is temporary
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LicenseInfo_IsTemporary(boolean_type* result);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_LICENSE_INFO_H */
