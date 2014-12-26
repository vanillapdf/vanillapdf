#include "file.h"

#include "indirect_object.h"
#include "lexical_reverse_stream.h"
#include "spirit_parser.h"
#include "exception.h"

#include "xref.h"
#include "log.h"
#include "header.h"
#include "trailer.h"
#include "parser.h"
#include "catalog.h"

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

		File::File(std::string filename) : _filename(filename)
		{
			LOG_DEBUG << "File constructor";
		}

		File::~File(void)
		{
			LOG_DEBUG << "File destructor";

			if (nullptr != _input)
			{
				_input->close();
				_input = nullptr;
			}

			_cache.clear();
		}

		void File::Initialize(void)
		{
			LOG_DEBUG << "Initialize";

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

			stream.seekg(_trailer->GetXrefOffset(), ios_base::beg);
			stream >> *_xref;

			stream.ReadTokenWithType(Token::Type::TRAILER);
			stream.ReadTokenWithType(Token::Type::EOL);

			// HACK
			SpiritParser spirit_parser = SpiritParser(this, *_input);
			auto trailer_dict = spirit_parser.ReadDirectObjectWithType<DictionaryObjectPtr>(_input->tellg());
			_trailer->SetDictionary(trailer_dict);

			_initialized = true;
		}

		bool File::IsIndirectObjectIntialized(types::integer objNumber,
			types::ushort genNumber)
		{
			auto item = _xref->at(objNumber);
			return item->Initialized();
		}

		IndirectObjectPtr File::GetIndirectObject(types::integer objNumber,
			types::ushort genNumber)
		{
			LOG_DEBUG << "GetIndirectObject " << objNumber << " and " << genNumber;

			if (!_initialized)
				throw new Exception("File has not been initialized yet");

			auto item = _xref->at(objNumber);
			if (!item->Initialized()) {
				auto rewind_pos = _input->tellg();
				BOOST_SCOPE_EXIT(&_input, &rewind_pos) {
					_input->seekg(rewind_pos);
				} BOOST_SCOPE_EXIT_END;
				auto parser = SpiritParser(this, *_input);
				auto offset = item->GetOffset();
				auto object = parser.ReadIndirectObject(offset);
				item->SetReference(object);
				item->SetInitialized(true);
			}

			return item->GetReference();;
		}

		SmartPtr<documents::Catalog> File::GetDocumentCatalog(void) const
		{
			if (!_initialized)
				throw new Exception("File has not been initialized yet");

			auto reference = _trailer->GetDictionary()->FindAs<IndirectObjectReferencePtr>(constant::Name::Root);
			auto dict = reference->GetReferencedObjectAs<DictionaryObjectPtr>();
			return new documents::Catalog(dict);
		}

		Deferred<Header> File::GetHeader(void) const { return _header; }
		Deferred<Trailer> File::GetTrailer(void) const { return _trailer; }
		Deferred<Xref> File::GetXref(void) const { return _xref; }
	}
}
