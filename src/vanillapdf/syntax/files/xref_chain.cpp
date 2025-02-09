#include "precompiled.h"

#include "syntax/files/xref_chain.h"

#include "syntax/exceptions/syntax_exceptions.h"

namespace vanillapdf {
namespace syntax {

XrefEntryBasePtr XrefChain::GetXrefEntry(types::big_uint obj_number, types::ushort gen_number) const {
    for (auto it = _list.begin(); it != _list.end(); it++) {
        auto xref = (*it);

        if (!xref->Contains(obj_number)) {
            continue;
        }

        auto item = xref->Find(obj_number);
        if (item->GetGenerationNumber() != gen_number) {
            continue;
        }

        assert(item->GetObjectNumber() == obj_number && item->GetGenerationNumber() == gen_number);
        return item;
    }

    spdlog::error("Xref entry {} {} was not found in the list", obj_number, gen_number);
    throw ObjectMissingException(obj_number, gen_number);
}

bool XrefChain::Contains(types::big_uint obj_number, types::ushort gen_number) const {
    for (auto it = _list.begin(); it != _list.end(); it++) {
        auto xref = (*it);

        if (!xref->Contains(obj_number)) {
            continue;
        }

        auto item = xref->Find(obj_number);
        if (item->GetGenerationNumber() != gen_number) {
            continue;
        }

        assert(item->GetObjectNumber() == obj_number && item->GetGenerationNumber() == gen_number);
        return true;
    }

    return false;
}

bool XrefChain::ReleaseEntry(XrefUsedEntryBasePtr entry) {

    for (auto& xref : _list) {
        auto object_number = entry->GetObjectNumber();

        if (!xref->Contains(object_number)) {
            continue;
        }

        auto item = xref->Find(object_number);
        if (item->GetGenerationNumber() != entry->GetGenerationNumber()) {
            continue;
        }

        bool removed = xref->Remove(entry);
        assert(removed && "Could not remove the xref entry");

        if (removed) {
            // Adobe acrobat has problems with gaps inside xref streams
            // I wasn't able to find out why, so lets be nice and insert
            // a fresh free entry as a placeholder for the item that is missing

            // TODO maybe set next free object number?
            XrefFreeEntryPtr freed_entry = make_deferred<XrefFreeEntry>(entry->GetObjectNumber(), static_cast<types::ushort>(0));
            xref->Add(freed_entry);
        }

        return removed;
    }

    return false;
}

} // syntax
} // vanillapdf
