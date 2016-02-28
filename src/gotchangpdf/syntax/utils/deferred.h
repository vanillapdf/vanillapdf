#ifndef _DEFERRED_H
#define _DEFERRED_H

#include <initializer_list>
#include <cassert>

namespace gotchangpdf
{
	/*!
	* \class Deferred
	* \brief Deferred construction container for AST nodes.
	*
	* This class is used to speed up the construction of the AST. The construction of the node is only done when an access to the data is issued.
	* This code has been taken from the Eddi Compiler project (https://github.com/wichtounet/eddic/) and has been adapted a little.
	*/
	template <typename T>
	class Deferred
	{
	public:
		typedef T value_type;

	public:
		//template <typename = std::enable_if_t<std::is_constructible<T>::value>>
		Deferred() : m_ptr(reinterpret_cast<T*>(nullptr)) {}

		Deferred(T* value, bool add_ref) : m_ptr(value) { if (m_ptr && add_ref) m_ptr->AddRef(); }

		Deferred(T* value) : Deferred(value, true) {}
		Deferred(const Deferred& rhs) : Deferred(rhs.m_ptr, true) {}
		Deferred(Deferred&& rhs) : Deferred(rhs.m_ptr, false) { rhs.m_ptr = nullptr; }

		template <typename U, typename = std::enable_if_t<std::is_convertible<U*, T*>::value>>
		Deferred(const Deferred<U>& rhs) : Deferred(rhs.get(), true) {}

		template <typename... Parameters, typename = std::enable_if_t<std::is_constructible<T, Parameters...>::value>>
		Deferred(const Parameters&... p) : Deferred(pdf_new T(p...), true) {}

		template <typename U, typename = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>>::value>>
		Deferred(std::initializer_list<U> list) : Deferred(pdf_new T(list), true) {}

		operator T&() { return *get(); }
		operator T&() const { return *get(); }
		//operator bool() const { return Contents; }

		Deferred& operator=(const Deferred& rhs)
		{
			Deferred(rhs).swap(*this);
			return *this;
		}

		Deferred& operator=(Deferred&& rhs)
		{
			Deferred(std::move(rhs)).swap(*this);
			return *this;
		}

		bool empty(void) const { return m_ptr == nullptr; }

		T* get(void) const
		{
			return get_internal<std::is_constructible<T>::value>();
		}

		template <bool Constructible>
		T* get_internal(void) const
		{
			assert(m_ptr);
			if (!m_ptr) {
				return nullptr;
			}

			return m_ptr;
		}

		template <>
		T* get_internal<true>(void) const
		{
			if (!m_ptr) {
				m_ptr = pdf_new T();
				m_ptr->AddRef();
			}

			return m_ptr;
		}

		void reset() { Deferred().swap(*this); }
		void reset(T* rhs) { Deferred(rhs).swap(*this); }
		T* detach()
		{
			T* result = get();
			m_ptr = nullptr;
			return result;
		}

		T& operator*() const { return *get(); }
		T* operator->() const { return get(); }
		T* AddRefGet(void) { return detach(); }

		void swap(Deferred& rhs) noexcept
		{
			T* tmp = m_ptr;
			m_ptr = rhs.m_ptr;
			rhs.m_ptr = tmp;
		}

		virtual ~Deferred() { if (m_ptr) m_ptr->Release(); }

	private:
		mutable T* m_ptr = nullptr;
	};

	template <typename T>
	class DeferredContainer : public Deferred<T>
	{
	public:
		typedef typename T::value_type value_type;
		typedef typename T::iterator iterator;
		typedef typename T::const_iterator const_iterator;
		typedef typename T::size_type size_type;
		typedef typename T::reference reference;
		typedef typename T::const_reference const_reference;

	public:
		//template <typename = std::enable_if_t<std::is_constructible<T>::value>>
		DeferredContainer() : Deferred() {}

		DeferredContainer(T* value) : Deferred(value) {}
		DeferredContainer(T* value, bool add_ref) : Deferred(value, add_ref) {}
		DeferredContainer(const Deferred& rhs) : Deferred(rhs) {}
		DeferredContainer(Deferred&& rhs) : Deferred(rhs) {}

