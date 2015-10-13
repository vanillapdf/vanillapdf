#ifndef _FWD_H
#define _FWD_H

namespace gotchangpdf
{
	template <typename T, bool DefaultConstructible /* = true */>
	struct Deferred;

	template <typename T, bool DefaultConstructible /* = true */>
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

		class XrefBase;
		class XrefSubsection;
		class XrefTable;
		class XrefStream;
		class XrefChain;

		class XrefEntryBase;
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

		struct ObjectStreamHeader;
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

	using FlateDecodeFilterPtr = Deferred<filters::FlateDecodeFilter>;

	using BufferPtr = DeferredContainer<Buffer>;

	template <typename T>
	using ArrayObjectPtr = Deferred<ArrayObject<T>>;

	using DictionaryObjectPtr = Deferred<DictionaryObject>;
	using MixedArrayObjectPtr = Deferred<MixedArrayObject>;

	using NameObjectPtr = Deferred<NameObject>;
	using BooleanObjectPtr = Deferred<BooleanObject>;
	using FunctionObjectPtr = Deferred<FunctionObject>;
	using IndirectObjectReferencePtr = Deferred<IndirectObjectReference, false>;
	using IntegerObjectPtr = Deferred<IntegerObject>;
	using NullObjectPtr = Deferred<NullObject, false>;
	using RealObjectPtr = Deferred<RealObject>;
	using StreamObjectPtr = Deferred<StreamObject>;
	using LiteralStringObjectPtr = Deferred<LiteralStringObject>;
	using HexadecimalStringObjectPtr = Deferred<HexadecimalStringObject>;

	using XrefChainPtr = Deferred<files::XrefChain>;
	using XrefEntryBasePtr = Deferred<files::XrefEntryBase, false>;
	using XrefFreeEntryPtr = Deferred<files::XrefFreeEntry, false>;
	using XrefUsedEntryPtr = Deferred<files::XrefUsedEntry, false>;
	using XrefCompressedEntryPtr = Deferred<files::XrefCompressedEntry, false>;

	using HeaderPtr = Deferred<files::Header>;

	using XrefBasePtr = Deferred<files::XrefBase>;
	using XrefSubsectionPtr = Deferred<files::XrefSubsection, false>;
	using XrefTablePtr = Deferred<files::XrefTable>;
	using XrefStreamPtr = Deferred<files::XrefStream>;

	using PageTreePtr = SmartPtr<documents::PageTree>;
	using PageTreeNodePtr = SmartPtr<documents::PageTreeNode>;
	using PageObjectPtr = SmartPtr<documents::PageObject>;
	using PageNodePtr = SmartPtr<documents::PageNode>;
	using ResourceDictionaryPtr = SmartPtr<documents::ResourceDictionary>;
	using RectanglePtr = SmartPtr<documents::Rectangle>;
	using CatalogPtr = SmartPtr<documents::Catalog>;
}

#endif /* _FWD_H */
