#ifndef _SYNTAX_FWD_H
#define _SYNTAX_FWD_H

#include "utils/utils_fwd.h"
#include "utils/deferred.h"

namespace gotchangpdf
{
	namespace syntax
	{
		// Exceptions
		class FileDisposedException;
		class InvalidObjectTypeException;

		// Filters
		class FilterBase; using FilterBasePtr = Deferred<FilterBase>;		
		class ASCII85DecodeFilter; using ASCII85DecodeFilterPtr = Deferred<ASCII85DecodeFilter>;
		class ASCIIHexDecodeFilter; using ASCIIHexDecodeFilterPtr = Deferred<ASCIIHexDecodeFilter>;
		class FlateDecodeFilter; using FlateDecodeFilterPtr = Deferred<FlateDecodeFilter>;
		class DCTDecodeFilter; using DCTDecodeFilterPtr = Deferred<DCTDecodeFilter>;

		// Files
		class File;
		class FileWriter; using FileWriterPtr = Deferred<FileWriter>;
		class FileHolder; using FileHolderPtr = Deferred<FileHolder>;
		class Header;

		class XrefBase; using XrefBasePtr = Deferred<XrefBase>;
		class XrefSubsection;
		class XrefTable;
		class XrefStream;
		class XrefChain;

		class XrefEntryBase;
		class XrefUsedEntryBase;
		class XrefNullEntry;
		class XrefUsedEntry;
		class XrefFreeEntry;
		class XrefCompressedEntry;

		using HeaderPtr = Deferred<Header>;

		using XrefTablePtr = Deferred<XrefTable>;
		using XrefStreamPtr = Deferred<XrefStream>;

		using XrefChainPtr = Deferred<XrefChain>;
		using XrefEntryBasePtr = Deferred<XrefEntryBase>;
		using XrefUsedEntryBasePtr = Deferred<XrefUsedEntryBase>;
		using XrefNullEntryPtr = Deferred<XrefNullEntry>;
		using XrefFreeEntryPtr = Deferred<XrefFreeEntry>;
		using XrefUsedEntryPtr = Deferred<XrefUsedEntry>;
		using XrefCompressedEntryPtr = Deferred<XrefCompressedEntry>;

		class Token; using TokenPtr = Deferred<Token>;
		class TokenDictionaryBase;
		class ParserTokenDictionary;
		class ReverseParserTokenDictionary;
		class ContentStreamTokenDictionary;

		// Streams
		class BaseStream;
		class Stream;
		class ReverseStream;

		// Objects
		class ObjectUtils;
		class Object;
		class ContainableObject;

		// Numeric objects
		class NumericObject;
		class NumericObjectBackend; using NumericObjectBackendPtr = Deferred<NumericObjectBackend>;

		template <typename KeyT, typename ValueT, typename MapT /*= std::map<KeyT, ValueT>*/>
		class DictionaryObjectBase;

		template <typename T>
		class ArrayObject;

		class MixedArrayObject;
		class BooleanObject;
		class DictionaryObject;
		class IndirectObjectReference;
		class IntegerObject;
		class NameObject;
		class NullObject;
		class NumericObject;
		class Object;
		class RealObject;
		class StreamObject;
		class StringObjectBase;
		class HexadecimalStringObject;
		class LiteralStringObject;

		template <typename T>
		using ArrayObjectPtr = Deferred<ArrayObject<T>>;

		template <typename KeyT, typename ValueT, typename MapT /*= std::map<KeyT, ValueT>*/>
		using DictionaryObjectBasePtr = Deferred<DictionaryObjectBase<KeyT, ValueT, MapT>>;

		using DictionaryObjectPtr = Deferred<DictionaryObject>;
		using MixedArrayObjectPtr = Deferred<MixedArrayObject>;

		class ObjectPtr;
		class StringObjectPtr; using OutputStringObjectPtr = OutputPointer<StringObjectPtr>;
		//using ObjectPtr = Deferred<Object>;
		using ContainableObjectPtr = Deferred<ContainableObject>; using OutputContainableObjectPtr = OutputPointer<ContainableObjectPtr>;
		using NameObjectPtr = Deferred<NameObject>; using OutputNameObjectPtr = OutputPointer<NameObjectPtr>;
		using BooleanObjectPtr = Deferred<BooleanObject>;
		using IndirectObjectReferencePtr = Deferred<IndirectObjectReference>;
		using IntegerObjectPtr = Deferred<IntegerObject>;
		using NullObjectPtr = Deferred<NullObject>;
		using RealObjectPtr = Deferred<RealObject>;
		using StreamObjectPtr = Deferred<StreamObject>;
		using LiteralStringObjectPtr = Deferred<LiteralStringObject>;
		using HexadecimalStringObjectPtr = Deferred<HexadecimalStringObject>;

		// Parsers
		struct ObjectStreamEntry;
	}
}

#endif /* _SYNTAX_FWD_H */