		template <typename... Parameters, typename = std::enable_if_t<std::is_constructible<T, Parameters...>::value>>
		DeferredContainer(const Parameters&... p) : Deferred(p...) {}

		template <typename U, typename = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>>::value>>
		DeferredContainer(std::initializer_list<U> list) : Deferred(list) {}

		// Support insertion as if this were itself a container
		void insert(const iterator& pos, const value_type& value)
		{
			get()->insert(pos, value);
		}

		// Retrieve a starting iterator as if this were itself a container
		iterator begin()
		{
			return get()->begin();
		}

		// Retrieve an ending iterator as if this were itself a container
		iterator end()
		{
			return get()->end();
		}

		// Check if the wrapped container is empty
		bool empty() const
		{
			return get()->empty();
		}

		const_reference operator[](size_type i) const
		{
			return get()->operator[](i);
		}

		reference operator[](size_type i)
		{
			return get()->operator[](i);
		}
	};

	template <typename T>
	class DeferredIterator: public Deferred<T>
	{
	public:
		typedef typename T::value_type value_type;
		typedef typename T::difference_type difference_type;
		typedef typename T::pointer pointer;
		typedef typename T::reference reference;

	public:
		//template <typename = std::enable_if_t<std::is_constructible<T>::value>>
		DeferredIterator() : Deferred() {}

		DeferredIterator(T* value) : Deferred(value) {}
		DeferredIterator(T* value, bool add_ref) : Deferred(value, add_ref) {}
		DeferredIterator(const Deferred& rhs) : Deferred(rhs) {}
		DeferredIterator(Deferred&& rhs) : Deferred(rhs) {}

		template <typename... Parameters, typename = std::enable_if_t<std::is_constructible<T, Parameters...>::value>>
		DeferredIterator(const Parameters&... p) : Deferred(p...) {}

		template <typename U, typename = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>>::value>>
		DeferredIterator(std::initializer_list<U> list) : Deferred(list) {}
	};

	// conversion operators

	template<class T, class U>
	Deferred<T> static_pointer_cast(const Deferred<U>& p)
	{
		return static_cast<T*>(p.get());
	}

	template<class T, class U>
	Deferred<T> const_pointer_cast(const Deferred<U>& p)
	{
		return const_cast<T*>(p.get());
	}

	template<class T, class U>
	Deferred<T> dynamic_pointer_cast(const Deferred<U>& p)
	{
		return dynamic_cast<T*>(p.get());
	}

	// comparison operators

	template <typename T, typename U>
	inline bool operator==(const Deferred<T>& left, const Deferred<U>& right)
	{
		return (left.get() == right.get());
	}

	template <typename T, typename U>
	inline bool operator!=(const Deferred<T>& left, const Deferred<U>& right)
	{
		return (left.get() != right.get());
	}

	template <typename T, typename U>
	inline bool operator==(const Deferred<T>& left, U* right)
	{
		return (left.get() == right);
	}

	template <typename T, typename U>
	inline bool operator!=(const Deferred<T>& left, U* right)
	{
		return (left.get() != right);
	}

	template <typename T, typename U>
	inline bool operator==(U* left, const Deferred<U>& right)
	{
		return (left == right.get());
	}

	template <typename T, typename U>
	inline bool operator!=(U* left, const Deferred<U>& right)
	{
		return (left != right.get());
	}

	template <typename T>
	inline bool operator==(const Deferred<T>& left, std::nullptr_t) noexcept
	{
		return (nullptr == left.get());
	}

	template <typename T>
	inline bool operator==(std::nullptr_t, const Deferred<T>& right) noexcept
	{
		return (nullptr == right.get());
	}

	template <typename T>
	inline bool operator!=(const Deferred<T>& left, std::nullptr_t) noexcept
	{
		return (nullptr != left.get());
	}

	template <typename T>
	inline bool operator!=(std::nullptr_t, const Deferred<T>& right) noexcept
	{
		return (nullptr != right.get());
	}

	template <typename T>
	inline bool operator<(const Deferred<T>& left, const Deferred<T>& right)
	{
		return std::less<T*>()(left.get(), right.get());
	}

	// swap
	template <typename T>
	void swap(Deferred<T>& lhs, Deferred<T>& rhs)
	{
		lhs.swap(rhs);
	}
}
#endif /* _DEFERRED_H */
