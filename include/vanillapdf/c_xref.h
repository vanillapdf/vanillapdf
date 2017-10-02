#ifndef _C_XREF_H
#define _C_XREF_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_xref.h
	* This file contains class definitions for cross-reference tables, chained tables and entries.
	*/

	/**
	* \class XrefHandle
	* \extends IUnknownHandle
	* \ingroup Xref
	* \brief Represents either cross-reference table or stream
	*/

	/**
	* \class XrefIteratorHandle
	* \extends IUnknownHandle
	* \ingroup Xref
	* \brief Represents pointer to cross-reference entry collection
	*
	* This method is useful for iterating over whole entry collecion.
	* \see XrefIterator_Next
	* \see XrefIterator_GetValue
	*/

	/**
	* \class XrefEntryHandle
	* \extends IUnknownHandle
	* \ingroup Xref
	* \brief Represents base entry type within XrefHandle
	*/

	/**
	* \class XrefFreeEntryHandle
	* \extends XrefEntryHandle
	* \ingroup Xref
	* \brief Represents free entry within cross-reference section.
	*
	* Free entry means, that this object is not used in the document.
	* It can be reused in new cross-reference section.
	*/

	/**
	* \class XrefUsedEntryHandle
	* \extends XrefEntryHandle
	* \ingroup Xref
	* \brief Represents used entry within cross-reference section.
	*/

	/**
	* \class XrefCompressedEntryHandle
	* \extends XrefEntryHandle
	* \ingroup Xref
	* \brief Represents compressed entry within cross-reference section.
	*
	* Compressed entry means that the object is located within (7.5.7 Object streams) compressed object stream.
	* This entry type can be only found in cross-reference streams.
	*/

	/**
	* \class XrefChainHandle
	* \extends IUnknownHandle
	* \ingroup Xref
	* \brief Represents collection of XrefHandle
	*/

	/**
	* \class XrefChainIteratorHandle
	* \extends IUnknownHandle
	* \ingroup Xref
	* \brief Represents pointer to cross-reference collection
	*
	* This method is useful for iterating over whole entry collecion.
	* \see XrefChainIterator_Next
	* \see XrefChainIterator_GetValue
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
		* \see XrefFreeEntryHandle
		*/
		XrefEntryType_Free,

		/**
		* \copybrief XrefUsedEntryHandle
		* \see XrefUsedEntryHandle
		*/
		XrefEntryType_Used,

		/**
		* \copybrief XrefCompressedEntryHandle
		* \see XrefCompressedEntryHandle
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
	VANILLAPDF_API error_type CALLING_CONVENTION Xref_TrailerDictionary(XrefHandle handle, DictionaryObjectHandle* result);

	/**
	* \brief Get byte offset in the decoded stream from the beginning of the file to the beginning of the xref keyword in the last cross-reference section.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Xref_LastXrefOffset(XrefHandle handle, offset_type* result);

	/**
	* \brief Get cross-reference entry iterator
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Xref_Iterator(XrefHandle handle, XrefIteratorHandle* result);

	/**
	* \copydoc IUnknown_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Xref_Release(XrefHandle handle);

	/** @} */

	/**
	* \memberof XrefIteratorHandle
	* @{
	*/

	/**
	* \brief Get cross-reference entry from current iterator position
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_GetValue(XrefIteratorHandle handle, XrefEntryHandle* result);

	/**
	* \brief Determine if the current iterator position is valid
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_IsValid(XrefIteratorHandle handle, XrefHandle xref, boolean_type* result);

	/**
	* \brief Advance iterator to the next position
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_Next(XrefIteratorHandle handle);

	/**
	* \copydoc IUnknown_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefIterator_Release(XrefIteratorHandle handle);

	/** @} */

	/**
	* \memberof XrefChainHandle
	* @{
	*/

	/**
	* \brief Get cross-reference iterator
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChain_Iterator(XrefChainHandle handle, XrefChainIteratorHandle* result);

	/**
	* \copydoc IUnknown_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChain_Release(XrefChainHandle handle);

	/** @} */

	/**
	* \memberof XrefChainIteratorHandle
	* @{
	*/

	/**
	* \brief Get cross-reference section from current iterator position
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_GetValue(XrefChainIteratorHandle handle, XrefHandle* result);

	/**
	* \brief Determine if the current iterator position is valid
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_IsValid(XrefChainIteratorHandle handle, XrefChainHandle chain, boolean_type* result);

	/**
	* \brief Advance iterator to the next position
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_Next(XrefChainIteratorHandle handle);

	/**
	* \copydoc IUnknown_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefChainIterator_Release(XrefChainIteratorHandle handle);

	/** @} */

	/**
	* \memberof XrefEntryHandle
	* @{
	*/

	/**
	* \brief Get entry type
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_Type(XrefEntryHandle handle, XrefEntryType* result);

	/**
	* \brief Get entry object number
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_ObjectNumber(XrefEntryHandle handle, biguint_type* result);

	/**
	* \brief Get entry generation number
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_GenerationNumber(XrefEntryHandle handle, ushort_type* result);

	/**
	* \brief Quick check, if the entry is used or compressed
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_InUse(XrefEntryHandle handle, boolean_type* result);

	/**
	* \copydoc IUnknown_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle);

	/**
	* \brief Reinterpret entry as XrefFreeEntryHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_ToFreeEntry(XrefEntryHandle handle, XrefFreeEntryHandle* result);

	/**
	* \brief Reinterpret entry as XrefUsedEntryHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_ToUsedEntry(XrefEntryHandle handle, XrefUsedEntryHandle* result);

	/**
	* \brief Reinterpret entry as XrefCompressedEntryHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefEntry_ToCompressedEntry(XrefEntryHandle handle, XrefCompressedEntryHandle* result);

	/** @} */

	/**
	* \memberof XrefFreeEntryHandle
	* @{
	*/

	/**
	* \copydoc XrefEntryHandle::XrefEntry_ObjectNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_ObjectNumber(XrefEntryHandle handle, biguint_type* result);

	/**
	* \copydoc XrefEntryHandle::XrefEntry_GenerationNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_GenerationNumber(XrefEntryHandle handle, ushort_type* result);

	/**
	* \copydoc XrefEntryHandle::XrefEntry_InUse
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_InUse(XrefFreeEntryHandle handle, boolean_type* result);

	/**
	* \brief Object number of the next free object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_NextFreeObjectNumber(XrefFreeEntryHandle handle, biguint_type* result);

	/**
	* \copydoc IUnknown_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefFreeEntry_Release(XrefFreeEntryHandle handle);

	/** @} */

	/**
	* \memberof XrefUsedEntryHandle
	* @{
	*/

	/**
	* \copydoc XrefEntryHandle::XrefEntry_ObjectNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_ObjectNumber(XrefEntryHandle handle, biguint_type* result);

	/**
	* \copydoc XrefEntryHandle::XrefEntry_GenerationNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_GenerationNumber(XrefEntryHandle handle, ushort_type* result);

	/**
	* \brief Number of bytes from the beginning of the file to the beginning of the referenced object.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_Offset(XrefUsedEntryHandle handle, offset_type* result);

	/**
	* \copydoc XrefEntryHandle::XrefEntry_InUse
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_InUse(XrefUsedEntryHandle handle, boolean_type* result);

	/**
	* \brief Get reference to the object represented by this entry
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_Reference(XrefUsedEntryHandle handle, ObjectHandle* result);

	/**
	* \copydoc IUnknown_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefUsedEntry_Release(XrefUsedEntryHandle handle);

	/** @} */

	/**
	* \memberof XrefCompressedEntryHandle
	* @{
	*/

	/**
	* \copydoc XrefEntryHandle::XrefEntry_ObjectNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_ObjectNumber(XrefEntryHandle handle, biguint_type* result);

	/**
	* \copydoc XrefEntryHandle::XrefEntry_GenerationNumber
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GenerationNumber(XrefEntryHandle handle, ushort_type* result);

	/**
	* \copydoc XrefEntryHandle::XrefEntry_InUse
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_InUse(XrefCompressedEntryHandle handle, boolean_type* result);

	/**
	* \brief Get reference to the object represented by this entry
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Reference(XrefCompressedEntryHandle handle, ObjectHandle* result);

	/**
	* \brief The index of this object within the object stream.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Index(XrefCompressedEntryHandle handle, size_type* result);

	/**
	* \brief The object number of the object stream in which this object is stored.
	* (The generation number of the object stream shall be implicitly 0.)
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_ObjectStreamNumber(XrefCompressedEntryHandle handle, biguint_type* result);

	/**
	* \copydoc IUnknown_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Release(XrefCompressedEntryHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_XREF_H */
