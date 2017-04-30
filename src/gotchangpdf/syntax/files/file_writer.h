#ifndef _FILE_WRITER_H
#define _FILE_WRITER_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/files/file.h"

#include <memory>
#include <unordered_map>

namespace gotchangpdf {
namespace syntax {

class IFileWriterObserver : public virtual IUnknown, public IWeakReferenceable<IFileWriterObserver> {
public:
	virtual void OnInitializing(IInputStreamPtr input, IOutputStreamPtr output) {}
	virtual void OnFinalizing() {}

	virtual void OnBeforeObjectWrite(ObjectPtr ptr) {}
	virtual void OnAfterObjectWrite(ObjectPtr ptr) {}

	virtual void OnBeforeObjectOffsetRecalculation(ObjectPtr ptr) {}
	virtual void OnAfterObjectOffsetRecalculation(ObjectPtr ptr) {}

	virtual void OnBeforeEntryOffsetRecalculation(XrefUsedEntryBasePtr ptr) {}
	virtual void OnAfterEntryOffsetRecalculation(XrefUsedEntryBasePtr ptr) {}

	virtual void OnBeforeOutputFlush(IOutputStreamPtr output) {}
	virtual void OnAfterOutputFlush(IOutputStreamPtr output) {}

	virtual ~IFileWriterObserver() = 0;
};

class IFileWriterObservable : public virtual IUnknown, public IObservable<IFileWriterObserver> {
public:
	virtual void Initializing(IInputStreamPtr input, IOutputStreamPtr output) {
		for (auto current = m_observers.begin(); current != m_observers.end(); ++current) {
			Invoke(*current, &IFileWriterObserver::OnInitializing, input, output);
		}
	}

	virtual void Finalizing() {
		for (auto current = m_observers.begin(); current != m_observers.end(); ++current) {
			Invoke(*current, &IFileWriterObserver::OnFinalizing);
		}
	}

	virtual void BeforeOutputFlush(IOutputStreamPtr output) {
		for (auto current = m_observers.begin(); current != m_observers.end(); ++current) {
			Invoke(*current, &IFileWriterObserver::OnBeforeOutputFlush, output);
		}
	}

	virtual void AfterOutputFlush(IOutputStreamPtr output) {
		for (auto current = m_observers.begin(); current != m_observers.end(); ++current) {
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

class FileWriter : public virtual IUnknown {
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
	void WriteXrefObjects(FilePtr destination, XrefBasePtr source);
	XrefBasePtr CloneXref(FilePtr destination, XrefBasePtr source);
	XrefBasePtr CreateIncrementalXref(FilePtr source, FilePtr destination);
	void WriteXref(IOutputStreamPtr output, XrefBasePtr xref);
	void WriteXrefTable(IOutputStreamPtr output, XrefTablePtr xref_table);
	void WriteXrefOffset(IOutputStreamPtr output, types::stream_offset offset);
	void WriteEntry(IOutputStreamPtr output, XrefUsedEntryBasePtr entry);
	void WriteObject(IOutputStreamPtr output, ObjectPtr obj);

	static void CopyStreamContent(IInputStreamPtr source, IOutputStreamPtr destination);
	static std::string GetFormattedOffset(types::stream_offset offset);
	static std::string GetFormattedGenerationNumber(types::ushort generation_number);
	static std::string GetFormattedObjectNumber(types::big_uint generation_number);

	void RecalculateStreamLength(ObjectPtr obj);
	void RecalculateStreamsLength(XrefBasePtr source);
	void RecalculateObjectStreamContent(XrefBasePtr source);

	void CompressAndOptimize(XrefChainPtr xref);
	void RemoveFreedObjects(XrefChainPtr xref);
	void MergeXrefs(XrefChainPtr xref);
	void RemoveUnreferencedObjects(XrefChainPtr xref);
	void ExtractDuplicitDirectObjects(XrefChainPtr xref);
	void RemoveAllDuplicitIndirectObjects(XrefChainPtr xref);
	bool RemoveDuplicitIndirectObjects(XrefChainPtr xref);
	void CompressObjects(XrefChainPtr xref);
	void SquashTableSpace(XrefChainPtr xref);
	void CompressXref(XrefChainPtr xref);

	void InitializeReferences(ObjectPtr source);
	void RedirectReferences(ObjectPtr source, const std::unordered_map<ObjectPtr, ObjectPtr>& duplicit_items);

	// flags
	bool m_recalculate_offset = true;
	bool m_recalculate_stream_size = true;
	bool m_recalculate_xref_size = true;
	bool m_recalculate_object_stream_content = true;

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
} // gotchangpdf

#endif /* _FILE_WRITER_H */
