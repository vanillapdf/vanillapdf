#ifndef _FILE_WRITER_H
#define _FILE_WRITER_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/files/file.h"

#include <memory>
#include <unordered_map>

namespace vanillapdf {
namespace syntax {

class IFileWriterObserver : public IUnknown, public IWeakReferenceable<IFileWriterObserver> {
public:
    virtual void OnInitializing(IInputOutputStreamPtr output) {}
    virtual void OnFinalizing(IInputOutputStreamPtr output) {}

    virtual void OnBeforeObjectWrite(ObjectPtr ptr) {}
    virtual void OnAfterObjectWrite(ObjectPtr ptr) {}

    virtual void OnBeforeObjectOffsetRecalculation(ObjectPtr ptr) {}
    virtual void OnAfterObjectOffsetRecalculation(ObjectPtr ptr) {}

    virtual void OnBeforeEntryOffsetRecalculation(XrefUsedEntryBasePtr entry) {}
    virtual void OnAfterEntryOffsetRecalculation(XrefUsedEntryBasePtr entry) {}

    virtual void OnBeforeOutputFlush(IInputOutputStreamPtr output) {}
    virtual void OnAfterOutputFlush(IInputOutputStreamPtr output) {}

    virtual ~IFileWriterObserver() = 0;
};

class IFileWriterObservable : public IUnknown, public IObservable<IFileWriterObserver> {
public:
    virtual void Initializing(IInputOutputStreamPtr output) {
        for (auto current = GetObservers()->begin(); current != GetObservers()->end(); ++current) {
            Invoke(*current, &IFileWriterObserver::OnInitializing, output);
        }
    }

    virtual void Finalizing(IInputOutputStreamPtr output) {
        for (auto current = GetObservers()->begin(); current != GetObservers()->end(); ++current) {
            Invoke(*current, &IFileWriterObserver::OnFinalizing, output);
        }
    }

    virtual void BeforeObjectOffsetRecalculation(ObjectPtr obj) {
        for (auto current = GetObservers()->begin(); current != GetObservers()->end(); ++current) {
            Invoke(*current, &IFileWriterObserver::OnBeforeObjectOffsetRecalculation, obj);
        }
    }

    virtual void AfterObjectOffsetRecalculation(ObjectPtr obj) {
        for (auto current = GetObservers()->begin(); current != GetObservers()->end(); ++current) {
            Invoke(*current, &IFileWriterObserver::OnAfterObjectOffsetRecalculation, obj);
        }
    }

    virtual void BeforeEntryOffsetRecalculation(XrefUsedEntryBasePtr entry) {
        for (auto current = GetObservers()->begin(); current != GetObservers()->end(); ++current) {
            Invoke(*current, &IFileWriterObserver::OnBeforeEntryOffsetRecalculation, entry);
        }
    }

    virtual void AfterEntryOffsetRecalculation(XrefUsedEntryBasePtr entry) {
        for (auto current = GetObservers()->begin(); current != GetObservers()->end(); ++current) {
            Invoke(*current, &IFileWriterObserver::OnAfterEntryOffsetRecalculation, entry);
        }
    }

    virtual void BeforeObjectWrite(ObjectPtr obj) {
        for (auto current = GetObservers()->begin(); current != GetObservers()->end(); ++current) {
            Invoke(*current, &IFileWriterObserver::OnBeforeObjectWrite, obj);
        }
    }

    virtual void AfterObjectWrite(ObjectPtr obj) {
        for (auto current = GetObservers()->begin(); current != GetObservers()->end(); ++current) {
            Invoke(*current, &IFileWriterObserver::OnAfterObjectWrite, obj);
        }
    }

    virtual void BeforeOutputFlush(IInputOutputStreamPtr output) {
        for (auto current = GetObservers()->begin(); current != GetObservers()->end(); ++current) {
            Invoke(*current, &IFileWriterObserver::OnBeforeOutputFlush, output);
        }
    }

    virtual void AfterOutputFlush(IInputOutputStreamPtr output) {
        for (auto current = GetObservers()->begin(); current != GetObservers()->end(); ++current) {
            Invoke(*current, &IFileWriterObserver::OnAfterOutputFlush, output);
        }
    }

