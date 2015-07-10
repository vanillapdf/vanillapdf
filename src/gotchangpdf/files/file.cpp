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
				if (xref->GetType() == Xref::Type::TABLE) {
					stream.ReadTokenWithType(Token::Type::TRAILER);
					stream.ReadTokenWithType(Token::Type::EOL);

					// HACK
					auto trailer_dict = stream.ReadDirectObjectWithType<DictionaryObjectPtr>(_input->tellg());
					trailer->SetDictionary(trailer_dict);

					assert(trailer->GetXrefOffset() == offset);
					item = XrefWithMetadataPtr(new XrefWithMetadata(xref, trailer));
				} else if (xref->GetType() == Xref::Type::STREAM) {
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

		XrefEntryPtr File::GetXrefEntry(types::integer objNumber,
			types::ushort genNumber)
		{
			// TODO xref entry should be a map, instead of vector for searching

			if (!_initialized)
				throw Exception("File has not been initialized yet");

			for (auto it = _xref->Begin(); *it != *_xref->End(); (*it)++) {
				auto xref = it->Value()->GetXref();
				for (auto item : *xref) {
					if (item->GetObjectNumber() == objNumber && item->GetGenerationNumber() == genNumber)
						return item;
				}
			}

			std::stringstream ss;
			ss << "Item " << objNumber << " " << genNumber << " was not found";
			throw Exception(ss.str());
		}

		bool File::IsIndirectObjectIntialized(types::integer objNumber,
			types::ushort genNumber)
		{
			if (!_initialized)
				throw Exception("File has not been initialized yet");

			auto item = GetXrefEntry(objNumber, genNumber);
			return item->Initialized();
		}

		DirectObject File::GetIndirectObject(types::integer objNumber,
			types::ushort genNumber)
		{
			LOG_DEBUG << "GetIndirectObject " << objNumber << " and " << genNumber;

			if (!_initialized)
				throw Exception("File has not been initialized yet");

			auto item = GetXrefEntry(objNumber, genNumber);
			if (!item->InUse())
				throw Exception("Required object is marked as free");


			switch (item->GetUsage()) {
			case XrefEntry::Usage::USED:
			{
				auto used = dynamic_wrapper_cast<XrefUsedEntry>(item);

				if (!used->Initialized()) {
					auto rewind_pos = _input->tellg();
					BOOST_SCOPE_EXIT(_input, rewind_pos) {
						_input->seekg(rewind_pos);
					} BOOST_SCOPE_EXIT_END;
					auto parser = SpiritParser(this, *_input);
					auto offset = used->GetOffset();
					auto object = parser.ReadDirectObject(offset);
					used->SetReference(object);
					used->SetInitialized(true);
				}

				return used->GetReference();
			}
			case XrefEntry::Usage::COMPRESSED:
			{
				auto compressed = dynamic_wrapper_cast<XrefCompressedEntry>(item);

				if (!compressed->Initialized()) {
					auto stm = GetIndirectObject(compressed->GetObjectStreamNumber(), 0);

					ObjectVisitor<StreamObjectPtr> stream_visitor;
					auto converted = stm.apply_visitor(stream_visitor);
					auto header = converted->GetHeader();
					auto size = header->FindAs<IntegerObjectPtr>(constant::Name::N);
					auto first = header->FindAs<IntegerObjectPtr>(constant::Name::First);
					auto body = converted->GetBodyDecoded();

					auto stream = body->ToStringStream();
					auto parser = SpiritParser(this, stream);
					auto stream_entries = parser.ReadObjectStreamEntries(first->Value(), size->Value());
					for (auto stream_entry : stream_entries)
					{
						ObjectBaseVisitor visitor;
						auto stream_entry_base = stream_entry.apply_visitor(visitor);
						auto object_number = stream_entry_base->GetObjectNumber();

						auto stream_entry_xref = GetXrefEntry(object_number, 0);
						if (stream_entry_xref->GetUsage() != XrefEntry::Usage::COMPRESSED)
							throw exceptions::Exception("Compressed entry type expected");

						auto stream_compressed_entry_xref = dynamic_wrapper_cast<XrefCompressedEntry>(stream_entry_xref);
						stream_compressed_entry_xref->SetReference(stream_entry);
						item->SetInitialized(true);
					}

					if (!compressed->Initialized())
						throw exceptions::Exception("Item was not found in object stream");
				}

				return compressed->GetReference();
			}
			default:
				throw Exception("Unknown entry type");
			}
		}

		SmartPtr<documents::Catalog> File::GetDocumentCatalog(void) const
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
