#include "precompiled.h"
#include "xref.h"

#include "file.h"
#include "xref_chain.h"
#include "parser.h"

namespace gotchangpdf
{
	namespace syntax
	{
		void XrefUsedEntry::SetReference(ObjectPtr ref)
		{
			_reference->Unsubscribe(this);
			_reference = ref;
			_reference->Subscribe(this);

			if (_initialized)
				SetDirty();

			SetInitialized();
		}

		void XrefUsedEntry::Initialize(void)
		{
			if (Initialized())
				return;

			auto locked_file = _file.lock();
			if (!locked_file)
				throw FileDisposedException();

			auto input = locked_file->GetInputStream();
			auto rewind_pos = input->tellg();
			SCOPE_GUARD_CAPTURE_VALUES(input->seekg(rewind_pos));
			auto parser = Parser(_file, *input);
			auto object = parser.ReadIndirectObject(_offset);
			SetReference(object);
			SetInitialized(true);
		}

		void XrefCompressedEntry::SetReference(ObjectPtr ref)
		{
			_reference->Unsubscribe(this);
			_reference = ref;
			_reference->Subscribe(this);

			if (_initialized)
				SetDirty();

			SetInitialized();
		}

		void XrefCompressedEntry::Initialize(void)
		{
			if (_initialized)
				return;

			auto locked_file = _file.lock();
			if (!locked_file)
				throw FileDisposedException();

			auto chain = locked_file->GetXrefChain();
			auto stm = locked_file->GetIndirectObject(_object_stream_number, 0);

			auto converted = ObjectUtils::ConvertTo<StreamObjectPtr>(stm);
			auto header = converted->GetHeader();
			auto size = header->FindAs<IntegerObjectPtr>(constant::Name::N);
			auto first = header->FindAs<IntegerObjectPtr>(constant::Name::First);
			auto body = converted->GetBody();

			auto stream = body->ToStringStream();
			auto parser = Parser(_file, stream);
			auto stream_entries = parser.ReadObjectStreamEntries(first->GetValue(), size->SafeConvert<size_t>());
			for (auto stream_entry : stream_entries) {
				auto object_number = stream_entry->GetObjectNumber();
				auto stream_entry_xref = chain->GetXrefEntry(object_number, 0);

				assert(stream_entry_xref->GetUsage() == XrefEntryBase::Usage::Compressed && "Stream entry has different usage than Compressed");
				if (stream_entry_xref->GetUsage() != XrefEntryBase::Usage::Compressed) {
					continue;
				}

				auto stream_compressed_entry_xref = XrefUtils::ConvertTo<XrefCompressedEntryPtr>(stream_entry_xref);
				stream_compressed_entry_xref->SetReference(stream_entry);
				stream_compressed_entry_xref->SetInitialized(true);
			}

			assert(_initialized);
		}
	}
}
