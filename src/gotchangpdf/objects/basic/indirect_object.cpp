#include "indirect_object.h"
#include "File.h"
#include "export.h"

#include <string>
#include <algorithm>

namespace gotchangpdf
{
	using namespace std;

	IndirectObject::IndirectObject(std::shared_ptr<files::File> file, int objNumber, int genNumber, std::streamoff offset /*= _BADOFF */)
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

	IndirectObject::~IndirectObject() {}

	#pragma region DllInterface

	IObject* IIndirectObject::GetObject() const
	{
		auto removed = const_cast<IIndirectObject*>(this);
		auto obj = reinterpret_cast<IndirectObject*>(removed);

		return reinterpret_cast<IObject*>(obj->GetObject().AddRefGet());
	}

	streamOffsetValueType IIndirectObject::GetOffset() const
	{
		auto removed = const_cast<IIndirectObject*>(this);
		auto obj = reinterpret_cast<IndirectObject*>(removed);

		return obj->GetOffset();
	}

	IIndirectObject::~IIndirectObject() {}

	#pragma endregion
}
