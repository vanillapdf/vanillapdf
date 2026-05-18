#include "precompiled.h"

#include "syntax/files/xref_chain.h"

#include "syntax/utils/output_pointer.h"
#include "syntax/exceptions/syntax_exceptions.h"

namespace vanillapdf {
namespace syntax {

XrefEntryBasePtr XrefChain::GetXrefEntry(types::big_uint obj_number, types::ushort gen_number) const {
    for (auto it = _list.begin(); it != _list.end(); it++) {
        auto xref = (*it);

        OutputXrefEntryBasePtr found;
        if (!xref->TryFind(obj_number, found)) {
            continue;
        }

        if (found->GetGenerationNumber() != gen_number) {
            continue;
        }

        assert(found->GetObjectNumber() == obj_number && found->GetGenerationNumber() == gen_number);
        return *found;
    }

    spdlog::error("Xref entry {} {} was not found in the list", obj_number, gen_number);
    throw ObjectMissingException(obj_number, gen_number);
}

bool XrefChain::Contains(types::big_uint obj_number, types::ushort gen_number) const {
    for (auto it = _list.begin(); it != _list.end(); it++) {
        auto xref = (*it);

        OutputXrefEntryBasePtr found;
        if (!xref->TryFind(obj_number, found)) {
            continue;
        }

        if (found->GetGenerationNumber() != gen_number) {
            continue;
        }

        assert(found->GetObjectNumber() == obj_number && found->GetGenerationNumber() == gen_number);
        return true;
    }

    return false;
}

bool XrefChain::ReleaseEntry(XrefUsedEntryBasePtr entry) {

    for (auto& xref : _list) {
        auto object_number = entry->GetObjectNumber();

        OutputXrefEntryBasePtr found;
        if (!xref->TryFind(object_number, found)) {
            continue;
        }

        if (found->GetGenerationNumber() != entry->GetGenerationNumber()) {
            continue;
        }

        bool removed = xref->Remove(object_number);
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
