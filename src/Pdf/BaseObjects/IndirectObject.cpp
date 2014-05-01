#include "IndirectObject.h"
#include "File.h"
#include "Export.h"

#include <string>
#include <algorithm>

namespace Pdf
{
	using namespace std;

	IndirectObject::IndirectObject(std::shared_ptr<Files::File> file, int objNumber, int genNumber, std::streamoff offset /*= _BADOFF */)
		: Object(Object::Type::IndirectObject), _file(file), _objNumber(objNumber), _genNumber(genNumber), _offset(offset), _reference(nullptr) {}

	IndirectObject::IndirectObject(std::shared_ptr<Files::File> file, const Buffer& value) : Object(Object::Type::IndirectObject), _file(file), _reference(nullptr)
	{
		string str(value.begin(), value.end());
		int space = str.find_first_of(static_cast<unsigned char>(Character::WhiteSpace::SPACE));

		_objNumber = stoi(str.substr(0, space));
		_genNumber = stoi(str.substr(space));

		// TODO 'obj' validation
	}

	IndirectObject::IndirectObject(const IndirectObject& other) : _file(other._file), _genNumber(other._genNumber), _objNumber(other._objNumber), _offset(other._offset), _reference(other._reference) {}

	void IndirectObject::SetObject(boost::intrusive_ptr<Object> ref)
	{
		// TODO validation

		_reference = ref;
	}

	boost::intrusive_ptr<Object> IndirectObject::GetObject() const
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

	void IndirectObject::SetOffset(std::streamoff offset) { _offset = offset; }
	streamOffsetValueType IndirectObject::GetOffset() const { return _offset; }

	IObject* IndirectObject::GetIObject() const
	{
		auto item = GetObject();

		Pdf::Object *ptr = item.get();
		boost::intrusive_ptr_add_ref(ptr);

		return reinterpret_cast<IObject*>(ptr);
	}
}

using namespace Pdf;

GOTCHANG_PDF_API long long CALLING_CONVENTION IndirectObject_GetOffset(IndirectObjectHandle handle)
{
	Pdf::IndirectObject* obj = reinterpret_cast<Pdf::IndirectObject*>(handle);
	return obj->GetOffset();
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION IndirectObject_GetObject(IndirectObjectHandle handle)
{
	Pdf::IndirectObject* obj = reinterpret_cast<Pdf::IndirectObject*>(handle);

	boost::intrusive_ptr<Pdf::Object> item = obj->GetObject();

	Pdf::Object *ptr = item.get();
	boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API void CALLING_CONVENTION IndirectObject_Release(IndirectObjectHandle handle)
{
	Pdf::IndirectObject* obj = reinterpret_cast<Pdf::IndirectObject*>(handle);
	boost::intrusive_ptr_release(obj);
}
