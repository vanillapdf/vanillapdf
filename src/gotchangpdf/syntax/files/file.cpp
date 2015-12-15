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

		std::string extract_filename(const std::string& filepath)
		{
			int pos = filepath.rfind("\\");
			if (pos == std::string::npos)
				pos = -1;

			return std::string(filepath.begin() + pos + 1, filepath.end());
		}

		File::File(std::string path)
			: _full_path(path)
		{
			LOG_DEBUG << "File constructor";
			_filename = extract_filename(path);
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
			_input->open(_full_path,
				ios_base::in | ios_base::out | ios_base::binary | ios::ate);

			if (!_input || !_input->good())
				throw GeneralException("Could not open file");

			auto file_size = _input->tellg();
			SpiritParser stream = SpiritParser(holder, *_input);

			stream.seekg(ios_base::beg);
			stream >> *_header;

			types::integer offset;
			{
				ReverseStream raw_reversed(*_input, file_size);
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
				throw FileNotInitializedException(_filename);

			auto item = _xref->GetXrefEntry(objNumber, genNumber);

			if (!item->InUse())
				throw ObjectMissingException(objNumber, genNumber);

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
				LOG_ERROR << "Xref entry type is null for object " << objNumber << " " << genNumber;
				throw ObjectMissingException(objNumber, genNumber);
			case XrefEntryBase::Usage::Free:
				LOG_ERROR << "Xref entry type is free for object " << objNumber << " " << genNumber << " and InUse() returned true";
				assert(!"Current entry is supposed to be InUse(), while it's type is Free");
				throw ObjectMissingException(objNumber, genNumber);
			default:
				throw GeneralException("Unknown entry type");
			}
		}

		HeaderPtr File::GetHeader(void) const
		{
			if (!_initialized)
				throw FileNotInitializedException(_filename);

			return _header;
		}

		XrefChainPtr File::GetXrefChain(void) const
		{
			if (!_initialized)
				throw FileNotInitializedException(_filename);

			return _xref;
		}
	}
}
