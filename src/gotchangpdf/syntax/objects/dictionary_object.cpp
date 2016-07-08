#include "precompiled.h"
#include "dictionary_object.h"

namespace gotchangpdf
{
	namespace syntax
	{
		DictionaryObject* DictionaryObject::Clone(void) const
		{
			DictionaryObjectPtr result;
			for (auto item : _list)
			{
				auto name = ObjectUtils::Clone<NameObjectPtr>(item.first);
				auto value = ObjectUtils::Clone<ContainableObjectPtr>(item.second);
				result->Insert(name, value);
			}

			return result.AddRefGet();
		}

		void DictionaryObject::SetObjectNumber(types::big_uint number) noexcept
		{
			Object::SetObjectNumber(number);

			for (auto it = _list.begin(); it != _list.end(); ++it) {
				auto item = it->second;
				item->SetObjectNumber(number);
			}
		}

		void DictionaryObject::SetGenerationNumber(types::ushort number) noexcept
		{
			Object::SetGenerationNumber(number);

			for (auto it = _list.begin(); it != _list.end(); ++it) {
				auto item = it->second;
				item->SetGenerationNumber(number);
			}
		}

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
			auto found = _list.find(name);
			if (found == _list.end()) {
				return;
			}

			found->first->Unsubscribe(this);
			found->second->Unsubscribe(this);
			_list.erase(found);
			OnChanged();
		}

		bool DictionaryObject::Insert(const NameObjectPtr& name, const ContainableObjectPtr& value)
		{
			std::pair<NameObjectPtr, ContainableObjectPtr> pair(name, value);
			auto result = _list.insert(pair);
			name->Subscribe(this);
			value->Subscribe(this);

			//assert(result.second && "Key was already in the dictionary");
			OnChanged();
			return result.second;
		}

		bool DictionaryObject::Contains(const NameObjectPtr & name) const { return (_list.find(name) != _list.end()); }
		std::vector<ContainableObjectPtr> DictionaryObject::Values() const
		{
			std::vector<ContainableObjectPtr> result;
			std::for_each(_list.begin(), _list.end(), [&result](const std::pair<NameObjectPtr, ContainableObjectPtr>& item) {result.push_back(item.second); });
			return result;
		}

		DictionaryObject::~DictionaryObject()
		{
			for (auto item : _list) {
				item.first->Unsubscribe(this);
				item.second->Unsubscribe(this);
			}
		}
	}
}