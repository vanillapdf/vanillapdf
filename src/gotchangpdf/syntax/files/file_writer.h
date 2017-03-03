#ifndef _FILE_WRITER_H
#define _FILE_WRITER_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/files/file.h"

#include <memory>

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
	void Write(FilePtr source, FilePtr destination);
	void WriteIncremental(FilePtr source, FilePtr destination);

	bool GetRecalculateOffsetFlag(void) const noexcept { return m_recalculate_offset; }
	void SetRecalculateOffsetFlag(bool flag) noexcept { m_recalculate_offset = flag; }

	bool GetRecalculateStreamSizeFlag(void) const noexcept { return m_recalculate_stream_size; }
	void SetRecalculateStreamSizeFlag(bool flag) noexcept { m_recalculate_stream_size = flag; }

	bool GetRecalculateXrefSizeFlag(void) const noexcept { return m_recalculate_xref_size; }
	void SetRecalculateXrefSizeFlag(bool flag) noexcept { m_recalculate_xref_size = flag; }

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

	// flags
	bool m_recalculate_offset = true;
	bool m_recalculate_stream_size = true;
	bool m_recalculate_xref_size = true;
};

} // syntax
} // gotchangpdf

#endif /* _FILE_WRITER_H */
