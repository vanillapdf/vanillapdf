#ifndef _DICTIONARY_OBJECT_H
#define _DICTIONARY_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "name_object.h"
#include "smart_ptr.h"
#include "direct_object.h"
#include "object_visitors.h"

#include <unordered_map>

namespace gotchangpdf
{
	class DictionaryObject : public Object
	{
	public:
		typedef std::unordered_map<NameObject, DirectObject, NameObject::Hasher> value_type;

	public:
		class Iterator
		{
		public:
			Iterator(value_type::const_iterator it);

			Iterator* Clone() const;

			const Iterator& operator++();
			const Iterator operator++(int);

			NameObject First() const;
			DirectObject Second() const;

			bool operator==(const Iterator& other) const;

		private:
			value_type::const_iterator _it;
		};

		virtual inline Object::Type GetType(void) const override { return Object::Type::DictionaryObject; }

		Iterator Begin(void) const;
		Iterator End(void) const;

		//friend Objects::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, DictionaryObject& o);
		friend lexical::Parser& operator>> (lexical::Parser& s, DictionaryObject& o);

		DirectObject Find(const NameObject& name) const;

		template <typename T>
		const T FindAs(const NameObject& name) const
		{
			auto result = _list.find(name);

			ObjectVisitor<T> visitor;
			return result->second.apply_visitor(visitor);
			//return dynamic_wrapper_cast<T>(result->second);


			// TODO
			//return nullptr;
		}

		const value_type& GetMap(void) const { return _list; }
		void SetMap(value_type& list) { _list = list; }

	//private:
	public:
		value_type _list;
	};

	typedef SmartPtr<DictionaryObject> DictionaryObjectPtr;
}

#endif /* _DICTIONARY_OBJECT_H */
