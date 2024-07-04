#include "precompiled.h"

#include "utils/license_info.h"
#include "syntax/files/file_writer.h"

#include "vanillapdf/syntax/c_file_writer_observer.h"
#include "implementation/c_helper.h"

#include <sstream>

using namespace vanillapdf;
using namespace vanillapdf::syntax;

class CustomFileWriterObserver : public IFileWriterObserver {
public:
	CustomFileWriterObserver(
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
		void* user_data
	) : m_on_initializing(on_initializing), m_on_finalizing(on_finalizing),
		m_on_before_object_write(on_before_object_write), m_on_after_object_write(on_after_object_write),
		m_on_before_object_offset_recalculation(on_before_object_offset_recalculation), m_on_after_object_offset_recalculation(on_after_object_offset_recalculation),
		m_on_before_entry_offset_recalculation(on_before_entry_offset_recalculation), m_on_after_entry_offset_recalculation(on_after_entry_offset_recalculation),
		m_on_before_output_flush(on_before_output_flush), m_on_after_output_flush(on_after_output_flush),
		m_user_data(user_data) {

		// File writer events are licensed feature
		if (!LicenseInfo::IsValid()) {
			throw LicenseRequiredException("Custom file writer observer is a licensed feature");
		}
	}

	virtual void OnInitializing(IInputOutputStreamPtr output) override {
		auto output_ptr = reinterpret_cast<InputOutputStreamHandle*>(output.get());

		if (m_on_initializing == nullptr) {
			return;
		}

		error_type rv = m_on_initializing(m_user_data, output_ptr);
		if (rv != VANILLAPDF_ERROR_SUCCESS) {
			std::stringstream ss;
			ss << "OnInitializing operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}
	}

	virtual void OnFinalizing(IInputOutputStreamPtr output) override {
		auto output_ptr = reinterpret_cast<InputOutputStreamHandle*>(output.get());

		if (m_on_finalizing == nullptr) {
			return;
		}

		error_type rv = m_on_finalizing(m_user_data, output_ptr);
		if (rv != VANILLAPDF_ERROR_SUCCESS) {
			std::stringstream ss;
			ss << "OnFinalizing operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}
	}

	virtual void OnBeforeObjectWrite(ObjectPtr ptr) override {
		auto object_ptr = reinterpret_cast<ObjectHandle*>(ptr.get());

		if (m_on_before_object_write == nullptr) {
			return;
		}

		error_type rv = m_on_before_object_write(m_user_data, object_ptr);
		if (rv != VANILLAPDF_ERROR_SUCCESS) {
			std::stringstream ss;
			ss << "OnBeforeObjectWrite operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}
	}

	virtual void OnAfterObjectWrite(ObjectPtr ptr) override {
		auto object_ptr = reinterpret_cast<ObjectHandle*>(ptr.get());

		if (m_on_after_object_write == nullptr) {
			return;
		}

		error_type rv = m_on_after_object_write(m_user_data, object_ptr);
		if (rv != VANILLAPDF_ERROR_SUCCESS) {
			std::stringstream ss;
			ss << "OnAfterObjectWrite operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}
	}

	virtual void OnBeforeObjectOffsetRecalculation(ObjectPtr ptr) override {
		auto object_ptr = reinterpret_cast<ObjectHandle*>(ptr.get());

		if (m_on_before_object_offset_recalculation == nullptr) {
			return;
		}

		error_type rv = m_on_before_object_offset_recalculation(m_user_data, object_ptr);
		if (rv != VANILLAPDF_ERROR_SUCCESS) {
			std::stringstream ss;
			ss << "OnBeforeObjectOffsetRecalculation operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}
	}

	virtual void OnAfterObjectOffsetRecalculation(ObjectPtr ptr) override {
		auto object_ptr = reinterpret_cast<ObjectHandle*>(ptr.get());

		if (m_on_after_object_offset_recalculation == nullptr) {
			return;
		}

		error_type rv = m_on_after_object_offset_recalculation(m_user_data, object_ptr);
		if (rv != VANILLAPDF_ERROR_SUCCESS) {
			std::stringstream ss;
			ss << "OnAfterObjectOffsetRecalculation operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}
	}

	virtual void OnBeforeEntryOffsetRecalculation(XrefUsedEntryBasePtr entry) override {
		auto entry_ptr = reinterpret_cast<XrefEntryHandle*>(entry.get());

		if (m_on_before_entry_offset_recalculation == nullptr) {
			return;
		}

		error_type rv = m_on_before_entry_offset_recalculation(m_user_data, entry_ptr);
		if (rv != VANILLAPDF_ERROR_SUCCESS) {
			std::stringstream ss;
			ss << "OnBeforeEntryOffsetRecalculation operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}
	}

	virtual void OnAfterEntryOffsetRecalculation(XrefUsedEntryBasePtr entry) override {
		auto entry_ptr = reinterpret_cast<XrefEntryHandle*>(entry.get());

		if (m_on_after_entry_offset_recalculation == nullptr) {
			return;
		}

		error_type rv = m_on_after_entry_offset_recalculation(m_user_data, entry_ptr);
		if (rv != VANILLAPDF_ERROR_SUCCESS) {
			std::stringstream ss;
			ss << "OnAfterEntryOffsetRecalculation operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}
	}

	virtual void OnBeforeOutputFlush(IInputOutputStreamPtr output) override {
		auto output_ptr = reinterpret_cast<InputOutputStreamHandle*>(output.get());

		if (m_on_before_output_flush == nullptr) {
			return;
		}

		error_type rv = m_on_before_output_flush(m_user_data, output_ptr);
		if (rv != VANILLAPDF_ERROR_SUCCESS) {
			std::stringstream ss;
			ss << "OnBeforeOutputFlush operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}
	}

	virtual void OnAfterOutputFlush(IInputOutputStreamPtr output) override {
		auto output_ptr = reinterpret_cast<InputOutputStreamHandle*>(output.get());

		if (m_on_after_output_flush == nullptr) {
			return;
		}

		error_type rv = m_on_after_output_flush(m_user_data, output_ptr);
		if (rv != VANILLAPDF_ERROR_SUCCESS) {
			std::stringstream ss;
			ss << "OnAfterOutputFlush operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}
	}

private:
	FileWriterObserver_OnInitializing_Function m_on_initializing;
	FileWriterObserver_OnFinalizing_Function m_on_finalizing;

	FileWriterObserver_OnBeforeObjectWrite_Function m_on_before_object_write;
	FileWriterObserver_OnAfterObjectWrite_Function m_on_after_object_write;

	FileWriterObserver_OnBeforeObjectOffsetRecalculation_Function m_on_before_object_offset_recalculation;
	FileWriterObserver_OnAfterObjectOffsetRecalculation_Function m_on_after_object_offset_recalculation;

	FileWriterObserver_OnBeforeEntryOffsetRecalculation_Function m_on_before_entry_offset_recalculation;
	FileWriterObserver_OnAfterEntryOffsetRecalculation_Function m_on_after_entry_offset_recalculation;

	FileWriterObserver_OnBeforeOutputFlush_Function m_on_before_output_flush;
	FileWriterObserver_OnAfterOutputFlush_Function m_on_after_output_flush;

	void* m_user_data;
};

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
	FileWriterObserverHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		Deferred<CustomFileWriterObserver> observer = make_deferred<CustomFileWriterObserver>(
			on_initializing, on_finalizing,
			on_before_object_write, on_after_object_write,
			on_before_object_offset_recalculation, on_after_object_offset_recalculation,
			on_before_entry_offset_recalculation, on_after_entry_offset_recalculation,
			on_before_output_flush, on_after_output_flush, user_data);

		auto ptr = static_cast<IFileWriterObserver*>(observer.AddRefGet());
		*result = reinterpret_cast<FileWriterObserverHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FileWriterObserver_ToUnknown(FileWriterObserverHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<IFileWriterObserver, IUnknown, FileWriterObserverHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FileWriterObserver_FromUnknown(IUnknownHandle* handle, FileWriterObserverHandle** result) {
	return SafeObjectConvert<IUnknown, IFileWriterObserver, IUnknownHandle, FileWriterObserverHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FileWriterObserver_Release(FileWriterObserverHandle* handle) {
	return ObjectRelease<IFileWriterObserver, FileWriterObserverHandle>(handle);
}
