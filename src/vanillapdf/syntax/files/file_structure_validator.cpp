#include "precompiled.h"

#include "syntax/files/file_structure_validator.h"
#include "syntax/files/file.h"
#include "syntax/files/xref_chain.h"
#include "syntax/files/xref.h"
#include "syntax/files/xref_entry.h"
#include "syntax/files/header.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/integer_object.h"
#include "syntax/objects/indirect_reference_object.h"
#include "syntax/utils/name_constants.h"
#include "syntax/utils/object_utils.h"
#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/streams/input_stream_interface.h"
#include "utils/streams/seek_direction.h"

namespace vanillapdf {
namespace syntax {

FileStructureValidationResultPtr FileStructureValidator::Validate(FilePtr file) {
    if (!file->IsInitialized()) {
        throw FileNotInitializedException(file->GetFilenameString());
    }

    FileStructureValidationResultPtr result;

    // Determine file size for offset checks
    auto input_stream = file->GetInputStream();
    auto original_position = input_stream->GetInputPosition();
    input_stream->SetInputPosition(0, SeekDirection::End);
    auto file_size = input_stream->GetInputPosition();
    input_stream->SetInputPosition(original_position);

    ValidateHeader(file, result);
    ValidateXrefChain(file, result);
    ValidateTrailerDictionaries(file, result);
    ValidateXrefEntries(file, file_size, result);
    ValidateTrailerReferences(file, result);

    return result;
}

void FileStructureValidator::ValidateHeader(FilePtr file, FileStructureValidationResultPtr result) {
    auto header = file->GetHeader();
    if (header.empty()) {
        AddIssue(result, FileStructureIssueSeverity::Error, FileStructureIssueCode::HeaderMissing,
            "File header is missing");
        return;
    }

    if (header->GetVersion() == Version::Undefined) {
        AddIssue(result, FileStructureIssueSeverity::Error, FileStructureIssueCode::HeaderVersionUndefined,
            "File header version is undefined");
    }
}

void FileStructureValidator::ValidateXrefChain(FilePtr file, FileStructureValidationResultPtr result) {
    auto chain = file->GetXrefChain();
    if (chain.empty() || chain->Empty()) {
        AddIssue(result, FileStructureIssueSeverity::Error, FileStructureIssueCode::XrefChainEmpty,
            "Cross-reference chain is empty");
    }
}

void FileStructureValidator::ValidateTrailerDictionaries(FilePtr file, FileStructureValidationResultPtr result) {
    auto chain = file->GetXrefChain();
    if (chain.empty() || chain->Empty()) {
        return;
    }

    // Check the first (most recent) xref section's trailer
    auto xref_iterator = chain->Begin();
    if (!xref_iterator->IsValid()) {
        return;
    }

    auto xref = xref_iterator->Value();
    auto trailer = xref->GetTrailerDictionary();
    if (trailer.empty()) {
        AddIssue(result, FileStructureIssueSeverity::Error, FileStructureIssueCode::TrailerMissing,
            "First trailer dictionary is missing");
        return;
    }

    // Check /Size
    if (!trailer->Contains(constant::Name::Size)) {
        AddIssue(result, FileStructureIssueSeverity::Error, FileStructureIssueCode::TrailerMissingSize,
            "First trailer is missing required /Size entry");
    } else {
        // Check /Size value consistency
        OutputPointer<IntegerObjectPtr> size_obj;
        if (trailer->TryFindAs(constant::Name::Size, size_obj)) {
            auto declared_size = static_cast<types::big_uint>(size_obj->GetIntegerValue());

            // Find max object number across all xref sections
            types::big_uint max_object_number = 0;
            for (auto xref_iter = chain->Begin(); xref_iter->IsValid(); ++(*xref_iter)) {
                auto current_xref = xref_iter->Value();
                auto entries = current_xref->Entries();
                for (auto entry : entries) {
                    auto obj_num = entry->GetObjectNumber();
                    if (obj_num > max_object_number) {
                        max_object_number = obj_num;
                    }
                }
            }

            // /Size should be one more than the max object number
            auto expected_size = max_object_number + 1;
            if (declared_size != expected_size) {
                AddIssue(result, FileStructureIssueSeverity::Warning, FileStructureIssueCode::TrailerSizeMismatch,
                    "Trailer /Size value (" + std::to_string(declared_size)
                    + ") does not match expected value (" + std::to_string(expected_size) + ")");
            }
        }
    }

    // Check /Root
    if (!trailer->Contains(constant::Name::Root)) {
        AddIssue(result, FileStructureIssueSeverity::Error, FileStructureIssueCode::TrailerMissingRoot,
            "First trailer is missing required /Root entry");
    }
}

void FileStructureValidator::ValidateXrefEntries(FilePtr file, types::stream_size file_size, FileStructureValidationResultPtr result) {
    auto chain = file->GetXrefChain();
    if (chain.empty() || chain->Empty()) {
        return;
    }

    for (auto xref_iter = chain->Begin(); xref_iter->IsValid(); ++(*xref_iter)) {
        auto xref = xref_iter->Value();
        auto entries = xref->Entries();

        for (auto entry : entries) {
            auto usage = entry->GetUsage();
            auto obj_num = entry->GetObjectNumber();
            auto gen_num = entry->GetGenerationNumber();

            if (usage == XrefEntryBase::Usage::Used) {
                auto used_entry = dynamic_pointer_cast<XrefUsedEntry>(entry);
                if (!used_entry.empty()) {
                    auto offset = used_entry->GetOffset();

                    if (offset == constant::BAD_OFFSET || offset < 0) {
                        AddIssue(result, FileStructureIssueSeverity::Error,
                            FileStructureIssueCode::XrefUsedEntryBadOffset,
                            "Used xref entry has invalid offset (" + std::to_string(offset) + ")",
                            obj_num, gen_num);
                    } else if (offset > file_size) {
                        AddIssue(result, FileStructureIssueSeverity::Error,
                            FileStructureIssueCode::XrefUsedEntryOffsetExceedsFileSize,
                            "Used xref entry offset (" + std::to_string(offset)
                            + ") exceeds file size (" + std::to_string(file_size) + ")",
                            obj_num, gen_num);
                    }
                }
            } else if (usage == XrefEntryBase::Usage::Compressed) {
                auto compressed_entry = dynamic_pointer_cast<XrefCompressedEntry>(entry);
                if (!compressed_entry.empty()) {
                    auto stream_obj_num = compressed_entry->GetObjectStreamNumber();

                    // Check that the object stream exists in the xref chain
                    if (!chain->Contains(stream_obj_num, 0)) {
                        AddIssue(result, FileStructureIssueSeverity::Error,
                            FileStructureIssueCode::XrefCompressedEntryMissingObjectStream,
                            "Compressed xref entry references object stream " + std::to_string(stream_obj_num)
                            + " which does not exist in the xref chain",
                            obj_num, gen_num);
                    }
                }
            } else if (usage == XrefEntryBase::Usage::Free) {
                auto free_entry = dynamic_pointer_cast<XrefFreeEntry>(entry);
                if (!free_entry.empty()) {
                    auto next_free = free_entry->GetNextFreeObjectNumber();

                    // Next free object number should be 0 or reference a known object number
                    if (next_free != 0 && !chain->Contains(next_free, 0)) {
                        AddIssue(result, FileStructureIssueSeverity::Warning,
                            FileStructureIssueCode::XrefFreeEntryInvalidNextFree,
                            "Free xref entry references next free object " + std::to_string(next_free)
                            + " which does not exist in the xref chain",
                            obj_num, gen_num);
                    }
                }
            }
        }
    }
}

void FileStructureValidator::ValidateTrailerReferences(FilePtr file, FileStructureValidationResultPtr result) {
    auto chain = file->GetXrefChain();
    if (chain.empty() || chain->Empty()) {
        return;
    }

    auto xref_iterator = chain->Begin();
    if (!xref_iterator->IsValid()) {
        return;
    }

    auto xref = xref_iterator->Value();
    auto trailer = xref->GetTrailerDictionary();
    if (trailer.empty()) {
        return;
    }

    // Check /Root reference
    if (trailer->Contains(constant::Name::Root)) {
        auto root_obj = trailer->Find(constant::Name::Root);
        if (ObjectUtils::IsType<IndirectReferenceObjectPtr>(root_obj)) {
            auto root_ref = ObjectUtils::ConvertTo<IndirectReferenceObjectPtr>(root_obj);
            auto ref_obj_num = root_ref->GetReferencedObjectNumber();
            auto ref_gen_num = root_ref->GetReferencedGenerationNumber();

            if (!chain->Contains(ref_obj_num, ref_gen_num)) {
                AddIssue(result, FileStructureIssueSeverity::Error,
                    FileStructureIssueCode::TrailerRootReferenceNotInXref,
                    "Trailer /Root indirect reference (" + std::to_string(ref_obj_num) + " "
                    + std::to_string(ref_gen_num) + " R) target not found in xref chain",
                    ref_obj_num, ref_gen_num);
            }
        }
    }

    // Check /Info reference (optional)
    if (trailer->Contains(constant::Name::Info)) {
        auto info_obj = trailer->Find(constant::Name::Info);
        if (ObjectUtils::IsType<IndirectReferenceObjectPtr>(info_obj)) {
            auto info_ref = ObjectUtils::ConvertTo<IndirectReferenceObjectPtr>(info_obj);
            auto ref_obj_num = info_ref->GetReferencedObjectNumber();
            auto ref_gen_num = info_ref->GetReferencedGenerationNumber();

            if (!chain->Contains(ref_obj_num, ref_gen_num)) {
                AddIssue(result, FileStructureIssueSeverity::Warning,
                    FileStructureIssueCode::TrailerInfoReferenceNotInXref,
                    "Trailer /Info indirect reference (" + std::to_string(ref_obj_num) + " "
                    + std::to_string(ref_gen_num) + " R) target not found in xref chain",
                    ref_obj_num, ref_gen_num);
            }
        }
    }
}

void FileStructureValidator::AddIssue(
    FileStructureValidationResultPtr result,
    FileStructureIssueSeverity severity,
    FileStructureIssueCode code,
    const std::string& message) {

    auto buffer = Buffer::CreateFromString(message);
    auto issue = make_deferred<FileStructureIssue>(severity, code, buffer);
    result->AddIssue(issue);
}

void FileStructureValidator::AddIssue(
    FileStructureValidationResultPtr result,
    FileStructureIssueSeverity severity,
    FileStructureIssueCode code,
    const std::string& message,
    types::big_uint object_number,
    types::ushort generation_number) {

    auto buffer = Buffer::CreateFromString(message);
    auto issue = make_deferred<FileStructureIssue>(severity, code, buffer, object_number, generation_number);
    result->AddIssue(issue);
}

} // syntax
} // vanillapdf
