#ifndef _FWD_H
#define _FWD_H

namespace gotchangpdf
{
	class MixedArrayObject;
	class BooleanObject;
	class Xref;
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

	class Character;
	class Buffer;
}

#endif /* _FWD_H */
