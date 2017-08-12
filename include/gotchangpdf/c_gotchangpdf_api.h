#ifndef _C_GOTCHANGPDF_API_H
#define _C_GOTCHANGPDF_API_H

/**
* \file c_gotchangpdf_api.h
* This file contains the entire available API merged in to single file.
*
* Additionaly it is a host for all the documentation group declarations.
*/

/* export rules */
#include "c_export.h"

/* return values */
#include "c_values.h"

/* forward declarations */
#include "c_handles.h"

/* files */
#include "c_file.h"
#include "c_xref.h"
#include "c_encryption.h"

/* documents */
#include "c_document.h"
#include "c_document_info.h"
#include "c_document_signature_settings.h"
#include "c_catalog.h"
#include "c_page_tree.h"
#include "c_page_object.h"
#include "c_developer_extensions.h"
#include "c_page_labels.h"
#include "c_viewer_preferences.h"
#include "c_outline.h"
#include "c_destinations.h"
#include "c_annotations.h"
#include "c_resource_dictionary.h"
#include "c_interactive_forms.h"
#include "c_fields.h"
#include "c_digital_signature.h"
#include "c_message_digest_algorithm.h"

/* fonts */
#include "c_font.h"
#include "c_font_map.h"
#include "c_character_map.h"

/* common data structures */
#include "c_date.h"
#include "c_rectangle.h"

/* contents */
#include "c_contents.h"

/* objects */
#include "c_object.h"
#include "c_array_object.h"
#include "c_boolean_object.h"
#include "c_real_object.h"
#include "c_string_object.h"
#include "c_stream_object.h"
#include "c_dictionary_object.h"
#include "c_indirect_object_reference.h"
#include "c_integer_object.h"
#include "c_name_object.h"

/* utils */
#include "c_unknown_interface.h"
#include "c_buffer.h"
#include "c_logging.h"
#include "c_pdf_version.h"
#include "c_library_info.h"
#include "c_errors.h"
#include "c_pkcs12_key.h"
#include "c_signing_key.h"

/**
* \defgroup Documents High-level document interface
* \brief
* Classes representing document's properties.
*
* The base entity for should be ::DocumentHandle,
* which represents the file in terms of the high-level interface.
*
* Document's pages are contained in so-called ::CatalogHandle.
*
* These classes shall provide a root for your research.
*
* All functions in this interface shall provide semantic
* correctness for a file. Meaning that all functions
* shall behave as defined in the standard.
*
* As this task is fairly large, not all operations
* and properties are yet exposed. If you find any
* important task, that cannot be done:
* - Please let me know (<mailto:jur.zikmund@gmail.com>)
* - Try to use \ref Files to complete the task
*/

/**
* \defgroup Files Low-level file interface
* \brief
* Accessing file thourgh list of objects such as
* ArrayObjectHandle or DictionaryObjectHandle.
*
* An in-depth knowledge about PDF file format is strongly
* recommended as many functions are a direct reference
* to a PDF specification.
*
* You might want to start with ::FileHandle,
* which is the low-level counterpart of the DocumentHandle.
*
* It's primary use is for missing or misbehaving functionality
* in the \ref Documents.
*
* This interface shall provide syntactic correctness
* for a file. However, the user is responsible for
* the semantic correctness.
*
* It allows you to create files, that are syntactically
* correct, but are \b invalid in terms of PDF document
* semantics.
*/

/**
* \defgroup CommonDataStructures Common data structures
* \ingroup Documents
* \brief
* General-purpose data structures that are built from the basic
* object types described in \ref Objects.
*
* This is a direct reference to a PDF specification,
* section 7.9, "Common Data Structures".
*/

/**
* \defgroup Fonts Fonts
* \ingroup Documents
* \brief
* Group of font related classes and functions.
*
* FontHandle represents a base class for all derived font types.
*/

/**
* \defgroup Contents Page contents
* \ingroup Documents
* \brief
* Content streams are the primary means for describing
* the appearance of pages and other graphical elements.
*
* A content stream depends on information
* contained in an associated resource dictionary.
* These two objects form a self-contained entity.
*
* This is a direct reference to a PDF specification,
* section 7.8, "Content Streams and Resources".
*/

/**
* \defgroup Objects Objects
* \ingroup Files
* \brief
* Basic object types from which a whole PDF file is composed.
*
* This is a direct reference to a PDF specification,
* section 7.3, "Objects".
*/

/**
* \defgroup Xref Cross-reference tables and streams
* \ingroup Files
* \brief
* The cross-reference table contains information that permits random
* access to indirect objects within the file so that the entire file
* need not be read to locate any particular object.
*
* Cross-reference streams provide the following advantages:
* - A more compact representation of cross-reference information
* - The ability to access compressed objects that are stored in object
*   streams (see 7.5.7, "Object Streams") and to allow new cross-reference
*   entry types to be added in the future
*
* This is a direct reference to a PDF specification,
* section 7.5.4, "Cross-Reference Table" and
* section 7.5.8, "Cross-Reference Streams".
*/

/**
* \defgroup NameConstants Name constants
* \ingroup Files
* \brief
* Often used name constant definitions.
*
* These are especially useful in conjuction with the \ref Files.
*
* It allow to search PDF contents without the need to create
* a name for every property that the PDF already defines.
*/

/**
* \defgroup Utils Utilities
* \brief
* Additional features that does not correspond directly with PDF.
*/

/**
* \defgroup Types Types
* \brief
* Defines some general-purpose types used throughout the interface.
*/

/**
* \defgroup BooleanTypes Boolean types
* \ingroup Types
* \brief
* Defines boolean type with its possible values.
*/

/**
* \defgroup ErrorTypes Error types
* \ingroup Types
* \brief
* Defines a type and ranges of possible return values from whole API.
*
* Please refer to these variables when checking for function return values
* instead of any other constants (such as 0).
*
* Whole possible range of return values is described in this module.
*/

/**
* \mainpage
* This manual documents whole gotchangpdf __C API__.
*
* The library is writted in standard C++ (currently 14)
* and can be compiled using
* Visual studio
* and
* GCC (tested on Ubuntu 16.04) as well.
*
* Reason, why I have chosen the __C API__ is
* rooted in the incompatibility of the C++ ABI between compilers.
*
* In order to provide a built library, I'd rather loose
* all of the C++ API benefits and provide only __C API__.
*
* It has three dependent libraries (which can be disabled
* if desired, however it is __not recommended__):
* - OpenSSL (<https://www.openssl.org/>) - used for encrypted files
* - Zlib (<http://www.zlib.net/>) - used for compressed ::StreamObjectHandle
* - libjpeg (<http://libjpeg.sourceforge.net/>) - used for JPEG images
* (I would call this one really optinal, but I do not support disabling it)
*
* There are build scripts provided in CMake (<https://cmake.org/>) framework.
* Currently, the library dependencies are not copied to the destination folder.
* Please, do this step manually if you are trying to build from source.
* ______
*
* High-level document interface
* ------------------------
*
* For the majority of the tasks prefer using \ref Documents whener possible.
*
* Most of these functions could be used without
* an in-depth knowledge about the PDF file format.
*
* ______
*
* Low-level file interface
* ------------------------
*
* In case you find high-level interface
* insufficient, take a look at \ref Files.
*
* ______
*
* Utilities
* ------------------------
*
* Some other features that are available can be found in \ref Utils.
*
* To pick off most useful one, I would say have a look at ::Logging.
*/

#endif /* _C_GOTCHANGPDF_API_H */
