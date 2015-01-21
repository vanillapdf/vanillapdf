#ifndef _FWD_H
#define _FWD_H

#include "deferred.h"
#include "smart_ptr.h"

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
	class HexadecimalString;
	class LiteralString;

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
		class Trailer;
		class Xref;
		class XrefEntry;
	}

	namespace lexical
	{
		class Tree;
		class Parser;
		class Token;
		class TokenDictionary;
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

	namespace lexical
	{
		class BaseStream;
		class Stream;
		class ReverseStream;
		class SpiritParser;
	}

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
	using LiteralStringPtr = Deferred<LiteralString>;
	using HexadecimalStringPtr = Deferred<HexadecimalString>;

	using XrefEntryPtr = Deferred<files::XrefEntry>;
	using HeaderPtr = Deferred<files::Header>;
	using TrailerPtr = Deferred<files::Trailer>;
	using XrefPtr = Deferred<files::Xref>;

	using PageTreePtr = SmartPtr<documents::PageTree>;
	using PageTreeNodePtr = SmartPtr<documents::PageTreeNode>;
	using PageObjectPtr = SmartPtr<documents::PageObject>;
	using PageNodePtr = SmartPtr<documents::PageNode>;
	using ResourceDictionaryPtr = SmartPtr<documents::ResourceDictionary>;
	using RectanglePtr = SmartPtr<documents::Rectangle>;
	using CatalogPtr = SmartPtr<documents::Catalog>;
}

#endif /* _FWD_H */
