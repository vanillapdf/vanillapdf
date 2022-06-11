#ifndef _C_CONTENT_PARSER_H
#define _C_CONTENT_PARSER_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_content_parser.h
	* \brief This file contains class definitions for \ref ContentParserHandle
	*/

	/**
	* \class ContentParserHandle
	* \extends IUnknownHandle
	* \ingroup group_contents
	* \brief
	* TODO
	*/

	/**
	* \memberof ContentParserHandle
	* @{
	*/

	/**
	* \brief Create a new instance of \ref ContentParserHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentParser_Create(FileHandle* source_file, InputStreamHandle* input_stream, ContentParserHandle** result);

	/**
	* \brief Get byte representation of content operator
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentParser_ReadInstructionCollection(ContentParserHandle* handle, ContentInstructionCollectionHandle** result);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentParser_ToUnknown(ContentParserHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref ContentParserHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentParser_FromUnknown(IUnknownHandle* handle, ContentParserHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentParser_Release(ContentParserHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENT_PARSER_H */
