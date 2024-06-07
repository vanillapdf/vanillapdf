#ifndef _XREF_H
#define _XREF_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/files/xref_entry.h"

#include <unordered_set>

namespace vanillapdf {
namespace syntax {

class XrefBase : public IModifyObserver, public IModifyObservable {
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
	class Iterator : public BaseIterator<map_type::const_iterator> {
	public:
		using BaseIterator<map_type::const_iterator>::BaseIterator;

		const Iterator& operator++() {
			++BaseIterator<map_type::const_iterator>::m_current;
			return *this;
		}

		const Iterator operator++(int) {
			Iterator temp(BaseIterator<map_type::const_iterator>::m_current, BaseIterator<map_type::const_iterator>::m_invalid);
			++BaseIterator<map_type::const_iterator>::m_current;
			return temp;
		}
	};

	using IteratorPtr = DeferredIterator<Iterator>;

public:
	enum class Type {
		Undefined = 0,
		Table,
		Stream,
		Virtual
	};

	virtual void ObserveeChanged(const IModifyObservable*) override;
	virtual void OnChanged() override;

	virtual void SetFile(WeakReference<File>file) noexcept = 0;
	virtual WeakReference<File> GetFile() const noexcept = 0;

	virtual DictionaryObjectPtr GetTrailerDictionary(void) const = 0;
	virtual void SetTrailerDictionary(DictionaryObjectPtr dictionary) = 0;

	virtual types::stream_offset GetOffset() const = 0;
	virtual void SetOffset(types::stream_offset offset) = 0;

	types::stream_offset GetLastXrefOffset() const noexcept { return _last_xref_offset; }
	void SetLastXrefOffset(types::stream_offset offset) noexcept { _last_xref_offset = offset; }

	virtual bool IsDirty(void) const noexcept = 0;
	virtual void SetDirty(bool dirty = true) noexcept = 0;

	IteratorPtr Begin(void) const { return make_deferred_iterator<Iterator>(_entries.begin(), _entries.end()); }
	IteratorPtr End(void) const { return make_deferred_iterator<Iterator>(_entries.end(), _entries.end()); }

	virtual void Add(XrefEntryBasePtr entry);
	bool Remove(XrefEntryBasePtr entry);
	types::size_type GetSize(void) const noexcept;
	virtual XrefEntryBasePtr Find(types::big_uint obj_number) const;
	virtual bool Contains(types::big_uint obj_number) const;
	std::vector<XrefEntryBasePtr> Entries(void) const;

	template <typename T>
	T FindAs(types::big_uint obj_number) const {
		auto found = Find(obj_number);
		return ConvertUtils<XrefEntryBasePtr>::ConvertTo<T>(found);
	}

	virtual Type GetType(void) const noexcept = 0;
	virtual ~XrefBase();

	// stl compatibility
	iterator begin();
	const_iterator begin() const;

	iterator end();
	const_iterator end() const;

	iterator erase(const_iterator pos);

protected:
	map_type _entries;
	types::stream_offset _last_xref_offset = constant::BAD_OFFSET;
};

class XrefStream : public XrefBase {
public:
	virtual Type GetType(void) const noexcept override {
		return XrefBase::Type::Stream;
	}

	virtual void SetFile(WeakReference<File> file) noexcept override;
	virtual WeakReference<File> GetFile() const noexcept override;

	virtual types::stream_offset GetOffset() const override;
	virtual void SetOffset(types::stream_offset offset) override;

	virtual bool IsDirty(void) const noexcept override;
	virtual void SetDirty(bool dirty = true) noexcept override;

	virtual DictionaryObjectPtr GetTrailerDictionary(void) const override;
	virtual void SetTrailerDictionary(DictionaryObjectPtr dictionary) override;

	StreamObjectPtr GetStreamObject(void) const;
	void SetStreamObject(StreamObjectPtr stream);

	void RecalculateContent();

	~XrefStream();

private:
	StreamObjectPtr _stream;

	void WriteValue(std::ostream& dest, types::big_uint value, int64_t width);
};

class XrefTable : public XrefBase {
public:
	virtual Type GetType(void) const noexcept override {
		return XrefBase::Type::Table;
	}

	virtual void SetFile(WeakReference<File> file) noexcept override { _file = file; }
	virtual WeakReference<File> GetFile() const noexcept override { return _file; }

	virtual types::stream_offset GetOffset() const override { return _offset; }
	virtual void SetOffset(types::stream_offset offset) override { _offset = offset; }

	virtual bool IsDirty(void) const noexcept override { return m_dirty; }
	virtual void SetDirty(bool dirty = true) noexcept override { m_dirty = dirty; }

	virtual void Add(XrefEntryBasePtr entry) override;
	virtual XrefEntryBasePtr Find(types::big_uint obj_number) const override;
	virtual bool Contains(types::big_uint obj_number) const override;

	virtual DictionaryObjectPtr GetTrailerDictionary(void) const override;
	virtual void SetTrailerDictionary(DictionaryObjectPtr dictionary) override;

	bool HasHybridStream(void) const;
	XrefStreamPtr GetHybridStream(void) const;
	void SetHybridStream(XrefStreamPtr stream);

private:
	WeakReference<File> _file;
	XrefStreamPtr m_xref_stm;
	DictionaryObjectPtr m_trailer_dictionary;

	types::stream_offset _offset = constant::BAD_OFFSET;

	// Same as Object::m_dirty
	bool m_dirty = false;
};

class XrefVirtualTable : public XrefBase {
public:
	virtual Type GetType(void) const noexcept override {
		return XrefBase::Type::Virtual;
	}

	virtual void SetFile(WeakReference<File> file) noexcept override { _file = file; }
	virtual WeakReference<File> GetFile() const noexcept override { return _file; }

	virtual types::stream_offset GetOffset() const override { return _offset; }
	virtual void SetOffset(types::stream_offset offset) override { _offset = offset; }

	virtual bool IsDirty(void) const noexcept override { return m_dirty; }
	virtual void SetDirty(bool dirty = true) noexcept override { m_dirty = dirty; }

	virtual DictionaryObjectPtr GetTrailerDictionary(void) const override { return m_trailer_dictionary; }
	virtual void SetTrailerDictionary(DictionaryObjectPtr dictionary) override { m_trailer_dictionary = dictionary; }

private:
	WeakReference<File> _file;
	DictionaryObjectPtr m_trailer_dictionary;

	types::stream_offset _offset = constant::BAD_OFFSET;

	// Same as Object::m_dirty
	bool m_dirty = false;
};

} // syntax
} // vanillapdf

#endif /* _XREF_H */
