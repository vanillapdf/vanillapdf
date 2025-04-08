#ifndef _C_PDF_VERSION_H
#define _C_PDF_VERSION_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_pdf_version.h
    * \brief This file contains class definitions for \ref PDFVersion
    */

    /**
    * \brief Range of available PDF versions
    * \ingroup group_utils
    */
    typedef enum {

        PDFVersion_Undefined = 0,

        /**
        * \brief PDF version 1.0
        */
        PDFVersion_10,

        /**
        * \brief PDF version 1.1
        */
        PDFVersion_11,

        /**
        * \brief PDF version 1.2
        */
        PDFVersion_12,

        /**
        * \brief PDF version 1.3
        */
        PDFVersion_13,

        /**
        * \brief PDF version 1.4
        */
        PDFVersion_14,

        /**
        * \brief PDF version 1.5
        */
        PDFVersion_15,

        /**
        * \brief PDF version 1.6
        */
        PDFVersion_16,

        /**
        * \brief PDF version 1.7
        */
        PDFVersion_17,

        /**
        * \brief PDF version 2.0
        */
        PDFVersion_20,
    } PDFVersion;

#ifdef __cplusplus
};
#endif

#endif /* _C_PDF_VERSION_H */
