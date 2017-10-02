#ifndef _XREF_H
#define _XREF_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/files/xref_entry.h"

#include <unordered_set>

namespace vanillapdf {
namespace syntax {

class XrefBase : public virtual IUnknown, public IModifyObserver, public IModifyObservable {
public:
	using map_type = std::unordered_set<XrefEntryBasePtr>;

	typedef map_type::value_type value_type;
	typedef map_type::iterator iterator;
	typedef map_type::const_iterator const_iterator;
	typedef map_type::size_type size_type;
	typedef map_type::reference reference;
	typedef map_type::const_reference const_reference;
	typedef map_type::difference_type difference_type;

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
	};

	using IteratorPtr = DeferredIterator<Iterator>;

public:
	enum class Type {
		Table = 0,
		Stream
	};

	virtual void ObserveeChanged(IModifyObservable*) override;
	virtual void OnChanged() override;

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

	IteratorPtr Begin(void) const noexcept { return make_deferred<Iterator>(_entries.begin()); }
	IteratorPtr End(void) const noexcept { return make_deferred<Iterator>(_entries.end()); }

	virtual void Add(XrefEntryBasePtr entry);
	bool Remove(XrefEntryBasePtr entry);
	types::size_type Size(void) const noexcept;
	XrefEntryBasePtr Find(types::big_uint obj_number) const;
	bool Contains(types::big_uint obj_number) const;
	std::vector<XrefEntryBasePtr> Entries(void) const;

	virtual Type GetType(void) const noexcept = 0;
	virtual ~XrefBase();

	// stl compatibility
	iterator begin();
	const_iterator begin() const;

	iterator end();
	const_iterator end() const;

	iterator erase(const_iterator pos);

protected:
	WeakReference<File> _file;
	map_type _entries;
	types::stream_offset _last_xref_offset = constant::BAD_OFFSET;
	types::stream_offset _offset = constant::BAD_OFFSET;
	DictionaryObjectPtr _trailer_dictionary;
	bool m_dirty = false;
};

class XrefTable : public XrefBase {
public:
	virtual Type GetType(void) const noexcept override {
		return XrefBase::Type::Table;
	}

	virtual void Add(XrefEntryBasePtr entry) override;
};

class XrefStream : public XrefBase {
public:
	virtual Type GetType(void) const noexcept override {
		return XrefBase::Type::Stream;
	}

	StreamObjectPtr GetStreamObject(void) const {
		return _stream;
	}

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

} // syntax
} // vanillapdf

#endif /* _XREF_H */
