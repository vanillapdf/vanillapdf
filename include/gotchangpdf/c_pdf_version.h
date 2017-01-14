#ifndef _C_PDF_VERSION_H
#define _C_PDF_VERSION_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_pdf_version.h
	* This file contains class definitions for PDFVersion
	*/

	/**
	* \brief Range of available PDF versions
	*/
	typedef enum {
		/**
		* \brief PDF version 1.0
		*/
		PDFVersion_10 = 0,

		/**
		* \brief PDF version 1.1
		*/
		PDFVersion_11 = 1,

		/**
		* \brief PDF version 1.2
		*/
		PDFVersion_12 = 2,

		/**
		* \brief PDF version 1.3
		*/
		PDFVersion_13 = 3,

		/**
		* \brief PDF version 1.4
		*/
		PDFVersion_14 = 4,

		/**
		* \brief PDF version 1.5
		*/
		PDFVersion_15 = 5,

		/**
		* \brief PDF version 1.6
		*/
		PDFVersion_16 = 6,

		/**
		* \brief PDF version 1.7
		*/
		PDFVersion_17 = 7
	} PDFVersion;

#ifdef __cplusplus
};
#endif

#endif /* _C_PDF_VERSION_H */
