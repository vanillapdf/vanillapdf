#include "precompiled.h"
#include "file.h"

#include "lexical_reverse_stream.h"
#include "spirit_parser.h"
#include "exception.h"

#include "xref_chain.h"
#include "log.h"
#include "header.h"
#include "trailer.h"
#include "catalog.h"

#include <memory>
#include <cassert>
#include <iostream>

#include <boost/scope_exit.hpp>
//#include <boost/typeof/incr_registration_group.hpp>

namespace gotchangpdf
{
	namespace files
	{
		using namespace std;
		using namespace lexical;
		using namespace exceptions;

		File::File(std::string filename)
			: _filename(filename),
			_header(new Header()),
			_xref(new XrefChain())
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

			_cache = vector<DirectObject>();

			//TODO check if file exists

			_input = shared_ptr<FileDevice>(new FileDevice());
			_input->open(_filename,
				ios_base::in | ios_base::out | ios_base::binary);

			if (!_input || !_input->good())
				throw exceptions::Exception("Could not open file");

			SpiritParser stream = SpiritParser(this, *_input);

			stream.seekg(ios_base::beg);
			stream >> *_header;

			TrailerPtr trailer(new Trailer());
			ReverseStream reversed = ReverseStream(*_input);
			reversed >> *trailer;

			auto offset = trailer->GetXrefOffset();
			do {
				XrefWithMetadataPtr item;

				// TODO read trailer
				auto xref = stream.ReadXref(offset);
				if (xref->GetType() == Xref::Type::Table) {
					stream.ReadTokenWithType(Token::Type::TRAILER);
					stream.ReadTokenWithType(Token::Type::EOL);

					// HACK
					auto trailer_dict = stream.ReadDirectObjectWithType<DictionaryObjectPtr>(_input->tellg());
					trailer->SetDictionary(trailer_dict);

					stream.ReadTokenWithType(Token::Type::EOL);
					stream.ReadTokenWithType(Token::Type::START_XREF);
					stream.ReadTokenWithType(Token::Type::EOL);

					IntegerObject trailer_offset;
					stream >> trailer_offset;

					stream.ReadTokenWithType(Token::Type::EOL);
					//stream.ReadTokenWithType(Token::Type::END_OF_FILE); // TODO

					// This actually does not work
					//assert(trailer_offset == offset);
					trailer->SetXrefOffset(offset);

					item = XrefWithMetadataPtr(new XrefWithMetadata(xref, trailer));
				} else if (xref->GetType() == Xref::Type::Stream) {
					auto xref_stream = dynamic_wrapper_cast<XrefStream>(xref);
					item = XrefWithMetadataPtr(new XrefWithMetadata(xref_stream, offset));
				} else {
					throw Exception("Unknown xref type");
				}

				_xref->Append(item);
				if (item->GetDictionary()->Contains(constant::Name::Prev)) {
					offset = item->GetDictionary()->FindAs<IntegerObjectPtr>(constant::Name::Prev)->Value();
				} else {
					break;
				}
			} while (true);

			_initialized = true;
		}

		DirectObject File::GetIndirectObject(types::integer objNumber,
			types::ushort genNumber)
		{
			LOG_DEBUG << "GetIndirectObject " << objNumber << " and " << genNumber;

			if (!_initialized)
				throw Exception("File has not been initialized yet");

			auto item = _xref->GetXrefEntry(objNumber, genNumber);
			if (!item->InUse())
				throw Exception("Required object is marked as free");

			switch (item->GetUsage()) {
			case XrefEntry::Usage::Used:
			{
				auto used = dynamic_wrapper_cast<XrefUsedEntry>(item);
				return used->GetReference();
			}
			case XrefEntry::Usage::Compressed:
			{
				auto compressed = dynamic_wrapper_cast<XrefCompressedEntry>(item);
				return compressed->GetReference();
			}
			default:
				throw Exception("Unknown entry type");
			}
		}

		CatalogPtr File::GetDocumentCatalog(void) const
		{
			if (!_initialized)
				throw Exception("File has not been initialized yet");

			auto dictionary = _xref->Begin()->Value()->GetDictionary();
			auto reference = dictionary->FindAs<IndirectObjectReferencePtr>(constant::Name::Root);
			auto dict = reference->GetReferencedObjectAs<DictionaryObjectPtr>();
			return new documents::Catalog(dict);
		}

		HeaderPtr File::GetHeader(void) const { return _header; }
		XrefChainPtr File::GetXrefChain(void) const { return _xref; }
	}
}
