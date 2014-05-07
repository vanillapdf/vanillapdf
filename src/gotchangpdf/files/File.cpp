#include "File.h"
#include "IntegerObject.h"
#include "IndirectObject.h"
#include "IndirectObjectReference.h"
#include "lexical/LexicalReverseStream.h"
#include "Exception.h"
#include "Export.h"

#include <memory>
#include <cassert>
#include <iostream>

namespace Pdf
{
	namespace Files
	{
		using namespace std;
		using namespace Streams::Lexical;
		using namespace Pdf::Lexical;

		File::File(const char *filename) : _input(nullptr), _cache(), _xref(new CrossReferenceInfo()), _header(new Header()), _trailer(new Trailer())
		{
			_input = shared_ptr<fstream>(new fstream);
			_input->open(filename, ios_base::in | ios_base::out | std::ifstream::binary);

			_stream = shared_ptr<Lexical::Parser>(new Lexical::Parser(shared_ptr<File>(this), _input));
		}

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
			_stream->seekg(ios_base::beg);
			*_stream >> *_header;

			ReverseStream reversed = ReverseStream(*_stream);
			reversed >> *_trailer;

			_stream->seekg(_trailer->xref_offset(), ios_base::beg);
			*_stream >> *_xref;

			_stream->ReadTokenWithType(Token::Type::TRAILER);
			_stream->ReadTokenWithType(Token::Type::EOL);

			// HACK
			*_stream >> *_trailer->dictionary();
		}

		boost::intrusive_ptr<IndirectObject> File::GetIndirectObject(int objNumber, int genNumber) const
		{
			//if (_cache.)

			//TODO
			auto item = _xref->At(objNumber);
			return item._reference;
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

		boost::intrusive_ptr<IndirectObject> File::GetDocumentCatalog(void) const
		{
			static const char root[] = "Root";
			auto reference = _trailer->dictionary()->Find(NameObject(Buffer(root, sizeof(root))));
			return boost::dynamic_pointer_cast<IndirectObjectReference>(reference)->GetReference();
		}

		boost::intrusive_ptr<Header> File::GetHeader(void) const { return _header; }
		boost::intrusive_ptr<Trailer> File::GetTrailer(void) const { return _trailer; }
		boost::intrusive_ptr<CrossReferenceInfo> File::GetCrossReferenceTable(void) const { return _xref; }

		Lexical::Parser File::GetParser(void) const { return Lexical::Parser(*_stream); }
	}
}

using namespace Pdf::Files;

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

	auto table = file->GetCrossReferenceTable();
	auto ptr = table.get();

	boost::intrusive_ptr_add_ref(ptr);
	return reinterpret_cast<XrefHandle>(ptr);
}

GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION File_GetIndirectObject(FileHandle handle, int objNumber, int genNumber)
{
	File* file = reinterpret_cast<File*>(handle);

	auto item = file->GetIndirectObject(objNumber, genNumber);
	auto ptr = item.get();

	boost::intrusive_ptr_add_ref(ptr);
	return reinterpret_cast<IndirectObjectHandle>(ptr);
}

GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION File_GetDocumentCatalog(FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);

	auto item = file->GetDocumentCatalog();
	auto ptr = item.get();

	boost::intrusive_ptr_add_ref(ptr);
	return reinterpret_cast<IndirectObjectHandle>(ptr);
}
