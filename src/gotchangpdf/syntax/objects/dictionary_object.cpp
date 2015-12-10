#include "precompiled.h"
#include "dictionary_object.h"

namespace gotchangpdf
{
	namespace syntax
	{
		std::ostream& operator<<(std::ostream& os, const syntax::DictionaryObject& obj)
		{
			os << obj.ToString();
			return os;
		}

		std::string DictionaryObject::ToString(void) const
		{
			std::stringstream ss;
			ss << "<< ";
			bool first = true;
			for (auto item : _list) {
				ss << (first ? "" : ", ") << item.first << " " << item.second->ToString();
				first = false;
			}
			ss << " >>";
			return ss.str();
		}

		DictionaryObject::const_iterator DictionaryObject::begin(void) const _NOEXCEPT { return _list.begin(); }
		DictionaryObject::const_iterator DictionaryObject::end(void) const _NOEXCEPT { return _list.end(); }

		DictionaryObject::iterator DictionaryObject::insert(const_iterator pos, const value_type & value)
		{
			return _list.insert(pos, value);
		}

		ContainableObjectPtr DictionaryObject::Find(const NameObjectPtr & name) const
		{
			auto result = _list.find(name);
			if (result == _list.end()) {
				throw GeneralException("Item with name " + name->Value()->ToString() + " was not found in dictionary");
			}

			return result->second;
		}

		bool DictionaryObject::TryFind(const NameObjectPtr & name, ContainableObjectPtr & result) const
		{
			auto item = _list.find(name);
			if (item == _list.end())
				return false;

			result = item->second;
			return true;
		}

		bool DictionaryObject::Contains(const NameObjectPtr & name) const { return (_list.find(name) != _list.end()); }
	}
}