#include "File.h"
#include "integer_object.h"
#include "indirect_object.h"
#include "indirect_object_reference.h"
#include "lexical_reverse_stream.h"
#include "exception.h"
#include "export.h"

#include <memory>
#include <cassert>
#include <iostream>

namespace gotchangpdf
{
	namespace files
	{
		using namespace std;
		using namespace lexical;

		File::File(const char *filename) : _input(nullptr), _cache(), _xref(new Xref()), _header(new Header()), _trailer(new Trailer())
		{
			_input = shared_ptr<fstream>(new fstream);
			_input->open(filename, ios_base::in | ios_base::out | std::ifstream::binary);

			_stream = shared_ptr<lexical::Parser>(new lexical::Parser(shared_ptr<File>(this), _input));
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

		ObjectReferenceWrapper<IndirectObject> File::GetIndirectObject(int objNumber, int genNumber) const
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

		ObjectReferenceWrapper<documents::Catalog> File::GetDocumentCatalog(void) const
		{
			static const char root[] = "Root";
			auto reference = _trailer->dictionary()->Find(NameObject(Buffer(root, sizeof(root))));
			auto indirect = reference.GetAs<IndirectObjectReference>()->GetReference();
			return ObjectReferenceWrapper<documents::Catalog>(new documents::Catalog(*indirect));
		}

		ObjectReferenceWrapper<Header> File::GetHeader(void) const { return _header; }
		ObjectReferenceWrapper<Trailer> File::GetTrailer(void) const { return _trailer; }
		ObjectReferenceWrapper<Xref> File::GetXref(void) const { return _xref; }

		lexical::Parser File::GetParser(void) const { return lexical::Parser(*_stream); }

		#pragma region DllInterface

		IFile* IFile::Create(const char* filename)
		{
			return reinterpret_cast<IFile*>(new File(filename));
		}

		void IFile::Initialize(void)
		{
			auto removed = const_cast<IFile*>(this);
			auto obj = reinterpret_cast<File*>(removed);

			return obj->Initialize();
		}

		IXref* IFile::GetXref(void) const
		{
			auto removed = const_cast<IFile*>(this);
			auto obj = reinterpret_cast<File*>(removed);

			return reinterpret_cast<IXref*>(obj->GetXref().AddRefGet());
		}

		IIndirectObject* IFile::GetIndirectObject(int objNumber, int genNumber) const
		{
			auto removed = const_cast<IFile*>(this);
			auto obj = reinterpret_cast<File*>(removed);

			return reinterpret_cast<IIndirectObject*>(obj->GetIndirectObject(objNumber, genNumber).AddRefGet());
		}

		IFile::~IFile() {}

		#pragma endregion
	}
}
