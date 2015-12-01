#include "precompiled.h"
#include "xref.h"

#include "file.h"
#include "xref_chain.h"
#include "spirit_parser.h"
#include "object_visitors.h"

#include <boost/scope_exit.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		void XrefUsedEntry::Initialize(void)
		{
			if (Initialized())
				return;

			auto locked_file = _file.lock();
			if (!locked_file)
				throw Exception("File already disposed");

			auto weak_input = locked_file->GetInputStream();
			if (auto input = weak_input.lock()) {
				auto rewind_pos = input->tellg();
				BOOST_SCOPE_EXIT(input, rewind_pos)
				{
					input->seekg(rewind_pos);
				} BOOST_SCOPE_EXIT_END;
				auto parser = SpiritParser(_file, *input);
				auto object = parser.ReadDirectObject(_offset);
				SetReference(object);
				SetInitialized(true);
			} else {
				throw Exception("Could not lock input stream");
			}
		}

		void XrefCompressedEntry::Initialize(void)
		{
			if (Initialized())
				return;

			auto locked_file = _file.lock();
			if (!locked_file)
				throw Exception("File already disposed");

			auto chain = locked_file->GetXrefChain();
			auto stm = locked_file->GetIndirectObject(_object_stream_number, 0);

			ConversionVisitor<StreamObjectPtr> stream_visitor;
			auto converted = stm.apply_visitor(stream_visitor);
			auto header = converted->GetHeader();
			auto size = header->FindAs<IntegerObjectPtr>(constant::Name::N);
			auto first = header->FindAs<IntegerObjectPtr>(constant::Name::First);
			auto body = converted->GetBodyDecoded();

			auto stream = body->ToStringStream();
			auto parser = syntax::SpiritParser(_file, stream);
			auto stream_entries = parser.ReadObjectStreamEntries(first->Value(), size->Value());
			for (auto stream_entry : stream_entries) {
				ObjectBaseVisitor visitor;
				auto stream_entry_base = stream_entry.apply_visitor(visitor);
				auto object_number = stream_entry_base->GetObjectNumber();

				XrefEntryBaseVisitor xref_base_visitor;
				auto stream_entry_xref_variant = chain->GetXrefEntry(object_number, 0);
				auto stream_entry_xref_base = stream_entry_xref_variant.apply_visitor(xref_base_visitor);

				if (stream_entry_xref_base->GetUsage() != XrefEntryBase::Usage::Compressed)
					throw Exception("Compressed entry type expected");

				XrefEntryVisitor<XrefCompressedEntryPtr> xref_compressed_visitor;
				auto stream_compressed_entry_xref = stream_entry_xref_variant.apply_visitor(xref_compressed_visitor);
				stream_compressed_entry_xref->SetReference(stream_entry);
				stream_compressed_entry_xref->SetInitialized(true);
			}

			if (!Initialized())
				throw Exception("Item was not found in object stream");
		}
	}
}
