#ifndef _DICTIONARY_OBJECT_H
#define _DICTIONARY_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "name_object.h"
#include "i_dictionary_object.h"

#include "boost/intrusive_ptr.hpp"

#include <unordered_map>

namespace gotchangpdf
{
	class DictionaryObject : public Object, public IDictionaryObject
	{
	public:
		// TODO
		typedef std::unordered_map<NameObject, boost::intrusive_ptr<Object>, NameObject::Hasher> listType;
		listType::const_iterator DictionaryObject::Begin() const;
		listType::const_iterator DictionaryObject::End() const;

		DictionaryObject();
		//friend Objects::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, DictionaryObject& o);
		friend lexical::Parser& operator>> (lexical::Parser& s, DictionaryObject& o);

		boost::intrusive_ptr<Object> Find(const NameObject& name) const;

		virtual IObject* IObjectFind(const INameObject& name) const override;
		virtual IObject* IObjectFind(const char* name, int len) const override;

	private:
		listType _list;

		friend void ::boost::intrusive_ptr_add_ref(DictionaryObject*);
		friend void ::boost::intrusive_ptr_release(DictionaryObject*);
	};
}

#endif /* _DICTIONARY_OBJECT_H */
