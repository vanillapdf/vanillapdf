#ifndef _C_XREF_H
#define _C_XREF_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_xref.h
	* \brief This file contains class definitions for cross-reference tables, chained tables and entries.
	*/

	/**
	* \class XrefHandle
	* \extends IUnknownHandle
	* \ingroup group_xref
	* \brief Represents either cross-reference table or stream
	*/

	/**
	* \class XrefIteratorHandle
	* \extends IUnknownHandle
	* \ingroup group_xref
	* \brief Represents pointer to cross-reference entry collection
	*
	* This method is useful for iterating over whole entry collecion.
	* \see \ref XrefIterator_Next
	* \see \ref XrefIterator_GetValue
	*/

	/**
	* \class XrefEntryHandle
	* \extends IUnknownHandle
	* \ingroup group_xref
	* \brief Represents base entry type within \ref XrefHandle
	*/

	/**
	* \class XrefFreeEntryHandle
	* \extends XrefEntryHandle
	* \ingroup group_xref
	* \brief Represents free entry within cross-reference section.
	*
	* Free entry means, that this object is not used in the document.
	* It can be reused in new cross-reference section.
	*/

	/**
	* \class XrefUsedEntryHandle
	* \extends XrefEntryHandle
	* \ingroup group_xref
	* \brief Represents used entry within cross-reference section.
	*/

	/**
	* \class XrefCompressedEntryHandle
	* \extends XrefEntryHandle
	* \ingroup group_xref
	* \brief Represents compressed entry within cross-reference section.
	*
	* Compressed entry means that the object is located within (7.5.7 Object streams) compressed object stream.
	* This entry type can be only found in cross-reference streams.
	*/

	/**
	* \class XrefChainHandle
	* \extends IUnknownHandle
	* \ingroup group_xref
	* \brief Represents collection of \ref XrefHandle
	*/

	/**
	* \class XrefChainIteratorHandle
	* \extends IUnknownHandle
	* \ingroup group_xref
	* \brief Represents pointer to cross-reference collection
	*
	* This method is useful for iterating over whole entry collecion.
	* \see \ref XrefChainIterator_Next
	* \see \ref XrefChainIterator_GetValue
	*/

	/**
	* \brief Required for conversion to derived types.
	*/
	typedef enum {
		/**
		* \brief This type is not actually used.
		*
		* It represents undefined type only to distinguish from other types.
		* When an entry is created, this is the default value to mark it uninitialized entry.
		*
		* No function should return this value.
		* If it does, it is a bug.
		*
		* The reason it exists is that entry needs to have a default constructor.
		* This is only entry type that have default constructor.
		*/
		XrefEntryType_Null = 0,

		/**
		* \copybrief XrefFreeEntryHandle
		* \see \ref XrefFreeEntryHandle
		*/
		XrefEntryType_Free,

		/**
		* \copybrief XrefUsedEntryHandle
		* \see \ref XrefUsedEntryHandle
		*/
		XrefEntryType_Used,

		/**
		* \copybrief XrefCompressedEntryHandle
		* \see \ref XrefCompressedEntryHandle
		*/
		XrefEntryType_Compressed
	} XrefEntryType;

	/**
	* \memberof XrefHandle
	* @{
	*/

	/**
	* \brief Get cross-reference table meta-data dictionary
	*
	* For cross-reference tables it is the trailer dictionary after all entries.
	* For cross-reference streams it is the streams dictionary.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Xref_GetTrailerDictionary(XrefHandle* handle, DictionaryObjectHandle** result);

	/**
	* \brief Get byte offset in the decoded stream from the beginning of the file to the beginning of the xref keyword in the last cross-reference section.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Xref_GetLastXrefOffset(XrefHandle* handle, offset_type* result);

	/**
	* \brief Get cross-reference entry iterator
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Xref_Iterator(XrefHandle* handle, XrefIteratorHandle** result);

	/**
	* \brief Determine if the current iterator position is valid
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Xref_IsIteratorValid(XrefHandle* handle, XrefIteratorHandle* iterator_handle, boolean_type* result);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Xref_ToUnknown(XrefHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref XrefHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Xref_FromUnknown(IUnknownHandle* handle, XrefHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Xref_Release(XrefHandle* handle);

	/** @} */

	/**
	* \memberof XrefIteratorHandle
	* @{
	*/

	/**
	* \brief Get cross-reference entry from current iterator position
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_GetValue(XrefIteratorHandle* handle, XrefEntryHandle** result);

	/**
	* \brief Advance iterator to the next position
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_Next(XrefIteratorHandle* handle);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_ToUnknown(XrefIteratorHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref XrefIteratorHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_FromUnknown(IUnknownHandle* handle, XrefIteratorHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_Release(XrefIteratorHandle* handle);

	/** @} */

	/**
	* \memberof XrefChainHandle
	* @{
	*/

	/**
	* \brief Get cross-reference iterator
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChain_Iterator(XrefChainHandle* handle, XrefChainIteratorHandle** result);

	/**
	* \brief Determine if the current iterator position is valid
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChain_IsIteratorValid(XrefChainHandle* handle, XrefChainIteratorHandle* iterator_handle, boolean_type* result);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChain_ToUnknown(XrefChainHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref XrefChainHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChain_FromUnknown(IUnknownHandle* handle, XrefChainHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChain_Release(XrefChainHandle* handle);

	/** @} */

	/**
	* \memberof XrefChainIteratorHandle
	* @{
	*/

	/**
	* \brief Get cross-reference section from current iterator position
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_GetValue(XrefChainIteratorHandle* handle, XrefHandle** result);

	/**
	* \brief Advance iterator to the next position
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_Next(XrefChainIteratorHandle* handle);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_ToUnknown(XrefChainIteratorHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref XrefChainIteratorHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_FromUnknown(IUnknownHandle* handle, XrefChainIteratorHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_Release(XrefChainIteratorHandle* handle);

	/** @} */

	/**
	* \memberof XrefEntryHandle
	* @{
	*/

	/**
	* \brief Get entry type
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_GetType(XrefEntryHandle* handle, XrefEntryType* result);

	/**
	* \brief Get entry object number
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_GetObjectNumber(XrefEntryHandle* handle, biguint_type* result);

	/**
	* \brief Get entry generation number
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_GetGenerationNumber(XrefEntryHandle* handle, ushort_type* result);

	/**
	* \brief Quick check, if the entry is used or compressed
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_InUse(XrefEntryHandle* handle, boolean_type* result);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_ToUnknown(XrefEntryHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref XrefEntryHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_FromUnknown(IUnknownHandle* handle, XrefEntryHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle* handle);

	/** @} */

	/**
	* \memberof XrefFreeEntryHandle
	* @{
	*/

	/**
	* \copydoc XrefEntry_GetObjectNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_GetObjectNumber(XrefEntryHandle* handle, biguint_type* result);

	/**
	* \copydoc XrefEntry_GetGenerationNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_GetGenerationNumber(XrefEntryHandle* handle, ushort_type* result);

	/**
	* \copydoc XrefEntry_InUse
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_InUse(XrefFreeEntryHandle* handle, boolean_type* result);

	/**
	* \brief Object number of the next free object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_GetNextFreeObjectNumber(XrefFreeEntryHandle* handle, biguint_type* result);

	/**
	* \brief Reinterpret current object as \ref XrefEntryHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_ToEntry(XrefFreeEntryHandle* handle, XrefEntryHandle** result);

	/**
	* \brief Convert \ref XrefEntryHandle to \ref XrefFreeEntryHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_FromEntry(XrefEntryHandle* handle, XrefFreeEntryHandle** result);

	/**
	* \copydoc XrefEntry_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_Release(XrefFreeEntryHandle* handle);

	/** @} */

	/**
	* \memberof XrefUsedEntryHandle
	* @{
	*/

	/**
	* \copydoc XrefEntry_GetObjectNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_GetObjectNumber(XrefEntryHandle* handle, biguint_type* result);

	/**
	* \copydoc XrefEntry_GetGenerationNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_GetGenerationNumber(XrefEntryHandle* handle, ushort_type* result);

	/**
	* \brief Number of bytes from the beginning of the file to the beginning of the referenced object.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_GetOffset(XrefUsedEntryHandle* handle, offset_type* result);

	/**
	* \copydoc XrefEntry_InUse
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_InUse(XrefUsedEntryHandle* handle, boolean_type* result);

	/**
	* \brief Get reference to the object represented by this entry
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_GetReference(XrefUsedEntryHandle* handle, ObjectHandle** result);

	/**
	* \brief Reinterpret current object as \ref XrefEntryHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_ToEntry(XrefUsedEntryHandle* handle, XrefEntryHandle** result);

	/**
	* \brief Convert \ref XrefEntryHandle to \ref XrefUsedEntryHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_FromEntry(XrefEntryHandle* handle, XrefUsedEntryHandle** result);

	/**
	* \copydoc XrefEntry_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_Release(XrefUsedEntryHandle* handle);

	/** @} */

	/**
	* \memberof XrefCompressedEntryHandle
	* @{
	*/

	/**
	* \copydoc XrefEntry_GetObjectNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GetObjectNumber(XrefEntryHandle* handle, biguint_type* result);

	/**
	* \copydoc XrefEntry_GetGenerationNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GetGenerationNumber(XrefEntryHandle* handle, ushort_type* result);

	/**
	* \copydoc XrefEntry_InUse
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_InUse(XrefCompressedEntryHandle* handle, boolean_type* result);

	/**
	* \brief Get reference to the object represented by this entry
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GetReference(XrefCompressedEntryHandle* handle, ObjectHandle** result);

	/**
	* \brief The index of this object within the object stream.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GetIndex(XrefCompressedEntryHandle* handle, size_type* result);

	/**
	* \brief The object number of the object stream in which this object is stored.
	* (The generation number of the object stream shall be implicitly 0.)
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GetObjectStreamNumber(XrefCompressedEntryHandle* handle, biguint_type* result);

	/**
	* \brief Reinterpret current object as \ref XrefEntryHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_ToEntry(XrefCompressedEntryHandle* handle, XrefEntryHandle** result);

	/**
	* \brief Convert \ref XrefEntryHandle to \ref XrefUsedEntryHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_FromEntry(XrefEntryHandle* handle, XrefCompressedEntryHandle** result);

	/**
	* \copydoc XrefEntry_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Release(XrefCompressedEntryHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_XREF_H */
