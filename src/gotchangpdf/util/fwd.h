#ifndef _FWD_H
#define _FWD_H

#include "deferred.h"

namespace gotchangpdf
{
	template <typename T, typename Container>
	class ArrayObject;

	class MixedArrayObject;
	class BooleanObject;
	class DictionaryObject;
	class FunctionObject;
	class IndirectObject;
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

	namespace documents
	{
		class Document;
		class Catalog;
		class PageTree;
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
	}

	template <typename T, typename Container>
	using ArrayObjectPtr = Deferred<ArrayObject<T, Container>>;

	using NameObjectPtr = Deferred<NameObject>;
	using DictionaryObjectPtr = Deferred<DictionaryObject>;
	using MixedArrayObjectPtr = Deferred<MixedArrayObject>;
	using BooleanObjectPtr = Deferred<BooleanObject>;
	using FunctionObjectPtr = Deferred<FunctionObject>;
	using IndirectObjectPtr = Deferred<IndirectObject>;
	using IndirectObjectReferencePtr = Deferred<IndirectObjectReference>;
	using IntegerObjectPtr = Deferred<IntegerObject>;
	using NullObjectPtr = Deferred<NullObject>;
	using RealObjectPtr = Deferred<RealObject>;
	using StreamObjectPtr = Deferred<StreamObject>;
	using LiteralStringPtr = Deferred<LiteralString>;
	using HexadecimalStringPtr = Deferred<HexadecimalString>;
}

#endif /* _FWD_H */
