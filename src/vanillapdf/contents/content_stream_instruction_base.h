#ifndef _CONTENT_STREAM_INSTRUCTION_BASE_H
#define _CONTENT_STREAM_INSTRUCTION_BASE_H

#include "contents/content_fwd.h"

#include "utils/unknown_interface.h"
#include "utils/modify_observer_interface.h"

#include <list>

namespace vanillapdf {
namespace contents {

class InstructionBase : public virtual IUnknown, public IWeakReferenceable<InstructionBase>, public IModifyObservable {
public:
	enum class Type {
		Undefined = 0,
		Object,
		Operation
	};

	virtual Type GetInstructionType(void) const noexcept = 0;
	virtual std::string ToPdf() const = 0;
};

class BaseInstructionCollection : public virtual IUnknown, public IModifyObserver, public IModifyObservable {
public:
	using data_type = std::list<InstructionBasePtr>;

public:
	using value_type = data_type::value_type;
	using size_type = data_type::size_type;
	using iterator = data_type::iterator;
	using const_iterator = data_type::const_iterator;
	using reference = data_type::reference;
	using const_reference = data_type::const_reference;
	using difference_type = data_type::difference_type;

public:
	class Iterator : public BaseIterator<data_type::const_iterator>, public IWeakReferenceable<Iterator> {
	public:
		using BaseIterator<data_type::const_iterator>::BaseIterator;

		const Iterator& operator++() {
			++BaseIterator<data_type::const_iterator>::m_current;
			return *this;
		}

		const Iterator operator++(int) {
			Iterator temp(BaseIterator<data_type::const_iterator>::m_current, BaseIterator<data_type::const_iterator>::m_invalid);
			++BaseIterator<data_type::const_iterator>::m_current;
			return temp;
		}
	};

	using IteratorPtr = DeferredIterator<Iterator>;

public:
	BaseInstructionCollection() = default;
	BaseInstructionCollection(const BaseInstructionCollection&) = default;
	BaseInstructionCollection& operator=(const BaseInstructionCollection&) = default;

	BaseInstructionCollection(BaseInstructionCollection&&) = default;
	BaseInstructionCollection& operator=(BaseInstructionCollection&&) = default;

public:
	virtual void ObserveeChanged(const IModifyObservable*) override {
		OnChanged();
	}

public:
	// stl compatibility
	bool empty(void) const noexcept { return m_data.empty(); }
	size_type size(void) const noexcept { return m_data.size(); }
	iterator begin(void) noexcept { return m_data.begin(); }
	const_iterator begin(void) const noexcept { return m_data.begin(); }
	iterator end(void) noexcept { return m_data.end(); }
	const_iterator end(void) const noexcept { return m_data.end(); }
	reference front(void) { return m_data.front(); }
	const_reference front(void) const { return m_data.front(); }
	reference back(void) { return m_data.back(); }
	const_reference back(void) const { return m_data.back(); }

	IteratorPtr Begin() const { return make_deferred_iterator<Iterator>(m_data.begin(), m_data.end()); }
	IteratorPtr End(void) const { return make_deferred_iterator<Iterator>(m_data.end(), m_data.end()); }

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last) {

		m_data.assign(first, last);
		for (; first != last; ++first) {
			(*first)->Subscribe(this);
		}

		OnChanged();
	}

	template <class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last) {

		m_data.insert(position, first, last);
		for (; first != last; ++first) {
			(*first)->Subscribe(this);
		}

		OnChanged();
	}

	void push_back(InstructionBasePtr val) {
		m_data.push_back(val);
		val->Subscribe(this);
		OnChanged();
	}

	~BaseInstructionCollection() {
		for (auto item : m_data)
			item->Unsubscribe(this);
	}

private:
	data_type m_data;
};

} // contents
} // vanillapdf

#endif /* _CONTENT_STREAM_INSTRUCTION_BASE_H */
