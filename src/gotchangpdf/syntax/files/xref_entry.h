#ifndef _XREF_ENTRY_H
#define _XREF_ENTRY_H

#include "syntax/utils/syntax_fwd.h"
#include "utils/modify_observer_interface.h"
#include "utils/constants.h"

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
	types::big_uint GetObjectNumber(void) const noexcept {
		return _obj_number;
	}

	types::ushort GetGenerationNumber(void) const noexcept {
		return _gen_number;
	}

	void SetGenerationNumber(types::ushort value) {
		_gen_number = value;
		OnChanged();
	}

	virtual Usage GetUsage(void) const noexcept = 0;

	virtual bool InUse(void) const noexcept = 0;

	void SetFile(WeakReference<File> file) noexcept { _file = file; }
	WeakReference<File> GetFile() const noexcept { return _file; }

	bool IsDirty(void) const noexcept { return _dirty; }
	void SetDirty(bool dirty = true) noexcept { _dirty = dirty; }

	bool operator==(const XrefEntryBase& other) const;
	bool operator!=(const XrefEntryBase& other) const;
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
	XrefFreeEntry(types::big_uint obj_number, types::ushort gen_number, types::big_uint next_free_object);

	static XrefFreeEntryPtr Create(types::big_uint obj_number, types::ushort gen_number) {
		return make_deferred<XrefFreeEntry>(obj_number, gen_number);
	}

	static XrefFreeEntryPtr Create(types::big_uint obj_number, types::ushort gen_number, types::big_uint next_free_object) {
		return make_deferred<XrefFreeEntry>(obj_number, gen_number, next_free_object);
	}

public:
	virtual Usage GetUsage(void) const noexcept override {
		return XrefEntryBase::Usage::Free;
	}

	types::big_uint GetNextFreeObjectNumber(void) const noexcept {
		return m_next_free_object;
	}

	void SetNextFreeObjectNumber(types::big_uint value) {
		m_next_free_object = value;
		OnChanged();
	}

	virtual bool InUse(void) const noexcept {
		return false;
	}

private:
	types::big_uint m_next_free_object = 0;
};

class XrefUsedEntryBase : public XrefEntryBase, public IWeakReferenceable<XrefUsedEntryBase>, public IModifyObserver {
public:
	using XrefEntryBase::XrefEntryBase;

public:
	virtual ObjectPtr GetReference(void);
	virtual void SetReference(ObjectPtr ref);
	virtual void ReleaseReference(bool check_object_xref = true);

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
	XrefUsedEntry(types::big_uint obj_number, types::ushort gen_number);
	XrefUsedEntry(types::big_uint obj_number, types::ushort gen_number, types::stream_offset offset);

public:
	virtual Usage GetUsage(void) const noexcept override {
		return XrefEntryBase::Usage::Used;
	}

	types::stream_offset GetOffset(void) const noexcept {
		return _offset;
	}

	void SetOffset(types::stream_offset value) {
		_offset = value;
		OnChanged();
	}

private:
	virtual void Initialize(void) override;
	types::stream_offset _offset = constant::BAD_OFFSET;
};

class XrefCompressedEntry : public XrefUsedEntryBase {
public:
	XrefCompressedEntry(types::big_uint obj_number, types::ushort gen_number);
	XrefCompressedEntry(types::big_uint obj_number, types::ushort gen_number, types::big_uint object_stream_number, types::size_type index);

public:
	virtual Usage GetUsage(void) const noexcept override {
		return XrefEntryBase::Usage::Compressed;
	}

	types::big_uint GetObjectStreamNumber(void) const noexcept {
		return _object_stream_number;
	}

	void SetObjectStreamNumber(types::big_uint value) {
		_object_stream_number = value;
		OnChanged();
	}

	types::size_type GetIndex(void) const noexcept {
		return _index;
	}

	void SetIndex(types::size_type value) {
		_index = value;
		OnChanged();
	}

private:
	virtual void Initialize(void) override;

	types::big_uint _object_stream_number = 0;
	types::size_type _index = 0;
};

inline bool operator==(const Deferred<syntax::XrefEntryBase>& left, const Deferred<syntax::XrefEntryBase>& right) {
	return *left == *right;
}

inline bool operator!=(const Deferred<syntax::XrefEntryBase>& left, const Deferred<syntax::XrefEntryBase>& right) {
	return *left != *right;
}

inline bool operator<(const Deferred<syntax::XrefEntryBase>& left, const Deferred<syntax::XrefEntryBase>& right) {
	return *left < *right;
}

} // syntax
} // gotchangpdf

namespace std {

template <> struct hash<gotchangpdf::syntax::XrefEntryBasePtr> {
	size_t operator()(const gotchangpdf::syntax::XrefEntryBasePtr& entry) const;
};

} // std

#endif /* _XREF_ENTRY_H */
