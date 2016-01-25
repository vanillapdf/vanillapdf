#ifndef _DEFERRED_H
#define _DEFERRED_H

#include <boost/smart_ptr/intrusive_ptr.hpp>

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

		Deferred(T* value) : Contents(value)
		{
			assert(nullptr != value);
			Content.Owner = this;
		}

		Deferred(const Deferred& rhs) : Contents(rhs.Contents)
		{
			Content.Owner = this;
		}

		Deferred(Deferred&& rhs) : Contents(std::move(rhs.Contents))
		{
			Content.Owner = this;
		}

		template <typename U, typename = std::enable_if_t<std::is_convertible<U*, T*>::value>>
		Deferred(const Deferred<U>& rhs) : Contents(rhs.get())
		{
			Content.Owner = this;
		}

		template <typename... Parameters, typename = std::enable_if_t<std::is_constructible<T, Parameters...>::value>>
		Deferred(const Parameters&... p) : Contents(pdf_new T(p...))
		{
			Content.Owner = this;
		}

		template <typename U, typename = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>>::value>>
		Deferred(std::initializer_list<U> list) : Contents(pdf_new T(list))
		{
			Content.Owner = this;
		}

		//template <typename = std::enable_if_t<std::is_constructible<T>::value>>
		Deferred() : Contents(reinterpret_cast<T*>(nullptr))
		{
			Content.Owner = this;
		}

		operator T&() { return *Contents; }
		operator T&() const { return *Contents; }

		bool operator==(const Deferred& other) const { return *Contents == *other.Contents; }
		bool operator!=(const Deferred& other) const { return *Contents != *other.Contents; }
		bool operator<(const Deferred& other) const { return *Contents < *other.Contents; }

		T& operator*() const
		{
			return *Content.get();
		}

		T* operator->() const
		{
			return Content.get();
		}

		Deferred& operator=(const Deferred& rhs)
		{
			Contents = rhs.Contents;
			return *this;
		}

		Deferred& operator=(Deferred&& rhs)
		{
			//if (this != &rhs)
			Contents = std::move(rhs.Contents);
			return *this;
		}

		T* get(void) const
		{
			return Content.get();
		}

		T* AddRefGet(void)
		{
			Content->AddRef();
			return Content.get();
		}

		virtual ~Deferred()
		{
			if (this == Content.Owner) {
				Content.Owner = nullptr;
			}
		}

	protected:

		/*!
		* \struct SafeContentAccess
		* \brief Provide safe content access to the content of the deferred node
		*/
		class SafeContentAccess
		{
		public:
			T& operator*() const
			{
				return *get();
			}

			T* operator->() const
			{
				return get();
			}

			T* get(void) const
			{
				return get_internal<std::is_constructible<T>::value>();
			}

			template <bool Constructible>
			T* get_internal(void) const
			{
				assert(Owner && Owner->Contents);
				if (!Owner->Contents) {
					return nullptr;
				}

				return Owner->Contents.get();
			}

			template <>
			T* get_internal<true>(void) const
			{
				assert(Owner);
				if (!Owner->Contents) {
					Owner->Contents.reset(pdf_new T());
				}

				return Owner->Contents.get();
			}

			Deferred* Owner = nullptr;
		};

	protected:
		SafeContentAccess Content;

	private:
		mutable boost::intrusive_ptr<T> Contents;
	};

	/*!
	* \class DeferredContainer
	* \brief DeferredContainer construction container for AST nodes.
	*
	* This class is used to speed up the construction of the AST. The construction of the node is only done when an access to the data is issued.
	* This code has been taken from the Epoch Compiler project (http://code.google.com/p/epoch-language/) and has been adapted a little.
	*/
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
		DeferredContainer(T* value) : Deferred(value) {}
		DeferredContainer(const Deferred& rhs) : Deferred(rhs) {}

		template <typename... Parameters, typename = std::enable_if_t<std::is_constructible<T, Parameters...>::value>>
		DeferredContainer(const Parameters&... p) : Deferred(p...) {}

		template <typename U, typename = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>>::value>>
		DeferredContainer(std::initializer_list<U> list) : Deferred(list) {}

		//template <typename = std::enable_if_t<std::is_constructible<T>::value>>
		DeferredContainer() : Deferred() {}

		// Support insertion as if this were itself a container
		void insert(const iterator& pos, const value_type& value)
		{
			Content->insert(pos, value);
		}

		// Retrieve a starting iterator as if this were itself a container
		iterator begin()
		{
			return Content->begin();
		}

		// Retrieve an ending iterator as if this were itself a container
		iterator end()
		{
			return Content->end();
		}

		// Check if the wrapped container is empty
		bool empty() const
		{
			return Content->empty();
		}

		const_reference operator[](size_type i) const
		{
			return Content->operator[](i);
		}

		reference operator[](size_type i)
		{
			return Content->operator[](i);
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
		DeferredIterator(T* value) : Deferred(value) {}
		DeferredIterator(const Deferred& rhs) : Deferred(rhs) {}

		template <typename... Parameters, typename = std::enable_if_t<std::is_constructible<T, Parameters...>::value>>
		DeferredIterator(const Parameters&... p) : Deferred(p...) {}

		template <typename U, typename = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>>::value>>
		DeferredIterator(std::initializer_list<U> list) : Deferred(list) {}

		//template <typename = std::enable_if_t<std::is_constructible<T>::value>>
		DeferredIterator() : Deferred() {}
	};
}
#endif /* _DEFERRED_H */
