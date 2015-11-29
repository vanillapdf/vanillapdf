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
	struct Deferred
	{
		typedef T value_type;

		Deferred(T* value) : Contents(value)
		{
			Content.Owner = this;
		}

		Deferred(const Deferred& rhs) : Contents(rhs.Contents)
		{
			Content.Owner = this;
		}

		template <typename... Parameters, typename = std::enable_if_t<std::is_constructible<T, Parameters...>::value>>
		Deferred(const Parameters&... p) : Contents(new T(p...))
		{
			Content.Owner = this;
		}

		template <typename U, typename = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>>::value>>
		Deferred(std::initializer_list<U> list) : Contents(new T(list))
		{
			Content.Owner = this;
		}

		template <typename = std::enable_if_t<std::is_constructible<T>::value>>
		Deferred() : Contents(reinterpret_cast<T*>(nullptr))
		{
			Content.Owner = this;
		}

		inline operator T&() { return *Contents; }
		inline operator T&() const { return *Contents; }

		inline bool operator==(const Deferred& other) const { return *Contents == *other.Contents; }
		inline bool operator!=(const Deferred& other) const { return *Contents != *other.Contents; }
		inline bool operator<(const Deferred& other) const { return *Contents < *other.Contents; }

		T& operator*() const
		{
			return Content.operator*();
		}

		T* operator->() const
		{
			return Content.operator->();
		}

		Deferred& operator=(const Deferred& rhs)
		{
			if (this != &rhs)
				Contents = rhs.Contents;

			return *this;
		}

		//
		// Wrapper for accessing the content safely
		//
		// Constructs a default node in the parent wrapper if necessary,
		// or simply returns the existing content of the wrapper. This
		// helper is necessary for exposing a compatible interface with
		// boost::fusion adapters.
		//

		/*!
		* \struct SafeContentAccess
		* \brief Provide safe content access to the content of the deferred node
		*/
		struct SafeContentAccess
		{
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
				assert(Owner->Contents);
				if (!Owner->Contents) {
					return nullptr;
				}

				return Owner->Contents.get();
			}

			template <>
			T* get_internal<true>(void) const
			{
				if (!Owner->Contents) {
					Owner->Contents.reset(new T());
				}

				return Owner->Contents.get();
			}

			Deferred* Owner;
		} Content;

		T* AddRefGet(void)
		{
			Content->AddRef();
			return Content.get();
		}

	protected:
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
	struct DeferredContainer : public Deferred<T>
	{
		typedef typename T::value_type value_type;
		typedef typename T::iterator iterator;
		typedef typename T::const_iterator const_iterator;
		typedef typename T::size_type size_type;
		typedef typename T::reference reference;
		typedef typename T::const_reference const_reference;

		DeferredContainer(T* value) : Deferred(value) {}

		DeferredContainer(const Deferred& rhs) : Deferred(rhs) {}

		template <typename... Parameters, typename = std::enable_if_t<std::is_constructible<T, Parameters...>::value>>
		DeferredContainer(const Parameters&... p) : Deferred(p...) {}

		template <typename U, typename = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>>::value>>
		DeferredContainer(std::initializer_list<U> list) : Deferred(list) {}

		template <typename = std::enable_if_t<std::is_constructible<T>::value>>
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
	struct DeferredIterator: public Deferred<T>
	{
		typedef typename T::value_type value_type;
		typedef typename T::difference_type difference_type;
		typedef typename T::pointer pointer;
		typedef typename T::reference reference;

		DeferredIterator(T* value) : Deferred(value) {}

		DeferredIterator(const Deferred& rhs) : Deferred(rhs) {}

		template <typename... Parameters, typename = std::enable_if_t<std::is_constructible<T, Parameters...>::value>>
		DeferredIterator(const Parameters&... p) : Deferred(p...) {}

		template <typename U, typename = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>>::value>>
		DeferredIterator(std::initializer_list<U> list) : Deferred(list) {}

		template <typename = std::enable_if_t<std::is_constructible<T>::value>>
		DeferredIterator() : Deferred() {}
	};
}
#endif /* _DEFERRED_H */
