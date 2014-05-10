#ifndef _DICTIONARY_OBJECT_H
#define _DICTIONARY_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "name_object.h"
#include "object_reference_wrapper.h"

#include <unordered_map>

namespace gotchangpdf
{
	class DictionaryObject : public Object
	{
	public:
		// TODO
		typedef std::unordered_map<NameObject, ObjectReferenceWrapper<Object>, NameObject::Hasher> listType;
		listType::const_iterator DictionaryObject::Begin() const;
		listType::const_iterator DictionaryObject::End() const;

		DictionaryObject();
		//friend Objects::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, DictionaryObject& o);
		friend lexical::Parser& operator>> (lexical::Parser& s, DictionaryObject& o);

		ObjectReferenceWrapper<Object> Find(const NameObject& name) const;

		template <typename T>
		ObjectReferenceWrapper<T> Find(const NameObject& name) const
		{
			auto result = _list.find(name);
			return dynamic_wrapper_cast<T>(result->second);
		}

	private:
		listType _list;

		template <typename T>
		friend void ::boost::intrusive_ptr_add_ref(T*);

		template <typename T>
		friend void ::boost::intrusive_ptr_release(T*);
	};
}

#endif /* _DICTIONARY_OBJECT_H */
