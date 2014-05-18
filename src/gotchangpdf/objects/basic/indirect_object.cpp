#include "indirect_object.h"
#include "File.h"

#include "c_indirect_object.h"

#include <string>
#include <algorithm>

namespace gotchangpdf
{
	using namespace std;

	IndirectObject::IndirectObject(std::shared_ptr<files::File> file, int objNumber, int genNumber, streamOffsetValueType offset /*= _BADOFF */)
		: Object(Object::Type::IndirectObject), _file(file), _objNumber(objNumber), _genNumber(genNumber), _offset(offset), RequireVersion(file->GetHeader()->GetVersion()) {}

	IndirectObject::IndirectObject(const IndirectObject& other) :
		_file(other._file), _genNumber(other._genNumber), _objNumber(other._objNumber), _offset(other._offset), _reference(other._reference), RequireVersion(other._file->GetHeader()->GetVersion()) {}

	void IndirectObject::SetObject(ObjectReferenceWrapper<Object> ref) { _reference = ref; }

	ObjectReferenceWrapper<Object> IndirectObject::GetObject() const
	{
		if (nullptr == _reference)
		{
			//auto indirect = _file.GetIndirectObject(_objNumber, _genNumber);
			auto parser = _file->GetParser();
			auto pos = parser.tellg();
			parser.seekg(_offset);
			auto obj = parser.readObjectWithType<IndirectObject>();
			parser.seekg(pos);
			_reference = obj->GetObject();
		}

		return _reference;
	}

	void IndirectObject::SetOffset(streamOffsetValueType offset) { _offset = offset; }
	streamOffsetValueType IndirectObject::GetOffset() const { return _offset; }
	/*
	IObject* IndirectObject::GetIObject() const
	{
		auto item = GetObject();

		gotchangpdf::Object *ptr = item.AddRefGet();
		//boost::intrusive_ptr_add_ref(ptr);

		return reinterpret_cast<IObject*>(ptr);
	}
	*/
}

using namespace gotchangpdf;

GOTCHANG_PDF_API long long CALLING_CONVENTION IndirectObject_GetOffset(IndirectObjectHandle handle)
{
	gotchangpdf::IndirectObject* obj = reinterpret_cast<gotchangpdf::IndirectObject*>(handle);
	return obj->GetOffset();
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION IndirectObject_GetObject(IndirectObjectHandle handle)
{
	gotchangpdf::IndirectObject* obj = reinterpret_cast<gotchangpdf::IndirectObject*>(handle);

	gotchangpdf::ObjectReferenceWrapper<gotchangpdf::Object> item = obj->GetObject();

	gotchangpdf::Object *ptr = item.AddRefGet();
	//boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API void CALLING_CONVENTION IndirectObject_Release(IndirectObjectHandle handle)
{
	gotchangpdf::IndirectObject* obj = reinterpret_cast<gotchangpdf::IndirectObject*>(handle);
	obj->Release();
}
