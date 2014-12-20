#include "file.h"

#include "indirect_object.h"
#include "lexical_reverse_stream.h"
#include "spirit_parser.h"
#include "exception.h"

#include "xref.h"
#include "header.h"
#include "trailer.h"
#include "parser.h"
#include "catalog.h"

#include "c_file.h"

#include <memory>
#include <cassert>
#include <iostream>

#include <boost/scope_exit.hpp>
#include <boost/typeof/incr_registration_group.hpp>

namespace gotchangpdf
{
	namespace files
	{
		using namespace std;
		using namespace files;
		using namespace lexical;
		using namespace exceptions;

		File::File(const char * filename) : _filename(filename) {}

		File::~File(void)
		{
			if (nullptr != _input)
			{
				_input->close();
				_input = nullptr;
			}

			_cache.clear();
		}

		void File::Initialize(void)
		{
			if (_initialized)
				return;

			_cache = vector<IndirectObjectPtr>();
			_xref = Deferred<Xref>(Xref());
			_header = Deferred<Header>(Header());
			_trailer = Deferred<Trailer>(Trailer());

			//TODO check if file exists

			_input = shared_ptr<FileDevice>(new FileDevice());
			_input->open(_filename,
				ios_base::in | ios_base::out | ios_base::binary);

			if (!_input || !_input->good())
				throw new exceptions::Exception("Could not open file");

			Parser stream = Parser(this, *_input);

			stream.seekg(ios_base::beg);
			stream >> *_header;

			ReverseStream reversed = ReverseStream(*_input);
			reversed >> *_trailer;

			stream.seekg(_trailer->xref_offset(), ios_base::beg);
			stream >> *_xref;

			stream.ReadTokenWithType(Token::Type::TRAILER);
			stream.ReadTokenWithType(Token::Type::EOL);

			// HACK
			stream >> *_trailer->dictionary();

			_initialized = true;
		}

		Deferred<IndirectObject> File::GetIndirectObject(types::integer objNumber,
			types::ushort genNumber)
		{
			if (!_initialized)
				throw new Exception("File has not been initialized yet");

			//if (_cache.)

			//TODO
			auto item = _xref->at(objNumber);
			auto reference = item.reference;

			if (item.reference->IsEmpty()) {
				auto rewind_pos = _input->tellg();
				BOOST_SCOPE_EXIT(&_input, &rewind_pos) {
					_input->seekg(rewind_pos);
				} BOOST_SCOPE_EXIT_END;
				auto parser = SpiritParser(this, *_input);
				return parser.readObject(item.offset);
			}

			return item.reference->GetReferencedObject();

			/*
			auto pos = _stream->tellg();

			_stream->seekg(item._reference->GetOffset());

			auto obj = _stream->readObject();
			_stream->seekg(pos);

			assert(obj->GetType() == Object::Type::IndirectObject);

			auto indirect = boost::dynamic_pointer_cast<IndirectObject>(obj);
			_cache.push_back(indirect);
			if (_cache.size() > 16)
				_cache.erase(_cache.begin());

			// TODO
			item._reference = indirect;

			return indirect;
			*/
		}

		SmartPtr<documents::Catalog> File::GetDocumentCatalog(void) const
		{
			if (!_initialized)
				throw new Exception("File has not been initialized yet");

			auto reference = _trailer->dictionary()->FindAs<IndirectObjectReferencePtr>(constant::Name::Root);
			auto dict = reference->GetReferencedObjectAs<DictionaryObjectPtr>();
			return new documents::Catalog(dict);
		}

		Deferred<files::Header> File::GetHeader(void) const { return _header; }
		Deferred<files::Trailer> File::GetTrailer(void) const { return _trailer; }
		Deferred<files::Xref> File::GetXref(void) const { return _xref; }
	}
}

using namespace gotchangpdf::files;

GOTCHANG_PDF_API FileHandle CALLING_CONVENTION File_Create(const char *filename)
{
	return reinterpret_cast<FileHandle>(new File(filename));
}

GOTCHANG_PDF_API void CALLING_CONVENTION File_Release(FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);
	delete file;
}

GOTCHANG_PDF_API int CALLING_CONVENTION File_Initialize(FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);

	// TODO
	file->Initialize();
	return 0;
}

GOTCHANG_PDF_API XrefHandle CALLING_CONVENTION File_Xref(FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);

	auto table = file->GetXref();
	auto ptr = table.AddRefGet();

	return reinterpret_cast<XrefHandle>(ptr);
}

GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION File_GetIndirectObject(
	FileHandle handle, int objNumber, int genNumber)
{
	File* file = reinterpret_cast<File*>(handle);

	auto item = file->GetIndirectObject(objNumber, genNumber);
	auto ptr = item.AddRefGet();

	//boost::intrusive_ptr_add_ref(ptr);
	return reinterpret_cast<IndirectObjectHandle>(ptr);
}

GOTCHANG_PDF_API CatalogHandle CALLING_CONVENTION File_GetDocumentCatalog(
	FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);

	auto item = file->GetDocumentCatalog();
	auto ptr = item.AddRefGet();

	//boost::intrusive_ptr_add_ref(ptr);
	return reinterpret_cast<CatalogHandle>(ptr);
}