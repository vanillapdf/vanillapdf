#include "precompiled.h"
#include "file.h"

#include "raw_reverse_stream.h"
#include "spirit_parser.h"
#include "exception.h"

#include "xref_chain.h"
#include "header.h"

namespace gotchangpdf
{
	namespace syntax
	{
		using namespace std;

		File::File(std::string filename)
			: _filename(filename)
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

		void File::Initialize(std::shared_ptr<File> holder)
		{
			LOG_DEBUG << "Initialize";

			if (_initialized)
				return;

			_input = make_shared<FileDevice>();
			_input->open(_filename,
				ios_base::in | ios_base::out | ios_base::binary);

			if (!_input || !_input->good())
				throw Exception("Could not open file");

			SpiritParser stream = SpiritParser(holder, *_input);

			stream.seekg(ios_base::beg);
			stream >> *_header;

			types::integer offset;
			{
				ReverseStream raw_reversed(*_input);
				SpiritParser reverse_stream = SpiritParser(holder, raw_reversed);
				offset = reverse_stream.ReadLastXrefOffset();
			}

			do {
				XrefBaseVisitor visitor;
				auto xref_variant = stream.ReadXref(offset);
				auto xref = xref_variant.apply_visitor(visitor);

				_xref->Append(xref_variant);
				if (!xref->GetTrailerDictionary()->Contains(constant::Name::Prev)) {
					break;
				}

				offset = xref->GetTrailerDictionary()->FindAs<IntegerObjectPtr>(constant::Name::Prev)->Value();
			} while (true);

			_initialized = true;
		}

		DirectObject File::GetIndirectObject(types::integer objNumber,
			types::ushort genNumber)
		{
			LOG_DEBUG << "GetIndirectObject " << objNumber << " and " << genNumber;

			if (!_initialized)
				throw Exception("File has not been initialized yet");

			XrefEntryBaseVisitor base_visitor;
			auto item = _xref->GetXrefEntry(objNumber, genNumber);
			auto item_base = item.apply_visitor(base_visitor);

			if (!item_base->InUse())
				throw Exception("Required object is marked as free");

			switch (item_base->GetUsage()) {
			case XrefEntryBase::Usage::Used:
			{
				XrefEntryVisitor<XrefUsedEntryPtr> visitor;
				auto used = item.apply_visitor(visitor);
				return used->GetReference();
			}
			case XrefEntryBase::Usage::Compressed:
			{
				XrefEntryVisitor<XrefCompressedEntryPtr> visitor;
				auto compressed = item.apply_visitor(visitor);
				return compressed->GetReference();
			}
			case XrefEntryBase::Usage::Null:
				throw Exception("Specified entry " + std::to_string(objNumber) + " " + std::to_string(genNumber) + " obj is missing");
			default:
				throw Exception("Unknown entry type");
			}
		}

		//semantics::CatalogPtr File::GetDocumentCatalog(void) const
		//{
		//	if (!_initialized)
		//		throw Exception("File has not been initialized yet");

		//	XrefBaseVisitor visitor;
		//	auto xref_variant = _xref->Begin()->Value();
		//	auto xref = xref_variant.apply_visitor(visitor);
		//	auto dictionary = xref->GetTrailerDictionary();
		//	auto root = dictionary->FindAs<DictionaryObjectPtr>(constant::Name::Root);
		//	return new semantics::Catalog(root);
		//}

		HeaderPtr File::GetHeader(void) const { return _header; }
		XrefChainPtr File::GetXrefChain(void) const { return _xref; }
	}
}
