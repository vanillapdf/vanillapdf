#ifndef _C_FILE_WRITER_OBSERVER_H
#define _C_FILE_WRITER_OBSERVER_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_file_writer_observer.h
	* \brief This file contains class definitions for \ref FileWriterObserverHandle
	*/

	/**
	* \class FileWriterObserver
	* \extends IUnknownHandle
	* \ingroup group_files
	* \brief Allows hooking on \ref FileWriterHandle events
	*/

	/**
	* \memberof FileWriterObserver
	* @{
	*/

	/**
	* \brief The first operation of the FileWriter
	*/
	typedef error_type(*FileWriterObserver_OnInitializing_Function)(void* user_data, IInputOutputStreamHandle* output_stream);

	/**
	* \brief The last operation of the FileWriter
	*/
	typedef error_type(*FileWriterObserver_OnFinalizing_Function)(void* user_data, IInputOutputStreamHandle* output_stream);

	/**
	* \brief Invoked before every single object is written to the destination file
	*/
	typedef error_type(*FileWriterObserver_OnBeforeObjectWrite_Function)(void* user_data, ObjectHandle* object);

	/**
	* \brief Invoked after every single object is written to the destination file
	*/
	typedef error_type(*FileWriterObserver_OnAfterObjectWrite_Function)(void* user_data, ObjectHandle* object);

	/**
	* \brief Invoked before new object offset is recalculated
	*/
	typedef error_type(*FileWriterObserver_OnBeforeObjectOffsetRecalculation_Function)(void* user_data, ObjectHandle* object);

	/**
	* \brief Invoked after new object offset is recalculated
	*/
	typedef error_type(*FileWriterObserver_OnAfterObjectOffsetRecalculation_Function)(void* user_data, ObjectHandle* object);

	/**
	* \brief Invoked before xref entry offset is recalculated
	*/
	typedef error_type(*FileWriterObserver_OnBeforeEntryOffsetRecalculation_Function)(void* user_data, XrefEntryHandle* entry);

	/**
	* \brief Invoked after xref entry offset is recalculated
	*/
	typedef error_type(*FileWriterObserver_OnAfterEntryOffsetRecalculation_Function)(void* user_data, XrefEntryHandle* entry);

	/**
	* \brief Invoked before the destination stream is flushed
	*/
	typedef error_type(*FileWriterObserver_OnBeforeOutputFlush_Function)(void* user_data, IInputOutputStreamHandle* output_stream);

	/**
	* \brief Invoked after the destination stream is flushed
	*/
	typedef error_type(*FileWriterObserver_OnAfterOutputFlush_Function)(void* user_data, IInputOutputStreamHandle* output_stream);

	/**
	* \brief Create a new FileWriterObserverHandle instance with custom callbacks
	*
	* Callbacks can be set to NULL, if there is no action associated with them
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FileWriterObserver_CreateCustom(
		FileWriterObserver_OnInitializing_Function on_initializing,
		FileWriterObserver_OnFinalizing_Function on_finalizing,
		FileWriterObserver_OnBeforeObjectWrite_Function on_before_object_write,
		FileWriterObserver_OnAfterObjectWrite_Function on_after_object_write,
		FileWriterObserver_OnBeforeObjectOffsetRecalculation_Function on_before_object_offset_recalculation,
		FileWriterObserver_OnAfterObjectOffsetRecalculation_Function on_after_object_offset_recalculation,
		FileWriterObserver_OnBeforeEntryOffsetRecalculation_Function on_before_entry_offset_recalculation,
		FileWriterObserver_OnAfterEntryOffsetRecalculation_Function on_after_entry_offset_recalculation,
		FileWriterObserver_OnBeforeOutputFlush_Function on_before_output_flush,
		FileWriterObserver_OnAfterOutputFlush_Function on_after_output_flush,
		void* user_data,
		FileWriterObserverHandle** result);


	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FileWriterObserver_Release(FileWriterObserverHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FILE_WRITER_OBSERVER_H */
