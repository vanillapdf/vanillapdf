#include "precompiled.h"
#include "xref_chain.h"

namespace gotchangpdf
{
	namespace syntax
	{
		XrefUsedEntryBasePtr XrefChain::AllocateNewEntry()
		{
			for (types::big_uint i = m_next_allocation; i < std::numeric_limits<types::big_uint>::max(); ++i) {

				size_t eligible = 0;
				for (auto it = _list.begin(); it != _list.end(); it++) {
					auto xref = (*it);

					if (!xref->Contains(i)) {
						eligible++;
						continue;
					}

					auto item = xref->Find(i);
					if (item->GetUsage() != XrefEntryBase::Usage::Free) {
						continue;
					}

					// Disable free entries with max generation number
					if (item->GetGenerationNumber() == std::numeric_limits<types::ushort>::max()) {
						continue;
					}

					// TODO: I've tried reusing free entries in xref streams
					// but somehow it didn't worked. This would require
					// deleteing unused free entries and replacing them
					// is creating new pdf file or creating additional xref
					// for these new entries
					//eligible++;
				}

				if (eligible != _list.size()) {
					continue;
				}

				auto xref_it = _list.begin();
				auto xref = *xref_it;
				types::ushort gen_number = 0;

				//if (xref->Contains(i)) {
				//	auto item = xref->Find(i);
				//	if (item->GetUsage() == XrefEntryBase::Usage::Free) {
				//		gen_number = item->GetGenerationNumber() + 1;
				//	}
				//}

				XrefUsedEntryPtr new_entry(i, gen_number, -1);
				xref->Add(new_entry);

				m_next_allocation = i + 1;
				return new_entry;
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
			types::ushort genNumber) const
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
