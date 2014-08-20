#include "high_level_object.h"

#include "object.h"
#include "dictionary_object.h"
#include "name_object.h"
#include "indirect_object_reference.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		/*
		ObjectReferenceWrapper<HighLevelObject> HighLevelObject::Create(ObjectReferenceWrapper<Object> low_level)
		{
			auto low_type = low_level->GetType();
			if (low_type == Object::Type::DictionaryObject)
			{
				auto dict = low_level.GetAs<DictionaryObject>();
				auto high_type = dict->FindAs<NameObject>(Name::Type);

				if (*high_type == Name::Pages)
				{
					//auto parent = dict->FindAs<IndirectObjectReference>(Name::Parent);
					//return ObjectReferenceWrapper<SpecializedMixedArrayObject<IndirectObjectReference>>(new SpecializedMixedArrayObject<IndirectObjectReference>(low_level));
				}
			}
		}
		*/
	}
}

