#ifndef _XREF_H
#define _XREF_H

#include "syntax/utils/syntax_fwd.h"
#include "utils/unknown_interface.h"
#include "utils/constants.h"

#include <vector>
#include <unordered_map>

namespace gotchangpdf {
namespace syntax {

class XrefEntryBase : public virtual IUnknown, public IWeakReferenceable<XrefEntryBase>, public IModifyObservable {
public:
	enum class Usage {
		Null = 0,
		Used,
		Free,
		Compressed
	};

public:
	XrefEntryBase(types::big_uint obj_number, types::ushort gen_number);

public:
	types::big_uint GetObjectNumber(void) const noexcept { return _obj_number; }
	void SetObjectNumber(types::big_uint value) noexcept { _obj_number = value; OnChanged(); }

	types::ushort GetGenerationNumber(void) const noexcept { return _gen_number; }
	void SetGenerationNumber(types::ushort value) noexcept { _gen_number = value; OnChanged(); }

	virtual Usage GetUsage(void) const noexcept = 0;

	virtual bool InUse(void) const noexcept = 0;

	void SetFile(WeakReference<File> file) noexcept { _file = file; }
	WeakReference<File> GetFile() const noexcept { return _file; }

	bool IsDirty(void) const noexcept { return _dirty; }
	void SetDirty(bool dirty = true) noexcept { _dirty = dirty; }

	bool operator<(const XrefEntryBase& other) const;

protected:
	WeakReference<File> _file;
	types::big_uint _obj_number = 0;
	types::ushort _gen_number = 0;
	bool _dirty = false;

private:
	// Private only for NullEntry
	XrefEntryBase() = default;
	friend class XrefNullEntry;
};

class XrefNullEntry : public XrefEntryBase {
public:
	virtual Usage GetUsage(void) const noexcept override {
		return XrefEntryBase::Usage::Null;
	}

	virtual bool InUse(void) const noexcept {
		return false;
	}
};

class XrefFreeEntry : public XrefEntryBase {
public:
	XrefFreeEntry(types::big_uint obj_number, types::ushort gen_number);

public:
	virtual Usage GetUsage(void) const noexcept override { return XrefEntryBase::Usage::Free; }

	types::big_uint GetNextFreeObjectNumber(void) const noexcept { return _obj_number; }
	void SetNextFreeObjectNumber(types::big_uint value) noexcept { _obj_number = value; OnChanged(); }

	virtual bool InUse(void) const noexcept {
		return false;
	}
};

class XrefUsedEntryBase : public XrefEntryBase, public IWeakReferenceable<XrefUsedEntryBase>, public IModifyObserver {
public:
	using XrefEntryBase::XrefEntryBase;

public:
	virtual ObjectPtr GetReference(void);
	virtual void SetReference(ObjectPtr ref);
	virtual void ReleaseReference(bool check_object_xref);

	virtual void ObserveeChanged(IModifyObservable*) override;
	virtual bool InUse(void) const noexcept override;

	~XrefUsedEntryBase();

protected:
	// TODO rework used flag as std::optional
	bool m_used = false;
	ObjectPtr _reference;

	virtual void Initialize(void) = 0;
};

class XrefUsedEntry : public XrefUsedEntryBase {
public:
	XrefUsedEntry(types::big_uint obj_number, types::ushort gen_number, types::stream_offset offset);

public:
	virtual Usage GetUsage(void) const noexcept override { return XrefEntryBase::Usage::Used; }

	types::stream_offset GetOffset(void) const noexcept { return _offset; }
	void SetOffset(types::stream_offset value) noexcept { _offset = value; OnChanged(); }

private:
	virtual void Initialize(void) override;
	types::stream_offset _offset = constant::BAD_OFFSET;
};

class XrefCompressedEntry : public XrefUsedEntryBase {
public:
	XrefCompressedEntry(types::big_uint obj_number, types::ushort gen_number, types::big_uint object_stream_number, types::size_type index);

public:
	virtual Usage GetUsage(void) const noexcept override { return XrefEntryBase::Usage::Compressed; }

	types::big_uint GetObjectStreamNumber(void) const noexcept { return _object_stream_number; }
	void SetObjectStreamNumber(types::big_uint value) noexcept { _object_stream_number = value; OnChanged(); }

