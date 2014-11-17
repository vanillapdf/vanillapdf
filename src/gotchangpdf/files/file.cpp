#include "file.h"
#include "integer_object.h"
#include "indirect_object.h"
#include "indirect_object_reference.h"
#include "lexical_reverse_stream.h"
#include "exception.h"

#include "c_file.h"

#include <memory>
#include <cassert>
#include <iostream>

#include <boost/filesystem/operations.hpp>

//
#include "spirit_lexer.h"
#include "spirit_grammar.h"

//#include <boost/spirit/include/lex_generate_static_lexertl.hpp>
//

namespace gotchangpdf
{
	namespace files
	{
		using namespace std;
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

			_cache = vector<SmartPtr<IndirectObject>>();
			_xref = SmartPtr<Xref>(new Xref());
			_header = SmartPtr<Header>(new Header());
			_trailer = SmartPtr<Trailer>(new Trailer());

			if (!boost::filesystem::exists(_filename))
				throw new exceptions::Exception("File does not exist");

			_input = shared_ptr<FileDevice>(new FileDevice());
			_input->open(_filename,
				ios_base::in | ios_base::out | ios_base::binary);

			// Don't skip whitespace explicitly
			noskipws(*_input);

			lexical::SpiritLexer lexer;
			lexical::SpiritGrammar grammar(lexer);
			ast::IndirectObject obj;

			// Direct cast to pos_iterator_type is not possible
			lexical::base_iterator_type input_begin_base(*_input);
			lexical::base_iterator_type input_end_base;

			lexical::pos_iterator_type input_begin_pos(input_begin_base, input_end_base, _filename);
			lexical::pos_iterator_type input_end_pos;

			try
			{
				//fstream out("static_lexer.hpp", std::ios_base::out);
				//auto dfa = boost::spirit::lex::lexertl::generate_static_dfa(aa, out, "sl");
				auto result = lex::tokenize_and_parse(input_begin_pos, input_end_pos, lexer, grammar, obj);
				if (result)
				{
					return;
				}
				//out.close();
			}
			catch (std::exception e)
			{
			}
			catch (...)
			{
				
			}

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

		SmartPtr<IndirectObject> File::GetIndirectObject(unsigned int objNumber,
			unsigned int genNumber) const
		{
			if (!_initialized)
				throw new Exception("File has not been initialized yet");

			//if (_cache.)

			//TODO
			auto item = _xref->at(objNumber);
			return item.reference;
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

			auto reference = _trailer->dictionary()->FindAs<IndirectObjectReference>(constant::Name::Root);
			auto dict = reference->GetReferencedObjectAs<DictionaryObject>();
			return SmartPtr<documents::Catalog>(new documents::Catalog(dict));
		}
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