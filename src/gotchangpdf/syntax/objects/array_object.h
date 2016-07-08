#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "syntax_fwd.h"
#include "object.h"
#include "containable.h"
#include "util.h"

#include <vector>
#include <algorithm>
#include <functional>

namespace gotchangpdf
{
	namespace syntax
	{
		class MixedArrayObject : public ContainableObject, public IModifyObserver
		{
		public:
			typedef std::vector<ContainableObjectPtr> list_type;
			typedef list_type::value_type value_type;
			typedef list_type::iterator iterator;
			typedef list_type::const_iterator const_iterator;
			typedef list_type::size_type size_type;
			typedef list_type::reference reference;
			typedef list_type::const_reference const_reference;

		public:
			MixedArrayObject() = default;
			explicit MixedArrayObject(const list_type& list);
			explicit MixedArrayObject(const std::initializer_list<ContainableObjectPtr>& list);
			MixedArrayObject(const ContainableObject& other, list_type& list);

			virtual void SetFile(std::weak_ptr<File> file) noexcept override;
			virtual void SetObjectNumber(types::big_uint number) noexcept override;
			virtual void SetGenerationNumber(types::ushort number) noexcept override;

			virtual void ObserveeChanged(IModifyObservable*) override;

			virtual MixedArrayObject* Clone(void) const override;

			virtual Object::Type GetType(void) const noexcept override { return Object::Type::Array; }
			size_t Size(void) const noexcept { return _list.size(); }
			const ContainableObjectPtr& operator[](size_t i) const { return _list[i]; }
			ContainableObjectPtr& operator[](size_t i) { return _list[i]; }
			const ContainableObjectPtr& At(size_t at) const { return _list.at(at); }
			ContainableObjectPtr& At(size_t at) { return _list.at(at); }

			void Append(const ContainableObjectPtr& value);
			void Insert(const ContainableObjectPtr& value, size_t at);
			void Remove(size_t at);

			// stl compatibility
			void push_back(const value_type& value);

			iterator begin() noexcept { return _list.begin(); }
			const_iterator begin() const noexcept { return _list.begin(); }
			iterator end() noexcept { return _list.end(); }
			const_iterator end() const noexcept { return _list.end(); }

			template <typename U>
			ArrayObjectPtr<U> Convert(std::function<U(const ContainableObjectPtr& obj)> f) const
			{
				return ArrayObjectPtr<U>(*this, f);
			}

			virtual std::string ToString(void) const override;
			virtual std::string ToPdf(void) const override;

			virtual ~MixedArrayObject();

		protected:
			list_type _list;
		};

		// This class shall act as a front-end above mixed array
		template <typename T>
		class ArrayObject : public IUnknown
		{
		public:
			typedef std::vector<T> list_type;
			typedef typename list_type::value_type value_type;
			typedef typename list_type::iterator iterator;
			typedef typename list_type::const_iterator const_iterator;
			typedef typename list_type::size_type size_type;
			typedef typename list_type::reference reference;
			typedef typename list_type::const_reference const_reference;

			friend class ArrayObject;

		public:
			template <typename = std::enable_if_t<instantiation_of<Deferred, T>::value || std::is_base_of<Object, T::value_type>::value>>
			ArrayObject() : _conversion([](const ContainableObjectPtr& obj) { return ObjectUtils::ConvertTo<T>(obj); }) {}

			template <typename = std::enable_if_t<instantiation_of<Deferred, T>::value || std::is_base_of<Object, T::value_type>::value>>
			explicit ArrayObject(const MixedArrayObject& other)
				: _list(other),	_conversion([](const ContainableObjectPtr& obj) { return ObjectUtils::ConvertTo<T>(obj); })
			{ for (auto item : other) _conversion(item); }

			template <typename = std::enable_if_t<instantiation_of<Deferred, T>::value || std::is_base_of<Object, T::value_type>::value>>
			explicit ArrayObject(const list_type& list) : _conversion([](const ContainableObjectPtr& obj) { return ObjectUtils::ConvertTo<T>(obj); })
			{ for (auto item : other) _list->push_back(item); }

			template <typename = std::enable_if_t<instantiation_of<Deferred, T>::value || std::is_base_of<Object, T::value_type>::value>>
			explicit ArrayObject(const std::initializer_list<T>& list) : _conversion([](const ContainableObjectPtr& obj) { return ObjectUtils::ConvertTo<T>(obj); })
			{ for (auto item : list) _list->push_back(item); }

			//explicit ArrayObject(std::function<T(const ContainableObjectPtr& obj)> conversion) : _conversion(conversion) {}
			explicit ArrayObject(const list_type& list, std::function<T(const ContainableObjectPtr& obj)> conversion)
				: _conversion(conversion) { for (auto item : list) _list->push_back(item); }

			explicit ArrayObject(const std::initializer_list<T>& list, std::function<T(const ContainableObjectPtr& obj)> conversion)
				: _conversion(conversion) { for (auto item : list) _list->push_back(item); }

			ArrayObject(const MixedArrayObject& other, std::function<T(const ContainableObjectPtr& obj)> conversion)
			: _list(other), _conversion(conversion)	{ for (auto item : other) _conversion(item); }

			template <typename U>
			ArrayObject(const ArrayObject<U>& other, std::function<T(const U& obj)> new_conversion)
			{
				auto other_conversion = other._conversion;
				_conversion = [other_conversion, new_conversion](const ContainableObjectPtr& obj) { return new_conversion(other_conversion(obj)); };
				for (auto item : other) _list->push_back(item);
			}

			MixedArrayObjectPtr Data(void) const { return _list; }
			size_t Size(void) const { return _list->Size(); }
			const T operator[](size_t i) const { return _conversion((*_list)[i]); }
			T operator[](size_t i) { return _conversion((*_list)[i]); }
			const T At(size_t at) const { return _conversion(_list->At(at)); }
			T At(size_t at) { return _conversion(_list->At(at)); }

			template <typename U>
			ArrayObjectPtr<U> Convert(std::function<U(const T& obj)> f) const
			{
				return ArrayObjectPtr<U>(*this, f);
			}

		public:
			class Iterator : public IUnknown, public std::iterator<std::input_iterator_tag, T>
			{
			public:
				Iterator(std::function<T(const ContainableObjectPtr& obj)> conversion) : _conversion(conversion) {};
				Iterator(typename MixedArrayObject::iterator it, const std::function<T(const ContainableObjectPtr& obj)>& conversion)
					: _it(it), _conversion(conversion) {}

				const Iterator& operator++()
				{
					++_it;
					return *this;
				}

				const Iterator operator++(int)
				{
					Iterator temp(_it);
					++_it;
					return temp;
				}

				bool operator==(const Iterator& other) const
				{
					return _it == other._it;
				}

				bool operator!=(const Iterator& other) const
				{
					return _it != other._it;
				}

				T operator*() const { return _conversion(_it.operator*()); }

			private:
				typename MixedArrayObject::iterator _it;
				std::function<T(const ContainableObjectPtr& obj)> _conversion;
			};

			Iterator begin() { return Iterator(_list->begin(), _conversion); }
			Iterator begin() const { return Iterator(_list->begin(), _conversion); }
			Iterator end() { return Iterator(_list->end(), _conversion); }
			Iterator end() const { return Iterator(_list->end(), _conversion); }

		private:
			MixedArrayObjectPtr _list;
			std::function<T(const ContainableObjectPtr& obj)> _conversion;
		};
	}
}

#endif /* _ARRAY_OBJECT_H */
