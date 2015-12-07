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
				auto xref = stream.ReadXref(offset);
				_xref->Append(xref);

				if (xref->GetTrailerDictionary()->Contains(constant::Name::XRefStm)) {
					auto stm_offset = xref->GetTrailerDictionary()->FindAs<IntegerObjectPtr>(constant::Name::XRefStm)->Value();
					auto xref_stm = stream.ReadXref(stm_offset);

					assert(!xref_stm->GetTrailerDictionary()->Contains(constant::Name::Prev));
					_xref->Append(xref_stm);
				}

				if (!xref->GetTrailerDictionary()->Contains(constant::Name::Prev)) {
					break;
				}

				offset = xref->GetTrailerDictionary()->FindAs<IntegerObjectPtr>(constant::Name::Prev)->Value();
			} while (true);

			_initialized = true;
		}

		ObjectPtr File::GetIndirectObject(types::integer objNumber,
			types::ushort genNumber)
		{
			LOG_DEBUG << "GetIndirectObject " << objNumber << " and " << genNumber;

			if (!_initialized)
				throw Exception("File has not been initialized yet");

			auto item = _xref->GetXrefEntry(objNumber, genNumber);

			if (!item->InUse())
				throw Exception("Required object is marked as free");

			switch (item->GetUsage()) {
			case XrefEntryBase::Usage::Used:
			{
				auto used = XrefUtils::ConvertTo<XrefUsedEntryPtr>(item);
				return used->GetReference();
			}
			case XrefEntryBase::Usage::Compressed:
			{
				auto compressed = XrefUtils::ConvertTo<XrefCompressedEntryPtr>(item);
				return compressed->GetReference();
			}
			case XrefEntryBase::Usage::Null:
				throw Exception("Specified entry " + std::to_string(objNumber) + " " + std::to_string(genNumber) + " obj is missing");
			default:
				throw Exception("Unknown entry type");
			}
		}

		HeaderPtr File::GetHeader(void) const { return _header; }
		XrefChainPtr File::GetXrefChain(void) const { return _xref; }
	}
}
