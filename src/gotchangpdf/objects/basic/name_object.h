#ifndef _NAME_OBJECT_H
#define _NAME_OBJECT_H

#include "object.h"
#include "Buffer.h"

namespace gotchangpdf
{
	class NameObject : public Object
	{
	public:
		struct Hasher {
			unsigned long operator()(const NameObject& t) const
			{
				unsigned long result = 0;
				unsigned int size = t._value.Size();
				for (unsigned int i = 0; i < size; ++i)
					result ^= t._value[i];

				return result;
			}
		};

		NameObject();
		explicit NameObject(const gotchangpdf::lexical::Token& token);
		explicit NameObject(const Buffer& name);

		//const CharacterSet& Value() const;

		const Buffer& Value() const;
		bool operator==(const NameObject& other) const;
		bool operator!=(const NameObject& other) const;

	private:
		Buffer _value;
	};

	namespace constant
	{
		namespace Name
		{
			#define DECLARE_CONST_NAME(name)	const NameObject name = NameObject(Buffer(#name, sizeof(#name)))

			DECLARE_CONST_NAME(Length);
			DECLARE_CONST_NAME(Type);
			DECLARE_CONST_NAME(ObjStm);
			DECLARE_CONST_NAME(N);
			DECLARE_CONST_NAME(First);
			DECLARE_CONST_NAME(Extends);
			DECLARE_CONST_NAME(Filter);
			DECLARE_CONST_NAME(FlateDecode);
			DECLARE_CONST_NAME(Catalog);
			DECLARE_CONST_NAME(Pages);
			DECLARE_CONST_NAME(Page);
			DECLARE_CONST_NAME(Count);
			DECLARE_CONST_NAME(Kids);
			DECLARE_CONST_NAME(Parent);
			DECLARE_CONST_NAME(MediaBox);
			DECLARE_CONST_NAME(Resources);
		}
	}
}

#endif /* _NAME_OBJECT_H */
