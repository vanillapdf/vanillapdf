#include "precompiled.h"
#include "syntax/utils/object_utils.h"

namespace gotchangpdf {
namespace syntax {

bool ObjectUtils::ValueEquals(const ObjectPtr& first, const ObjectPtr& second) {
	if (first->GetType() != second->GetType())
		return false;

	auto type = first->GetType();
	switch (type) {
		case Object::Type::Array:
		{
			auto first_converted = ConvertUtils<Object*>::ConvertTo<MixedArrayObject*>(first.get());
			auto second_converted = ConvertUtils<Object*>::ConvertTo<MixedArrayObject*>(second.get());

			assert(nullptr != first_converted && nullptr != second_converted);
			if (nullptr == first_converted || nullptr == second_converted)
				return false;

			auto first_size = first_converted->Size();
			auto second_size = second_converted->Size();
			if (first_size != second_size)
				return false;

			for (unsigned int i = 0; i < first_size; ++i)
				if (!ValueEquals(first_converted->At(i), second_converted->At(i)))
					return false;

			return true;
		}
		case Object::Type::Boolean:
		{
			auto first_converted = ConvertUtils<Object*>::ConvertTo<BooleanObject*>(first.get());
			auto second_converted = ConvertUtils<Object*>::ConvertTo<BooleanObject*>(second.get());

			assert(nullptr != first_converted && nullptr != second_converted);
			if (nullptr == first_converted || nullptr == second_converted)
				return false;

			return (first_converted->GetValue() == second_converted->GetValue());
		}
		case Object::Type::Dictionary:
		{
			auto first_converted = ConvertUtils<Object*>::ConvertTo<DictionaryObject*>(first.get());
			auto second_converted = ConvertUtils<Object*>::ConvertTo<DictionaryObject*>(second.get());

			assert(nullptr != first_converted && nullptr != second_converted);
			if (nullptr == first_converted || nullptr == second_converted)
				return false;

			auto first_vals = first_converted->Values();
			auto second_vals = second_converted->Values();
			auto first_vals_size = first_vals.size();
			auto second_vals_size = second_vals.size();
			if (first_vals_size != second_vals_size)
				return false;

			for (unsigned int i = 0; i < second_vals_size; ++i)
				if (!ValueEquals(first_vals[i], second_vals[i]))
					return false;

			return true;
		}
		case Object::Type::IndirectReference:
		{
			auto first_converted = ConvertUtils<Object*>::ConvertTo<IndirectObjectReference*>(first.get());
			auto second_converted = ConvertUtils<Object*>::ConvertTo<IndirectObjectReference*>(second.get());

			assert(nullptr != first_converted && nullptr != second_converted);
			if (nullptr == first_converted || nullptr == second_converted)
				return false;

			return (*first_converted == *second_converted);
		}
		case Object::Type::Integer:
		{
			auto first_converted = ConvertUtils<Object*>::ConvertTo<IntegerObject*>(first.get());
			auto second_converted = ConvertUtils<Object*>::ConvertTo<IntegerObject*>(second.get());

			assert(nullptr != first_converted && nullptr != second_converted);
			if (nullptr == first_converted || nullptr == second_converted)
				return false;

			return (first_converted->Equals(*second_converted));
		}
		case Object::Type::Name:
		{
			auto first_converted = ConvertUtils<Object*>::ConvertTo<NameObject*>(first.get());
			auto second_converted = ConvertUtils<Object*>::ConvertTo<NameObject*>(second.get());

			assert(nullptr != first_converted && nullptr != second_converted);
			if (nullptr == first_converted || nullptr == second_converted)
				return false;

			return first_converted->Equals(*second_converted);
		}
		case Object::Type::Null:
		{
			auto first_converted = ConvertUtils<Object*>::ConvertTo<NullObject*>(first.get());
			auto second_converted = ConvertUtils<Object*>::ConvertTo<NullObject*>(second.get());

			assert(nullptr != first_converted && nullptr != second_converted);
			if (nullptr == first_converted || nullptr == second_converted)
				return false;

			return (*first_converted == *second_converted);
		}
		case Object::Type::Real:
		{
			auto first_converted = ConvertUtils<Object*>::ConvertTo<RealObject*>(first.get());
			auto second_converted = ConvertUtils<Object*>::ConvertTo<RealObject*>(second.get());

			assert(nullptr != first_converted && nullptr != second_converted);
			if (nullptr == first_converted || nullptr == second_converted)
				return false;

			return (first_converted->GetValue() == second_converted->GetValue());
		}
		case Object::Type::Stream:
		{
			auto first_converted = ConvertUtils<Object*>::ConvertTo<StreamObject*>(first.get());
			auto second_converted = ConvertUtils<Object*>::ConvertTo<StreamObject*>(second.get());

			assert(nullptr != first_converted && nullptr != second_converted);
			if (nullptr == first_converted || nullptr == second_converted)
				return false;

			auto first_header = first_converted->GetHeader();
			auto second_header = second_converted->GetHeader();
			if (!ValueEquals(first_header, second_header))
				return false;

			auto first_body = first_converted->GetBody();
			auto second_body = second_converted->GetBody();
			if (first_body != second_body)
				return false;

			return true;
		}
		case Object::Type::String:
		{
			auto first_converted = ConvertUtils<Object*>::ConvertTo<StringObjectBase*>(first.get());
			auto second_converted = ConvertUtils<Object*>::ConvertTo<StringObjectBase*>(second.get());

			assert(nullptr != first_converted && nullptr != second_converted);
			if (nullptr == first_converted || nullptr == second_converted)
				return false;

			return first_converted->Equals(*second_converted);
		}
		default:
			assert(!"Unknown object type");
	}

	return false;
}

} // syntax
} // gotchangpdf