    virtual ~IFileWriterObservable() = 0;

private:
    template <typename... Args>
    void Invoke(WeakReference<IFileWriterObserver> weak_observer, void (IFileWriterObserver:: *method)(Args...), Args... args) {
        if (weak_observer.IsActive() && !weak_observer.IsEmpty()) {
            auto observer = weak_observer.GetReference();
            auto raw_observer = observer.get();

            // Call (method) on observer with args
            (raw_observer->*method)(args...);
        }
    }
};

class FileWriter : public IFileWriterObservable {
public:
    bool ValidateConfiguration(FilePtr source, std::string& reason) const;

    void Write(FilePtr source, FilePtr destination);
    void WriteIncremental(FilePtr source, FilePtr destination);

    bool GetRecalculateOffsetFlag(void) const noexcept { return m_recalculate_offset; }
    void SetRecalculateOffsetFlag(bool flag) noexcept { m_recalculate_offset = flag; }

    bool GetRecalculateStreamSizeFlag(void) const noexcept { return m_recalculate_stream_size; }
    void SetRecalculateStreamSizeFlag(bool flag) noexcept { m_recalculate_stream_size = flag; }

    bool GetRecalculateXrefSizeFlag(void) const noexcept { return m_recalculate_xref_size; }
    void SetRecalculateXrefSizeFlag(bool flag) noexcept { m_recalculate_xref_size = flag; }

    bool GetRecalculateObjectStreamContentFlag(void) const noexcept { return m_recalculate_object_stream_content; }
    void SetRecalculateObjectStreamContentFlag(bool flag) noexcept { m_recalculate_object_stream_content = flag; }

    bool GetRecalculateXrefStreamWidth(void) const noexcept { return m_recalculate_xref_stream_width; }
    void SetRecalculateXrefStreamWidth(bool flag) noexcept { m_recalculate_xref_stream_width = flag; }

    bool GetMergeXrefsFlag(void) const noexcept { return m_merge_xrefs; }
    void SetMergeXrefsFlag(bool flag) noexcept { m_merge_xrefs = flag; }

    bool GetRemoveDuplicitIndirectObjectsFlag(void) const noexcept { return m_remove_duplicit_indirect_objects; }
    void SetRemoveDuplicitIndirectObjectsFlag(bool flag) noexcept { m_remove_duplicit_indirect_objects = flag; }

    bool GetExtractDuplicitDirectObjectsFlag(void) const noexcept { return m_extract_duplicit_direct_objects; }
    void SetExtractDuplicitDirectObjectsFlag(bool flag) noexcept { m_extract_duplicit_direct_objects = flag; }

    bool GetCompressObjectsFlag(void) const noexcept { return m_compress_objects; }
    void SetCompressObjectsFlag(bool flag) noexcept { m_compress_objects = flag; }

    bool GetCompressXrefFlag(void) const noexcept { return m_compress_xref; }
    void SetCompressXrefFlag(bool flag) noexcept { m_compress_xref = flag; }

    bool GetRemoveUnreferencedObjectsFlag(void) const noexcept { return m_remove_unreferenced; }
    void SetRemoveUnreferencedObjectsFlag(bool flag) noexcept { m_remove_unreferenced = flag; }

    bool GetSquashTableSpaceFlag(void) const noexcept { return m_squash_table_space; }
    void SetSquashTableSpaceFlag(bool flag) noexcept { m_squash_table_space = flag; }

    bool GetRemoveFreedObjectsFlag(void) const noexcept { return m_remove_freed; }
    void SetRemoveFreedObjectsFlag(bool flag) noexcept { m_remove_freed = flag; }

private:
    HeaderPtr CloneHeader(FilePtr source, FilePtr destination);
    XrefChainPtr CloneXrefChain(FilePtr source, FilePtr destination);
    XrefBasePtr CloneXref(FilePtr destination, XrefBasePtr source);
    DictionaryObjectPtr CloneTrailerDictionary(FilePtr source, XrefBasePtr xref);
    XrefBasePtr CreateIncrementalXref(FilePtr source, FilePtr destination);

