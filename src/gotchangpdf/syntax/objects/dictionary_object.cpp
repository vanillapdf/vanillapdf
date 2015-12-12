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
		std::vector<ContainableObjectPtr> DictionaryObject::Values() const
		{
			std::vector<ContainableObjectPtr> result;
			std::for_each(_list.begin(), _list.end(), [&result](const std::pair<NameObjectPtr, ContainableObjectPtr>& item) {result.push_back(item.second); });
			return result;
		}
	}
}