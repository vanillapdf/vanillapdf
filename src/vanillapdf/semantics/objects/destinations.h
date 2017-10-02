#ifndef _DESTINATIONS_H
#define _DESTINATIONS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/objects/dictionary_object.h"

namespace vanillapdf {
namespace semantics {

class DestinationBase : public HighLevelObject<syntax::ObjectPtr> {
public:
	enum class Type {
		XYZ = 0,
		Fit,
		FitHorizontal,
		FitVertical,
		FitRectangle,
		FitBoundingBox,
		FitBoundingBoxHorizontal,
		FitBoundingBoxVertical
	};

public:
	explicit DestinationBase(syntax::MixedArrayObjectPtr root);
	explicit DestinationBase(syntax::DictionaryObjectPtr root);

	static std::unique_ptr<DestinationBase> Create(syntax::MixedArrayObjectPtr root);
	static std::unique_ptr<DestinationBase> Create(syntax::DictionaryObjectPtr root);
	static std::unique_ptr<DestinationBase> Create(syntax::ObjectPtr root);

	syntax::ObjectPtr GetPage() const;
	bool HasAttribute(const syntax::NameObject& name) const;
	syntax::ObjectPtr GetAttribute(const syntax::NameObject& name) const;

	virtual Type GetType() const noexcept = 0;
};

class XYZDestination : public DestinationBase {
public:
	explicit XYZDestination(syntax::MixedArrayObjectPtr root);
	explicit XYZDestination(syntax::DictionaryObjectPtr root);
	virtual Type GetType() const noexcept override;
};

class FitDestination : public DestinationBase {
public:
	explicit FitDestination(syntax::MixedArrayObjectPtr root);
	explicit FitDestination(syntax::DictionaryObjectPtr root);
	virtual Type GetType() const noexcept override;
};

class FitHorizontalDestination : public DestinationBase {
public:
	explicit FitHorizontalDestination(syntax::MixedArrayObjectPtr root);
	explicit FitHorizontalDestination(syntax::DictionaryObjectPtr root);
	virtual Type GetType() const noexcept override;
};

class FitVerticalDestination : public DestinationBase {
public:
	explicit FitVerticalDestination(syntax::MixedArrayObjectPtr root);
	explicit FitVerticalDestination(syntax::DictionaryObjectPtr root);
	virtual Type GetType() const noexcept override;
};

class FitRectangleDestination : public DestinationBase {
public:
	explicit FitRectangleDestination(syntax::MixedArrayObjectPtr root);
	explicit FitRectangleDestination(syntax::DictionaryObjectPtr root);
	virtual Type GetType() const noexcept override;
};

class FitBoundingBoxDestination : public DestinationBase {
public:
	explicit FitBoundingBoxDestination(syntax::MixedArrayObjectPtr root);
	explicit FitBoundingBoxDestination(syntax::DictionaryObjectPtr root);
	virtual Type GetType() const noexcept override;
};

class FitBoundingBoxHorizontalDestination : public DestinationBase {
public:
	explicit FitBoundingBoxHorizontalDestination(syntax::MixedArrayObjectPtr root);
	explicit FitBoundingBoxHorizontalDestination(syntax::DictionaryObjectPtr root);
	virtual Type GetType() const noexcept override;
};

class FitBoundingBoxVerticalDestination : public DestinationBase {
public:
	explicit FitBoundingBoxVerticalDestination(syntax::MixedArrayObjectPtr root);
	explicit FitBoundingBoxVerticalDestination(syntax::DictionaryObjectPtr root);
	virtual Type GetType() const noexcept override;
};

class NamedDestinations : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	class Iterator : public BaseIterator<syntax::DictionaryObjectPtr::const_iterator> {
	public:
		using BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::BaseIterator;

	public:
		const Iterator& operator++() {
			++BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::m_it;
			return *this;
		}

		const Iterator operator++(int) {
			Iterator temp(BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::m_it);
			++BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::m_it;
			return temp;
		}

		std::pair<syntax::NameObjectPtr, DestinationPtr> operator*() {
			return std::pair<syntax::NameObjectPtr, DestinationPtr>(First(), Second());
		}

		syntax::NameObjectPtr First() const {
			return BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::m_it->first;
		}

		DestinationPtr Second() const {
			auto containable = BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::m_it->second;
			auto new_destination = DestinationBase::Create(containable);
			return DestinationPtr(new_destination.release());
		}
	};

public:
	typedef typename std::pair<syntax::NameObjectPtr, DestinationPtr> value_type;
	typedef Iterator iterator;
	typedef Iterator const_iterator;
	typedef typename syntax::DictionaryObjectPtr::size_type size_type;
	typedef typename syntax::DictionaryObjectPtr::reference reference;
	typedef typename syntax::DictionaryObjectPtr::const_reference const_reference;
	typedef typename syntax::DictionaryObjectPtr::difference_type difference_type;

public:
	explicit NamedDestinations(syntax::DictionaryObjectPtr root);

	void Insert(const syntax::NameObject& name, DestinationPtr value);
	bool Contains(const syntax::NameObject& name) const;
	DestinationPtr Find(const syntax::NameObject& name) const;

	// stl compatibility
	iterator begin() {
		return Iterator(_obj->begin());
	}

	const_iterator begin() const {
		return Iterator(_obj->begin());
	}

	iterator end() {
		return Iterator(_obj->end());
	}

	const_iterator end() const {
		return Iterator(_obj->end());
	}
};

} // semantics
} // vanillapdf

#endif /* _NAME_DICTIONARY_H */