    void WriteHeader(IOutputStreamPtr output, HeaderPtr header);
    void WriteXrefObjects(IOutputStreamPtr output, XrefBasePtr source);
    void WriteXrefChain(IOutputStreamPtr output, XrefChainPtr chain);
    void WriteXref(IOutputStreamPtr output, XrefBasePtr xref);
    void WriteXrefTable(IOutputStreamPtr output, XrefTablePtr xref_table);
    void WriteXrefOffset(IOutputStreamPtr output, types::stream_offset offset);
    void WriteEntry(IOutputStreamPtr output, XrefUsedEntryBasePtr entry);
    void WriteObject(IOutputStreamPtr output, ObjectPtr obj);

    static void CopyStreamContent(IInputStreamPtr source, IOutputStreamPtr destination);
    static std::string GetFormattedOffset(types::stream_offset offset);
    static std::string GetFormattedGenerationNumber(types::ushort generation_number);
    static std::string GetFormattedObjectNumber(types::big_uint generation_number);

    void CloneHybridStreams(FilePtr source, FilePtr destination);
    void FixStreamReferences(XrefChainPtr source, XrefChainPtr destination);
    void SetEncryptionData(FilePtr source, FilePtr destination);

    void RecalculateXrefPrevOffset(XrefBasePtr source, XrefBasePtr prev);
    void RecalculateXrefHybridOffset(XrefTablePtr source);
    void RecalculateStreamLength(StreamObjectPtr obj);
    void RecalculateStreamsLength(XrefBasePtr source);
    void RecalculateObjectStreamContent(XrefChainPtr chain, XrefBasePtr source);
    void RecalculateXrefSize(XrefChainPtr chain, XrefBasePtr source);
    void RecalculateXrefStreamWidth(XrefStreamPtr source);

    void CompressAndOptimize(FilePtr destination);
    void RemoveFreedObjects(XrefChainPtr xref);
    void MergeXrefs(XrefChainPtr xref);
    void RemoveUnreferencedObjects(XrefChainPtr xref);
    void ExtractDuplicitDirectObjects(XrefChainPtr xref);
    void RemoveAllDuplicitIndirectObjects(XrefChainPtr xref);
    bool RemoveDuplicitIndirectObjects(XrefChainPtr xref);
    void CompressObjects(XrefChainPtr xref);
    void SquashTableSpace(XrefChainPtr xref);
    void CompressXref(XrefChainPtr xref);

    void ApplyWatermark(FilePtr destination);
    DictionaryObjectPtr AddWatermarkFont(FilePtr destination);
    void ApplyWatermarkPageNode(DictionaryObjectPtr obj, DictionaryObjectPtr watermark_font);
    void ApplyWatermarkPrependSave(StreamObjectPtr obj);
    void ApplyWatermarkContentStream(StreamObjectPtr obj, ArrayObjectPtr<IntegerObjectPtr> media_box, const std::string& watermark_font);

    XrefBasePtr FindPreviousXref(XrefChainPtr chain, XrefBasePtr source);

    void InitializeReferences(ObjectPtr source);
    void RedirectReferences(ObjectPtr source, const std::unordered_map<ObjectPtr, ObjectPtr>& duplicit_items);
    void FindIndirectReferences(ObjectPtr source, std::unordered_map<XrefEntryBasePtr, bool>& used_entries);

    // flags
    bool m_recalculate_offset = true;
    bool m_recalculate_stream_size = true;
    bool m_recalculate_xref_size = true;
    bool m_recalculate_object_stream_content = true;
    bool m_recalculate_xref_stream_width = true;

    // Compress and optimize flags
    bool m_merge_xrefs = true;
    bool m_remove_freed = true;
    bool m_remove_unreferenced = true;
    bool m_extract_duplicit_direct_objects = true;
    bool m_remove_duplicit_indirect_objects = true;
    bool m_compress_objects = true;
    bool m_squash_table_space = true;
    bool m_compress_xref = true;
};

} // syntax
} // vanillapdf

#endif /* _FILE_WRITER_H */
