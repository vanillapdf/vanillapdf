#include "precompiled.h"
#include "dictionary_object.h"

namespace gotchangpdf
{
	namespace syntax
	{
		std::string DictionaryObject::ToString(void) const
		{
			std::stringstream ss;
			ss << "<<" << std::endl;
			for (auto item : _list) {
				ss << item.first->ToString() << " " << item.second->ToString() << std::endl;
			}
			ss << ">>";
			return ss.str();
		}

		std::string DictionaryObject::ToPdf(void) const
		{
			std::stringstream ss;
			ss << "<<";
			bool first = true;
			for (auto item : _list) {
				ss << (first ? "" : " ") << item.first->ToPdf() << " " << item.second->ToPdf();
				first = false;
			}
			ss << ">>";
			return ss.str();
		}

		ContainableObjectPtr DictionaryObject::Find(const NameObjectPtr & name) const
		{
			auto result = _list.find(name);
			if (result == _list.end()) {
				throw GeneralException("Item with name " + name->ToString() + " was not found in dictionary");
			}

			return result->second;
		}

		bool DictionaryObject::TryFind(const NameObjectPtr & name, OutputContainableObjectPtr& result) const
		{
			auto item = _list.find(name);
			if (item == _list.end())
				return false;

			result = item->second;
			return true;
		}

		void DictionaryObject::Remove(const NameObjectPtr& name)
		{
			_list.erase(name);
		}

		bool DictionaryObject::Insert(const NameObjectPtr& name, const ContainableObjectPtr& value)
		{
			std::pair<NameObjectPtr, ContainableObjectPtr> pair(name, value);
			auto result = _list.insert(pair);

			//assert(result.second && "Key was already in the dictionary");
			return result.second;
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