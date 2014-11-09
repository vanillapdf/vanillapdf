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

#include <boost/spirit/include/lex_generate_static_lexertl.hpp>
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

			lexical::SpiritLexer lexer;
			SpiritGrammar gram(lexer);
			ast::IndirectObject obj;
			ast::BooleanObject boo;
			/*
			string str("4 0 obj"
				"<<"
				"/ Type / Page"
				"/ Parent 5 0 R"
				"/ Resources <<"
				"/ Font <<"
				"/ F0 6 0 R"
				"/ F1 7 0 R"
				">>"
				"/ ProcSet 2 0 R"
				">>"
				"/ Contents 9 0 R"
				">>"
				"endobj");
				*/
			/*
			string str("4 0 obj\n"
			"115\n"
			"endobj");
				*/

			string str("false");

			//std::string::const_iterator beg = str.begin();
			//std::string::const_iterator end = str.end();

			std::string::iterator it = str.begin();
			lexical::pos_iterator_type iter(str.begin(), str.end(), "test");
			lexical::pos_iterator_type end;

			//lexical::SpiritLexer::iterator_type position_begin(str.begin(), str.end(), "test");
			//lexical::SpiritLexer::iterator_type position_end;

			try
			{
				using boost::spirit::ascii::space;

				//fstream out("static_lexer.hpp", std::ios_base::out);
				//auto dfa = boost::spirit::lex::lexertl::generate_static_dfa(aa, out, "sl");
				//auto test = lex::tokenize(begin, end, aa, gram);
				//bool result = boost::spirit::qi::phrase_parse(iter, end, gram, qi::in_state("WS")[lexer.self]);
				auto result = lex::tokenize_and_parse(iter, end, lexer, gram, boo);
				if (result)
				{
					auto which = boo.which();
					auto yes = boost::get<ast::True>(boo);
					auto no = boost::get<ast::False>(boo);
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