	types::size_type GetIndex(void) const noexcept { return _index; }
	void SetIndex(types::size_type value) noexcept { _index = value; OnChanged(); }

private:
	virtual void Initialize(void) override;

	types::big_uint _object_stream_number = 0;
	types::size_type _index = 0;
};

class XrefBase : public virtual IUnknown, public IModifyObserver, public IModifyObservable {
public:
	using map_type = std::unordered_map<types::big_uint, XrefEntryBasePtr>;

public:
	class Iterator : public BaseIterator<map_type::const_iterator>, public IWeakReferenceable {
	public:
		using BaseIterator<map_type::const_iterator>::BaseIterator;

		const Iterator& operator++() {
			++BaseIterator<map_type::const_iterator>::m_it;
			return *this;
		}

		const Iterator operator++(int) {
			Iterator temp(BaseIterator<map_type::const_iterator>::m_it);
			++BaseIterator<map_type::const_iterator>::m_it;
			return temp;
		}

		map_type::key_type Key() const {
			return BaseIterator<map_type::const_iterator>::m_it->first;
		}

		map_type::mapped_type Mapped() const {
			return BaseIterator<map_type::const_iterator>::m_it->second;
		}
	};

	using IteratorPtr = DeferredIterator<Iterator>;

public:
	enum class Type {
		Table = 0,
		Stream
	};

	bool IsInitialized(void) const noexcept { return m_initialized; }
	void SetInitialized(bool initialized = true) noexcept { m_initialized = initialized; }

	virtual void ObserveeChanged(IModifyObservable*) override;

	void SetFile(WeakReference<File>file) noexcept { _file = file; }
	WeakReference<File> GetFile() const noexcept { return _file; }

	DictionaryObjectPtr GetTrailerDictionary(void) const { return _trailer_dictionary; }
	void SetTrailerDictionary(DictionaryObjectPtr dictionary) { _trailer_dictionary = dictionary; }

	types::stream_offset GetOffset() const noexcept { return _offset; }
	void SetOffset(types::stream_offset offset) noexcept { _offset = offset; }

	types::stream_offset GetLastXrefOffset() const noexcept { return _last_xref_offset; }
	void SetLastXrefOffset(types::stream_offset offset) noexcept { _last_xref_offset = offset; }

	bool IsDirty(void) const noexcept { return m_dirty; }
	void SetDirty(bool dirty = true) noexcept { m_dirty = dirty; }

	IteratorPtr Begin(void) const noexcept { return _entries.begin(); }
	IteratorPtr End(void) const noexcept { return _entries.end(); }

	void Add(XrefEntryBasePtr entry);
	bool Remove(types::big_uint obj_number);
	size_t Size(void) const noexcept;
	XrefEntryBasePtr Find(types::big_uint obj_number) const;
	bool Contains(types::big_uint obj_number) const;
	std::vector<XrefEntryBasePtr> Entries(void) const;

	virtual Type GetType(void) const noexcept = 0;
	virtual ~XrefBase();

protected:
	WeakReference<File> _file;
	map_type _entries;
	types::stream_offset _last_xref_offset = constant::BAD_OFFSET;
	types::stream_offset _offset = constant::BAD_OFFSET;
	DictionaryObjectPtr _trailer_dictionary;
	bool m_initialized = false;
	bool m_dirty = false;
};

class XrefTable : public XrefBase {
public:
	virtual Type GetType(void) const noexcept override { return XrefBase::Type::Table; }
};

class XrefStream : public XrefBase {
public:
	virtual Type GetType(void) const noexcept override { return XrefBase::Type::Stream; }

	StreamObjectPtr GetStreamObject(void) const { return _stream; }
	void SetStreamObject(StreamObjectPtr stream) {
		_stream->Unsubscribe(this);
		_stream = stream;
		_stream->Subscribe(this);

		auto stream_header = stream->GetHeader();
		SetTrailerDictionary(stream_header);
	}

	void RecalculateContent();

	~XrefStream();

private:
	StreamObjectPtr _stream;

	void WriteValue(std::ostream& dest, types::big_uint value, int64_t width);
};

inline bool operator<(const Deferred<syntax::XrefEntryBase>& left, const Deferred<syntax::XrefEntryBase>& right) {
	return *left < *right;
}

} // syntax
} // gotchangpdf

#endif /* _XREF_H */
