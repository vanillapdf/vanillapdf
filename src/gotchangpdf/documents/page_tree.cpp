#include "page_tree.h"
#include "indirect_object.h"
#include "dictionary_object.h"
#include "exception.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		PageTree::PageTree() : _count(boost::intrusive_ptr<IntegerObject>(new IntegerObject())), _kids(boost::intrusive_ptr<ArrayObject>(new ArrayObject())) {}

		PageTree::PageTree(const IndirectObject& obj)
		{
			auto dict = dynamic_wrapper_cast<DictionaryObject>(obj.GetObject());

			if (*dict->Find<NameObject>(Name::Type) != Name::Pages)
				throw Exception("TODO");

			_count = dict->Find<IntegerObject>(Name::Count);
			_kids = dict->Find<ArrayObject>(Name::Kids);
		}
	}
}
