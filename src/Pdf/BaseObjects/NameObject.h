#ifndef _NAME_OBJECT_H
#define _NAME_OBJECT_H

#include "INameObject.h"
#include "BaseObjects/Object.h"
#include "Buffer.h"

namespace Pdf
{
	class NameObject : public Object, public INameObject
	{
	public:
		struct Hasher {
			unsigned long operator()(const NameObject& t) const
			{
				auto vec = t._value.Data();
				unsigned long result = 0;
				unsigned int size = t._value.Size();
				for (unsigned int i = 0; i < size; ++i)
					result ^= vec[i];

				return result;
			}
		};

		NameObject();
		explicit NameObject(const Pdf::Lexical::Token& token);
		explicit NameObject(const Buffer& name);

		//const CharacterSet& Value() const;

		virtual const Buffer& Value() const override;
		bool operator==(const NameObject& other) const;

	private:
		Buffer _value;

		friend void ::boost::intrusive_ptr_add_ref(NameObject*);
		friend void ::boost::intrusive_ptr_release(NameObject*);
	};

	namespace Constant
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
		}
	}
}

#endif /* _NAME_OBJECT_H */
