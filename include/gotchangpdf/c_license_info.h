#ifndef _C_LICENSE_INFO_H
#define _C_LICENSE_INFO_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_license.h
	* This file contains class definitions for LicenseInfo
	*/

	/**
	* \class LicenseHandle
	* \ingroup Utils
	* \brief Licensing utilities, determining whether this library is running in trial mode.
	*/

	/**
	* \memberof LicenseHandle
	* @{
	*/

	/**
	* \brief Set path to license file
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LicenseInfo_SetLicenseFile(string_type filename);

	/**
	* \brief Set content of the license file in case it is used as embedded resource
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LicenseInfo_SetLicenseBuffer(BufferHandle data);

	/**
	* \brief Determine whether a valid license has been presented
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LicenseInfo_IsValid(boolean_type* result);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_LICENSE_INFO_H */
