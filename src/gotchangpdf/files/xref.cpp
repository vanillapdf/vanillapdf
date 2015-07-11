#include "precompiled.h"
#include "xref.h"

#include "file.h"
#include "xref_chain.h"
#include "smart_ptr.h"
#include "spirit_parser.h"

#include <boost/scope_exit.hpp>


namespace gotchangpdf
{
	namespace files
	{
		void XrefUsedEntry::Initialize(void)
		{
			if (Initialized())
				return;

			auto weak_input = _file->GetInputStream();
			if (auto input = weak_input.lock()) {
				auto rewind_pos = input->tellg();
				BOOST_SCOPE_EXIT(input, rewind_pos)
				{
					input->seekg(rewind_pos);
				} BOOST_SCOPE_EXIT_END;
				auto parser = lexical::SpiritParser(_file, *input);
				auto object = parser.ReadDirectObject(_offset);
				SetReference(object);
				SetInitialized(true);
			} else {
				throw exceptions::Exception("Could not lock input stream");
			}
		}

		void XrefCompressedEntry::Initialize(void)
		{
			if (Initialized())
				return;

			auto chain = _file->GetXrefChain();
			auto stm = _file->GetIndirectObject(_object_stream_number, 0);

			ObjectVisitor<StreamObjectPtr> stream_visitor;
			auto converted = stm.apply_visitor(stream_visitor);
			auto header = converted->GetHeader();
			auto size = header->FindAs<IntegerObjectPtr>(constant::Name::N);
			auto first = header->FindAs<IntegerObjectPtr>(constant::Name::First);
			auto body = converted->GetBodyDecoded();

			auto stream = body->ToStringStream();
			auto parser = lexical::SpiritParser(_file, stream);
			auto stream_entries = parser.ReadObjectStreamEntries(first->Value(), size->Value());
			for (auto stream_entry : stream_entries) {
				ObjectBaseVisitor visitor;
				auto stream_entry_base = stream_entry.apply_visitor(visitor);
				auto object_number = stream_entry_base->GetObjectNumber();

				auto stream_entry_xref = chain->GetXrefEntry(object_number, 0);
				if (stream_entry_xref->GetUsage() != XrefEntry::Usage::Compressed)
					throw exceptions::Exception("Compressed entry type expected");

				auto stream_compressed_entry_xref = dynamic_wrapper_cast<XrefCompressedEntry>(stream_entry_xref);
				stream_compressed_entry_xref->SetReference(stream_entry);
				stream_entry_xref->SetInitialized(true);
			}

			if (!Initialized())
				throw exceptions::Exception("Item was not found in object stream");
		}
	}
}
