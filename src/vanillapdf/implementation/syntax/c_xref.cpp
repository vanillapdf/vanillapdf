#include "precompiled.h"

#include "syntax/files/xref.h"
#include "syntax/files/xref_chain.h"

#include "vanillapdf/syntax/c_xref.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION Xref_Create(XrefHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		XrefTablePtr table;
		auto ptr = table.AddRefGet();
		*result = reinterpret_cast<XrefHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Xref_Insert(XrefHandle* handle, XrefEntryHandle* entry) {
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	XrefEntryBase* entry_base = reinterpret_cast<XrefEntryBase*>(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry_base);

	try
	{
		xref->Add(entry_base);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Xref_Contains(XrefHandle* handle, biguint_type object_number, boolean_type* result) {
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = xref->Contains(object_number);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Xref_Find(XrefHandle* handle, biguint_type object_number, XrefEntryHandle** result) {
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto entry = xref->Find(object_number);
		auto ptr = entry.AddRefGet();
		*result = reinterpret_cast<XrefEntryHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Xref_Remove(XrefHandle* handle, biguint_type object_number, boolean_type* result) {
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		// We do not have a better interface for the time being
		XrefFreeEntryPtr temp = make_deferred<XrefFreeEntry>(object_number, static_cast<types::ushort>(0));

		*result = xref->Remove(temp);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Xref_Clear(XrefHandle* handle) {
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);

	xref->Clear();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Xref_GetTrailerDictionary(XrefHandle* handle, DictionaryObjectHandle** result)
{
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto dict = xref->GetTrailerDictionary();
		auto ptr = dict.AddRefGet();
		*result = reinterpret_cast<DictionaryObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Xref_GetLastXrefOffset(XrefHandle* handle, offset_type* result)
{
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = xref->GetLastXrefOffset();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Xref_GetIterator(XrefHandle* handle, XrefIteratorHandle** result)
{
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto begin = xref->Begin();
		auto ptr = begin.AddRefGet();
		*result = reinterpret_cast<XrefIteratorHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_GetValue(XrefIteratorHandle* handle, XrefEntryHandle** result)
{
	XrefBase::Iterator* iterator = reinterpret_cast<XrefBase::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto entry = iterator->Value();
		auto ptr = entry.AddRefGet();
		*result = reinterpret_cast<XrefEntryHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_Next(XrefIteratorHandle* handle)
{
	XrefBase::Iterator* iterator = reinterpret_cast<XrefBase::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);

	try {
		++(*iterator);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_IsValid(XrefIteratorHandle* handle, boolean_type* result) {
	XrefBase::Iterator* iterator = reinterpret_cast<XrefBase::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		if (iterator->IsValid()) {
			*result = VANILLAPDF_RV_TRUE;
		} else {
			*result = VANILLAPDF_RV_FALSE;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_ToUnknown(XrefIteratorHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<XrefBase::Iterator, IUnknown, XrefIteratorHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_FromUnknown(IUnknownHandle* handle, XrefIteratorHandle** result) {
	return SafeObjectConvert<IUnknown, XrefBase::Iterator, IUnknownHandle, XrefIteratorHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_Release(XrefIteratorHandle* handle)
{
	return ObjectRelease<XrefBase::Iterator, XrefIteratorHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION Xref_ToUnknown(XrefHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<XrefBase, IUnknown, XrefHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Xref_FromUnknown(IUnknownHandle* handle, XrefHandle** result) {
	return SafeObjectConvert<IUnknown, XrefBase, IUnknownHandle, XrefHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Xref_Release(XrefHandle* handle)
{
	return ObjectRelease<XrefBase, XrefHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_ToUnknown(XrefEntryHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<XrefEntryBase, IUnknown, XrefEntryHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_FromUnknown(IUnknownHandle* handle, XrefEntryHandle** result) {
	return SafeObjectConvert<IUnknown, XrefEntryBase, IUnknownHandle, XrefEntryHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle* handle)
{
	return ObjectRelease<XrefEntryBase, XrefEntryHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_InUse(XrefEntryHandle* handle, boolean_type* result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	if (entry->InUse())
		*result = VANILLAPDF_RV_TRUE;
	else
		*result = VANILLAPDF_RV_FALSE;

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_GetType(XrefEntryHandle* handle, XrefEntryType* result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	// I do not use Null entry intentionally - it is only for default construction
	// It might be something going on
	assert(entry->GetUsage() != XrefEntryBase::Usage::Null);

	switch (entry->GetUsage()) {
	case XrefEntryBase::Usage::Free:
		*result = XrefEntryType_Free; break;
	case XrefEntryBase::Usage::Used:
		*result = XrefEntryType_Used; break;
	case XrefEntryBase::Usage::Compressed:
		*result = XrefEntryType_Compressed; break;
	case XrefEntryBase::Usage::Null:
		*result = XrefEntryType_Null; break;
	default:
		return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_GetObjectNumber(XrefEntryHandle* handle, biguint_type* result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetObjectNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_GetGenerationNumber(XrefEntryHandle* handle, ushort_type* result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetGenerationNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}

// Free entry

VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_Create(biguint_type object_number, ushort_type generation_number, biguint_type next_free_object, XrefFreeEntryHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto entry = make_deferred<XrefFreeEntry>(object_number, generation_number, next_free_object);
		auto ptr = entry.AddRefGet();
		*result = reinterpret_cast<XrefFreeEntryHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_GetObjectNumber(XrefFreeEntryHandle* handle, biguint_type* result)
{
	XrefFreeEntry* entry = reinterpret_cast<XrefFreeEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetObjectNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_GetGenerationNumber(XrefFreeEntryHandle* handle, ushort_type* result)
{
	XrefFreeEntry* entry = reinterpret_cast<XrefFreeEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetGenerationNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_InUse(XrefFreeEntryHandle* handle, boolean_type* result)
{
	XrefFreeEntry* entry = reinterpret_cast<XrefFreeEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->InUse();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_GetNextFreeObjectNumber(XrefFreeEntryHandle* handle, biguint_type* result)
{
	XrefFreeEntry* entry = reinterpret_cast<XrefFreeEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetNextFreeObjectNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_ToEntry(XrefFreeEntryHandle* handle, XrefEntryHandle** result) {
	return SafeObjectConvert<XrefFreeEntry, XrefEntryBase, XrefFreeEntryHandle, XrefEntryHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_FromEntry(XrefEntryHandle* handle, XrefFreeEntryHandle** result) {
	return SafeObjectConvert<XrefEntryBase, XrefFreeEntry, XrefEntryHandle, XrefFreeEntryHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_Release(XrefFreeEntryHandle* handle)
{
	return ObjectRelease<XrefFreeEntry, XrefFreeEntryHandle>(handle);
}

// Used entry

VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_Create(biguint_type object_number, ushort_type generation_number, offset_type offset, XrefUsedEntryHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto entry = make_deferred<XrefUsedEntry>(object_number, generation_number, offset);
		auto ptr = entry.AddRefGet();
		*result = reinterpret_cast<XrefUsedEntryHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_GetObjectNumber(XrefUsedEntryHandle* handle, biguint_type* result)
{
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetObjectNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_GetGenerationNumber(XrefUsedEntryHandle* handle, ushort_type* result)
{
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetGenerationNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_GetOffset(XrefUsedEntryHandle* handle, offset_type* result)
{
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetOffset();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_InUse(XrefUsedEntryHandle* handle, boolean_type* result)
{
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->InUse();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_ToEntry(XrefUsedEntryHandle* handle, XrefEntryHandle** result) {
	return SafeObjectConvert<XrefUsedEntry, XrefEntryBase, XrefUsedEntryHandle, XrefEntryHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_FromEntry(XrefEntryHandle* handle, XrefUsedEntryHandle** result) {
	return SafeObjectConvert<XrefEntryBase, XrefUsedEntry, XrefEntryHandle, XrefUsedEntryHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_Release(XrefUsedEntryHandle* handle)
{
	return ObjectRelease<XrefUsedEntry, XrefUsedEntryHandle>(handle);
}

// Compressed entry

VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Create(biguint_type object_number, ushort_type generation_number, biguint_type object_stream_number, size_type index, XrefCompressedEntryHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto entry = make_deferred<XrefCompressedEntry>(object_number, generation_number, object_stream_number, index);
		auto ptr = entry.AddRefGet();
		*result = reinterpret_cast<XrefCompressedEntryHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GetObjectNumber(XrefCompressedEntryHandle* handle, biguint_type* result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetObjectNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GetGenerationNumber(XrefCompressedEntryHandle* handle, ushort_type* result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetGenerationNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_InUse(XrefCompressedEntryHandle* handle, boolean_type* result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->InUse();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GetIndex(XrefCompressedEntryHandle* handle, size_type* result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetIndex();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GetObjectStreamNumber(XrefCompressedEntryHandle* handle, biguint_type* result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetObjectStreamNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_ToEntry(XrefCompressedEntryHandle* handle, XrefEntryHandle** result) {
	return SafeObjectConvert<XrefCompressedEntry, XrefEntryBase, XrefCompressedEntryHandle, XrefEntryHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_FromEntry(XrefEntryHandle* handle, XrefCompressedEntryHandle** result) {
	return SafeObjectConvert<XrefEntryBase, XrefCompressedEntry, XrefEntryHandle, XrefCompressedEntryHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Release(XrefCompressedEntryHandle* handle)
{
	return ObjectRelease<XrefCompressedEntry, XrefCompressedEntryHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_GetReference(XrefUsedEntryHandle* handle, ObjectHandle** result)
{
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto direct = entry->GetReference();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<ObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_SetReference(XrefUsedEntryHandle* handle, ObjectHandle* data) {
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	Object* obj = reinterpret_cast<Object*>(data);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try {
		entry->SetReference(obj);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GetReference(XrefCompressedEntryHandle* handle, ObjectHandle** result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto direct = entry->GetReference();

		auto base = ObjectUtils::GetObjectBase(direct);
		auto ptr = base.AddRefGet();
		*result = reinterpret_cast<ObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_SetReference(XrefCompressedEntryHandle* handle, ObjectHandle* data) {
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	Object* obj = reinterpret_cast<Object*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try {
		entry->SetReference(obj);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_GetValue(XrefChainIteratorHandle* handle, XrefHandle** result)
{
	XrefChain::Iterator* iterator = reinterpret_cast<XrefChain::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto entry = iterator->Value();
		auto ptr = entry.AddRefGet();
		*result = reinterpret_cast<XrefHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_Next(XrefChainIteratorHandle* handle)
{
	XrefChain::Iterator* iterator = reinterpret_cast<XrefChain::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);

	try {
		++(*iterator);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_IsValid(XrefChainIteratorHandle* handle, boolean_type* result) {
	XrefChain::Iterator* iterator = reinterpret_cast<XrefChain::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		if (iterator->IsValid()) {
			*result = VANILLAPDF_RV_TRUE;
		} else {
			*result = VANILLAPDF_RV_FALSE;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_ToUnknown(XrefChainIteratorHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<XrefChain::Iterator, IUnknown, XrefChainIteratorHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_FromUnknown(IUnknownHandle* handle, XrefChainIteratorHandle** result) {
	return SafeObjectConvert<IUnknown, XrefChain::Iterator, IUnknownHandle, XrefChainIteratorHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_Release(XrefChainIteratorHandle* handle)
{
	return ObjectRelease<XrefChain::Iterator, XrefChainIteratorHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefChain_GetIterator(XrefChainHandle* handle, XrefChainIteratorHandle** result)
{
	XrefChain* chain = reinterpret_cast<XrefChain*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(chain);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto begin = chain->Begin();
		auto ptr = begin.AddRefGet();
		*result = reinterpret_cast<XrefChainIteratorHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefChain_ToUnknown(XrefChainHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<XrefChain, IUnknown, XrefChainHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefChain_FromUnknown(IUnknownHandle* handle, XrefChainHandle** result) {
	return SafeObjectConvert<IUnknown, XrefChain, IUnknownHandle, XrefChainHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XrefChain_Release(XrefChainHandle* handle)
{
	return ObjectRelease<XrefChain, XrefChainHandle>(handle);
}
