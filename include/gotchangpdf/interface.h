#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "export.h"
//#include "constants.h"

namespace gotchangpdf
{
	class GOTCHANG_PDF_API IObject
	{
	public:
		enum class Type : unsigned char
		{
			Unknown = 0,
			ArrayObject,
			Boolean,
			DictionaryObject,
			Function,
			IntegerObject,
			NameObject,
			//NameTree,
			NullObject,
			//NumberTree,
			RealObject,
			//Rectangle,
			StreamObject,
			HexadecimalString,
			LiteralString,
			IndirectReference,
			IndirectObject
		};

		static const char* TypeName(Type type);

		Type GetType(void) const;
		void Release();

		virtual ~IObject() = 0;
	};

	class GOTCHANG_PDF_API ICharacter
	{
	public:
		typedef unsigned char ValueType;

		enum class WhiteSpace : unsigned char
		{
			NUL = 0x00,
			HORIZONTAL_TAB = 0x09,
			LINE_FEED = 0x0A,
			FORM_FEED = 0x0C,
			CARRIAGE_RETURN = 0x0D,
			SPACE = 0x20
		};

		enum class Delimiter : unsigned char
		{
			LEFT_PARENTHESIS = 0x28,
			RIGHT_PARENTHESIS = 0x29,
			LESS_THAN_SIGN = 0x3C,
			GREATER_THAN_SIGN = 0x3E,
			LEFT_SQUARE_BRACKET = 0x5B,
			RIGHT_SQUARE_BRACKET = 0x5D,
			LEFT_CURLY_BRACKET = 0x7B,
			RIGHT_CURLY_BRACKET = 0x7D,
			SOLIDUS = 0x2F,
			PERCENT_SIGN = 0x25
		};

		enum class Numeric : unsigned char
		{
			ZERO = '0',
			ONE = '1',
			TWO = '2',
			THREE = '3',
			FOUR = '4',
			FIVE = '5',
			SIX = '6',
			SEVEN = '7',
			EIGHT = '8',
			NINE = '9'
		};

		bool isRegular(void) const;
		bool isSpace(void) const;
		bool isWhiteSpace(void) const;
		bool isDelimiter(void) const;
		bool isEndOfLine(void) const;
		bool isNumeric(void) const;

		ICharacter& operator= (ValueType value);

		operator ValueType() const;
		ValueType Value(void) const;

		friend bool operator== (const ICharacter& c1, const ICharacter& c2);
		friend bool operator!= (const ICharacter& c1, const ICharacter& c2);
		friend bool operator== (const ICharacter& c1, const WhiteSpace c2);
		friend bool operator!= (const ICharacter& c1, const WhiteSpace c2);
		friend bool operator== (const ICharacter& c1, const Delimiter c2);
		friend bool operator!= (const ICharacter& c1, const Delimiter c2);
		friend bool operator== (const ICharacter& c1, const Numeric c2);
		friend bool operator!= (const ICharacter& c1, const Numeric c2);
	};

	class GOTCHANG_PDF_API IIntegerObject : public virtual IObject
	{
	public:
		typedef long long ValueType;

		friend bool operator== (const IIntegerObject& i1, const IIntegerObject& i2);
		friend bool operator!= (const IIntegerObject& i1, const IIntegerObject& i2);
		friend bool operator< (const IIntegerObject& i1, const IIntegerObject& i2);

		ValueType Value(void) const;

		operator ValueType() const;

		//IntegerObject& operator= (ValueType value);
		//IntegerObject& operator= (const Buffer& value);
		//IntegerObject& operator= (const gotchangpdf::lexical::Token& value);

		virtual ~IIntegerObject() = 0;
	};

	class GOTCHANG_PDF_API IIndirectObject : public virtual IObject
	{
	public:
		void SetObject(IObject* ref);
		IObject* GetObject() const;
		void SetOffset(long long offset);
		long long GetOffset() const;

		virtual ~IIndirectObject() = 0;
	};

	class GOTCHANG_PDF_API IXref
	{
	public:
		enum class Type : unsigned char
		{
			TABLE = 0,
			STREAM
		};

		struct IEntry
		{
			IEntry();
			IEntry(IIndirectObject*, bool);
			IIndirectObject* _reference;
			bool _in_use;
		};

		void Add(const IEntry& e);
		int Size(void) const;
		IEntry At(int at) const;
		void Release();

		virtual ~IXref() = 0;
	};

	namespace files
	{
		class GOTCHANG_PDF_API IFile
		{
		public:
			static IFile* Create(const char *filename);

			void Initialize(void);
			IXref* GetXref(void) const;
			//ObjectReferenceWrapper<documents::Catalog> GetDocumentCatalog(void) const;
			IIndirectObject* GetIndirectObject(int objNumber, int genNumber) const;
			//ObjectReferenceWrapper<Header> GetHeader(void) const;
			//ObjectReferenceWrapper<Trailer> GetTrailer(void) const;
			//lexical::Parser GetParser(void) const;

			virtual ~IFile() = 0;
		};
	}

	class GOTCHANG_PDF_API IBuffer
	{
	public:
		typedef char ValueType;

		void Insert(int idx, const IBuffer& item);
		void Append(const ICharacter& ch);
		void Append(const IBuffer& item);
		void Reverse();

		ValueType* Data();
		const ValueType* Data() const;
		ValueType* Data(unsigned int idx);
		const ValueType* Data(unsigned int idx) const;

		//std::string ToString(void) const;

		//virtual bool operator==(const IBuffer& other) const override;
		//virtual bool operator<(const IBuffer& other) const override;
		ICharacter operator[](unsigned int i) const;
		ICharacter At(long at) const;
		int Size() const;

		bool operator==(const IBuffer& other) const;
		bool operator!=(const IBuffer& other) const;
		bool operator<(const IBuffer& other) const;

		virtual ~IBuffer() = 0;
	};

	class GOTCHANG_PDF_API INameObject : public virtual IObject
	{
	public:
		const IBuffer& Value() const;
		bool operator==(const INameObject& other) const;
		bool operator!=(const INameObject& other) const;

		virtual ~INameObject() = 0;
	};

	class GOTCHANG_PDF_API IDictionaryObject : public virtual IObject
	{
	public:

		class GOTCHANG_PDF_API IIterator
		{
		public:
			IIterator* Clone() const;

			INameObject* First() const;
			IObject* Second() const;

			const IIterator& operator++();
			const IIterator operator++(int);

			bool operator==(const IIterator& other) const;

			virtual ~IIterator();
		};

		IIterator* Begin(void) const;
		IIterator* End(void) const;

		IObject* Find(const INameObject& name) const;

		template <typename T>
		T* Find(const INameObject& name) const;

		virtual ~IDictionaryObject() = 0;
	};

	class GOTCHANG_PDF_API IArrayObject : public virtual IObject
	{
	public:
		int Size(void) const;
		IObject* At(unsigned int at) const;
		virtual ~IArrayObject() = 0;
	};
}

#endif /* _INTERFACE_H */
