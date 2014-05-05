#ifndef _FWD_H
#define _FWD_H

namespace Pdf
{
	class ArrayObject;
	class Boolean;
	class CrossReferenceInfo;
	class DictionaryObject;
	class Function;
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

	class Exception;
	class InvalidObjectTypeException;

	namespace Files
	{
		class File;
		class Header;
		class Trailer;
	}

	namespace Lexical
	{
		class Tree;
		class Parser;
		class Token;
		class TokenDictionary;
	}

	namespace Filters
	{
		class Filter;
		class FlateDecodeFilter;
	}

	namespace Streams
	{
		namespace Raw
		{
			class BaseStream;
			class Stream;
			class ReverseStream;
		}

		namespace Basic
		{
			class BaseStream;
			class Stream;
			class ReverseStream;
		}

		namespace Lexical
		{
			class BaseStream;
			class Stream;
			class ReverseStream;
		}
	}

	class Character;
	class Buffer;
}

#endif /* _FWD_H */
