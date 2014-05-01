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
}

#endif /* _NAME_OBJECT_H */
