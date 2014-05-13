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
	private:
		typedef std::unordered_map<NameObject, ObjectReferenceWrapper<Object>, NameObject::Hasher> listType;

	public:
		class Iterator
		{
		public:
			Iterator(listType::const_iterator it);

			Iterator* Clone() const;

			const Iterator& operator++();
			const Iterator operator++(int);

			NameObject First() const;
			ObjectReferenceWrapper<Object> Second() const;

			bool operator==(const Iterator& other) const;

		private:
			listType::const_iterator _it;
		};

		Iterator Begin(void) const;
		Iterator End(void) const;

		DictionaryObject();
		//friend Objects::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, DictionaryObject& o);
		friend lexical::Parser& operator>> (lexical::Parser& s, DictionaryObject& o);

		ObjectReferenceWrapper<Object> Find(const NameObject& name) const;

		template <typename T>
		ObjectReferenceWrapper<T> Find(const NameObject& name) const
		{
			auto result = _list.find(name);
			return result->second.GetAs<T>();
		}

	private:
		listType _list;
	};
}

#endif /* _DICTIONARY_OBJECT_H */
