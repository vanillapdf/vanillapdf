#include "precompiled.h"
#include "xref_chain.h"

namespace gotchangpdf
{
	namespace syntax
	{
		XrefEntryBasePtr XrefChain::AllocateNewEntry()
		{
			for (types::big_uint i = 0; i < std::numeric_limits<types::big_uint>::max(); ++i) {

				bool found = true;
				for (auto it = _list.begin(); it != _list.end(); it++) {
					auto xref = (*it);

					if (!xref->Contains(i)) {
						continue;
					}

					auto item = xref->Find(i);
					if (item->GetUsage() != XrefEntryBase::Usage::Free) {
						found = false;
						break;
					}

					if (item->GetGenerationNumber() == std::numeric_limits<types::ushort>::max()) {
						found = false;
						break;
					}
				}

				if (!found) {
					continue;
				}

				auto xref_it = _list.begin();
				auto xref = *xref_it;

				if (xref->GetType() == XrefBase::Type::Stream) {
					auto entries = xref->Entries();

					types::big_uint obj_stream_number = 0;
					types::uinteger highest_index = 0;
					for (size_t j = 0; j < entries.size(); ++j) {
						auto item = entries.at(j);
						assert(item->GetUsage() == XrefEntryBase::Usage::Compressed);
						if (item->GetUsage() != XrefEntryBase::Usage::Compressed) {
							continue;
						}

						auto compressed = XrefUtils::ConvertTo<XrefCompressedEntryPtr>(item);

						assert(obj_stream_number == 0 || obj_stream_number == compressed->GetObjectStreamNumber());
						obj_stream_number = compressed->GetObjectStreamNumber();

						if (compressed->GetIndex() > highest_index)
							highest_index = compressed->GetIndex();
					}

					assert(0 != obj_stream_number && "Invalid object stream number");
					if (0 == obj_stream_number) {
						throw GeneralException("Invalid object stream number");
					}

					types::uinteger obj_index = SafeAddition<types::uinteger>(highest_index, 1);
					XrefCompressedEntryPtr new_entry(i, static_cast<types::ushort>(0), obj_stream_number, obj_index);
					xref->Add(new_entry);
					return new_entry;
				}

				if (xref->GetType() == XrefBase::Type::Table) {
					types::ushort gen_number = 0;
					if (xref->Contains(i)) {
						auto item = xref->Find(i);
						if (item->GetUsage() == XrefEntryBase::Usage::Free) {
							gen_number = item->GetGenerationNumber() + 1;
						}
					}

					XrefUsedEntryPtr new_entry(i, gen_number, -1);
					xref->Add(new_entry);
					return new_entry;
				}

				throw GeneralException("Unknown xref type");
			}

			throw GeneralException("Unable to allocate new entry");
		}

		XrefEntryBasePtr XrefChain::GetXrefEntry(types::big_uint objNumber,
			types::ushort genNumber)
		{
			for (auto it = _list.begin(); it != _list.end(); it++) {
				auto xref = (*it);

				if (!xref->Contains(objNumber))
					continue;

				auto item = xref->Find(objNumber);
				if (item->GetGenerationNumber() != genNumber)
					continue;

				assert(item->GetObjectNumber() == objNumber && item->GetGenerationNumber() == genNumber);
				return item;
			}

			throw ObjectMissingException(objNumber, genNumber);
		}

		bool XrefChain::Contains(types::big_uint objNumber,
			types::ushort genNumber)
		{
			for (auto it = _list.begin(); it != _list.end(); it++) {
				auto xref = (*it);
				if (!xref->Contains(objNumber)) {
					continue;
				}

				auto item = xref->Find(objNumber);
				if (item->GetGenerationNumber() != genNumber)
					continue;

				assert(item->GetObjectNumber() == objNumber && item->GetGenerationNumber() == genNumber);
				return true;
			}

			return false;
		}
	}
}
