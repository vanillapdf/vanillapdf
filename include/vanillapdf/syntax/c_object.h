#ifndef _C_OBJECT_H
#define _C_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_object.h
    * \brief This file contains contains class definitions for \ref ObjectHandle
    */

    /**
    * \class ObjectHandle
    * \extends IUnknownHandle
    * \ingroup group_objects
    * \brief Base class for syntactic tokens
    */

    /**
    * \brief Derived types of \ref ObjectHandle
    * \ingroup group_objects
    */
    typedef enum {

        /**
        * \brief Undefined unitialized default value, triggers error when used
        */
        ObjectType_Undefined = 0,

        /**
        * \copybrief NullObjectHandle
        * \see \ref NullObjectHandle
        */
        ObjectType_Null,

        /**
        * \copybrief ArrayObjectHandle
        * \see \ref ArrayObjectHandle
        */
        ObjectType_Array,

        /**
        * \copybrief BooleanObjectHandle
        * \see \ref BooleanObjectHandle
        */
        ObjectType_Boolean,

        /**
        * \copybrief DictionaryObjectHandle
        * \see \ref DictionaryObjectHandle
        */
        ObjectType_Dictionary,

        /**
        * \copybrief IntegerObjectHandle
        * \see \ref IntegerObjectHandle
        */
        ObjectType_Integer,

        /**
        * \copybrief NameObjectHandle
        * \see \ref NameObjectHandle
        */
        ObjectType_Name,

        /**
        * \copybrief RealObjectHandle
        * \see \ref RealObjectHandle
        */
        ObjectType_Real,

        /**
        * \copybrief StreamObjectHandle
        * \see \ref StreamObjectHandle
        */
        ObjectType_Stream,

        /**
        * \copybrief StringObjectHandle
        * \see \ref StringObjectHandle
        */
        ObjectType_String,

        /**
        * \copybrief IndirectReferenceObjectHandle
        * \see \ref IndirectReferenceObjectHandle
        */
        ObjectType_IndirectReference
    } ObjectType;

    /**
    * \memberof ObjectHandle
    * @{
    */

    /**
    * \brief Get derived type of current object
    * \trivial
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_GetObjectType(ObjectHandle* handle, ObjectType* result);

    /**
    * \brief Get string representation of object type
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_TypeName(ObjectType type, string_type* result);

    /**
    * \brief Get input file offset where this object was found
    * \trivial
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle* handle, offset_type* result);

    /**
    * \brief Determine if the object is an indirect object
    *
    * An indirect object has an entry in the cross-reference table,
    * either directly in the file or inside a compressed object stream.
    * A direct object, such as a dictionary written inline as a value of another dictionary,
    * or an object that has not been added to a file yet, is not indirect.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_IsIndirect(ObjectHandle* handle, boolean_type* result);

    /**
    * \brief Get the object number of an indirect object
    *
    * \return \ref VANILLAPDF_ERROR_INVALID_OPERATION when the object is not indirect (see \ref Object_IsIndirect)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_GetObjectNumber(ObjectHandle* handle, biguint_type* result);

    /**
    * \brief Get the generation number of an indirect object
    *
    * \return \ref VANILLAPDF_ERROR_INVALID_OPERATION when the object is not indirect (see \ref Object_IsIndirect)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_GetGenerationNumber(ObjectHandle* handle, ushort_type* result);

    /**
    * \brief Get object attribute list attached to this PDF object
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_GetAttributeList(ObjectHandle* handle, ObjectAttributeListHandle** result);

    /**
    * \brief Compute hash value for this object
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_Hash(const ObjectHandle* handle, size_type* result);

    /**
    * \brief Convert to human readable text format
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_ToString(ObjectHandle* handle, BufferHandle** result);

    /**
    * \brief Get a PDF data representation of the current object
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_ToPdf(ObjectHandle* handle, BufferHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    * \trivial
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_ToUnknown(ObjectHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref ObjectHandle
    * \trivial
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_FromUnknown(IUnknownHandle* handle, ObjectHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_H */
