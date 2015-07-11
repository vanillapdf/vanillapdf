#ifndef _FWD_H
#define _FWD_H

namespace gotchangpdf
{
	template <typename T>
	struct Deferred;

	template <typename T>
	struct DeferredContainer;

	template <typename T>
	class SmartPtr;

	template <typename T>
	class ArrayObject;

	class MixedArrayObject;
	class BooleanObject;
	class DictionaryObject;
	class FunctionObject;
	class IndirectObjectReference;
	class IntegerObject;
	class NameObject;
	class NullObject;
	class NumericObject;
	class Object;
	class RealObject;
	class StreamObject;

	class StringObject;
	class HexadecimalStringObject;
	class LiteralStringObject;

	class Date;
	class NameTree;
	class NumberTree;
	class Rectangle;
	class Tree;

	class Character;
	class Buffer;

	class Containable;

	struct ObjectStreamHeader;

	namespace documents
	{
		class Document;
		class Catalog;
		class PageNode;
		class PageTree;
		class PageTreeNode;
		class PageObject;

		class ResourceDictionary;
		class Rectangle;
	}

	namespace exceptions
	{
		class Exception;
		class InvalidObjectTypeException;
	}

	namespace files
	{
		class File;
		class Header;
		class Trailer;

		class Xref;
		class XrefTable;
		class XrefStream;
		class XrefChain;
		class XrefWithMetadata;

		class XrefEntry;
		class XrefUsedEntry;
		class XrefFreeEntry;
		class XrefCompressedEntry;
	}

	namespace lexical
	{
		class Tree;
		class Parser;
		class Token;
		class TokenDictionary;

		class BaseStream;
		class Stream;
		class ReverseStream;
		class SpiritParser;
	}

	namespace filters
	{
		class Filter;
		class FlateDecodeFilter;
	}

	namespace raw
	{
		class BaseStream;
		class Stream;
		class ReverseStream;
	}

	using FilterPtr = SmartPtr<filters::Filter>;
	using FlateDecodeFilterPtr = SmartPtr<filters::FlateDecodeFilter>;

	using BufferPtr = DeferredContainer<Buffer>;

	template <typename T>
	using ArrayObjectPtr = Deferred<ArrayObject<T>>;

	using DictionaryObjectPtr = Deferred<DictionaryObject>;
	using MixedArrayObjectPtr = Deferred<MixedArrayObject>;

	using NameObjectPtr = Deferred<NameObject>;
	using BooleanObjectPtr = Deferred<BooleanObject>;
	using FunctionObjectPtr = Deferred<FunctionObject>;
	using IndirectObjectReferencePtr = Deferred<IndirectObjectReference>;
	using IntegerObjectPtr = Deferred<IntegerObject>;
	using NullObjectPtr = Deferred<NullObject>;
	using RealObjectPtr = Deferred<RealObject>;
	using StreamObjectPtr = Deferred<StreamObject>;
	using LiteralStringObjectPtr = Deferred<LiteralStringObject>;
	using HexadecimalStringObjectPtr = Deferred<HexadecimalStringObject>;

	using XrefChainPtr = SmartPtr<files::XrefChain>;
	using XrefWithMetadataPtr = SmartPtr<files::XrefWithMetadata>;
	using XrefEntryPtr = SmartPtr<files::XrefEntry>;
	using XrefFreeEntryPtr = SmartPtr<files::XrefFreeEntry>;
	using XrefUsedEntryPtr = SmartPtr<files::XrefUsedEntry>;
	using XrefCompressedEntryPtr = SmartPtr<files::XrefCompressedEntry>;

	using HeaderPtr = SmartPtr<files::Header>;
	using TrailerPtr = SmartPtr<files::Trailer>;

	using XrefPtr = SmartPtr<files::Xref>;
	using XrefTablePtr = SmartPtr<files::XrefTable>;
	using XrefStreamPtr = SmartPtr<files::XrefStream>;

	using PageTreePtr = SmartPtr<documents::PageTree>;
	using PageTreeNodePtr = SmartPtr<documents::PageTreeNode>;
	using PageObjectPtr = SmartPtr<documents::PageObject>;
	using PageNodePtr = SmartPtr<documents::PageNode>;
	using ResourceDictionaryPtr = SmartPtr<documents::ResourceDictionary>;
	using RectanglePtr = SmartPtr<documents::Rectangle>;
	using CatalogPtr = SmartPtr<documents::Catalog>;
}

#endif /* _FWD_H */
