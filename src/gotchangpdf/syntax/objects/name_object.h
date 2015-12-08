#ifndef _NAME_OBJECT_H
#define _NAME_OBJECT_H

#include "containable.h"
#include "buffer.h"

#include <cstddef>

namespace gotchangpdf
{
	namespace syntax
	{
		class NameObject : public ContainableObject
		{
		public:
			typedef BufferPtr value_type;
			friend std::ostream& operator<<(std::ostream& os, const NameObject& name);

		public:
			NameObject() = default;
			explicit NameObject(value_type name) : _value(name) {}

			inline value_type Value() const _NOEXCEPT { return _value; }

			inline bool operator==(const NameObject& other) const { return Equals(other); }
			inline bool operator!=(const NameObject& other) const { return !Equals(other); }
			inline bool operator<(const NameObject& other) const { return *_value < *other._value; }

			inline bool Equals(const NameObject& other) const { return _value->Equals(*other._value); }

			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Name; }
			virtual inline std::string ToString(void) const override { return _value->ToString(); }

			void SetName(value_type name) { _value = name; }

		public:
			value_type _value;
		};
	}

	namespace constant
	{
		namespace Name
		{
			extern const syntax::NameObjectPtr ASCII85Decode;
			extern const syntax::NameObjectPtr BitsPerComponent;
			extern const syntax::NameObjectPtr Colors;
			extern const syntax::NameObjectPtr Columns;
			extern const syntax::NameObjectPtr Container;
			extern const syntax::NameObjectPtr Contents;
			extern const syntax::NameObjectPtr DecodeParms;
			extern const syntax::NameObjectPtr EarlyChange;
			extern const syntax::NameObjectPtr Length;
			extern const syntax::NameObjectPtr Type;
			extern const syntax::NameObjectPtr ObjStm;
			extern const syntax::NameObjectPtr N;
			extern const syntax::NameObjectPtr First;
			extern const syntax::NameObjectPtr Extends;
			extern const syntax::NameObjectPtr Filter;
			extern const syntax::NameObjectPtr FlateDecode;
			extern const syntax::NameObjectPtr Index;
			extern const syntax::NameObjectPtr Catalog;
			extern const syntax::NameObjectPtr Pages;
			extern const syntax::NameObjectPtr Page;
			extern const syntax::NameObjectPtr Count;
			extern const syntax::NameObjectPtr Kids;
			extern const syntax::NameObjectPtr MediaBox;
			extern const syntax::NameObjectPtr Resources;
			extern const syntax::NameObjectPtr Parent;
			extern const syntax::NameObjectPtr Predictor;
			extern const syntax::NameObjectPtr Prev;
			extern const syntax::NameObjectPtr Root;
			extern const syntax::NameObjectPtr Size;
			extern const syntax::NameObjectPtr W;
			extern const syntax::NameObjectPtr XRefStm;
		}
	}
}

namespace std
{
	template <> struct hash<gotchangpdf::syntax::NameObject>
	{
		size_t operator()(const gotchangpdf::syntax::NameObject& name) const;
	};
}

#endif /* _NAME_OBJECT_H */
