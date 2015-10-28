#ifndef _DEFERRED_H
#define _DEFERRED_H

#include <boost/smart_ptr/intrusive_ptr.hpp>

namespace gotchangpdf
{

	 /* The plan was to use template <typename = std::enable_if<std::is_constructible<T>::value>>
	 to determine if the class T has default constructor.
	 Unfortunately class T is not defined yet, which is a must for std::is_constructible.
	 Therefore we use another template parameter, where it is a developer responsibility
	 to provide information about existence of default constructor. */

	/*!
	* \class Deferred
	* \brief Deferred construction container for AST nodes.
	*
	* This class is used to speed up the construction of the AST. The construction of the node is only done when an access to the data is issued.
	* This code has been taken from the Eddi Compiler project (https://github.com/wichtounet/eddic/) and has been adapted a little.
	*/
	template <typename T, bool DefaultConstructible = true>
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

		template <typename... Parameters>
		Deferred(const Parameters&... p) : Contents(new T(p...))
		{
			Content.Owner = this;
		}

		template <typename U>
		Deferred(std::initializer_list<U> list) : Contents(new T(list))
		{
			Content.Owner = this;
		}

		template <typename = std::enable_if<DefaultConstructible>>
		Deferred() : Contents(reinterpret_cast<T*>(nullptr))
		{
			Content.Owner = this;
		}

		inline operator T() { return *Contents; }
		inline operator T() const { return *Contents; }

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
				return get_internal<DefaultConstructible>();
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
	template <typename T, bool DefaultConstructible = true>
	struct DeferredContainer : public Deferred<T, DefaultConstructible>
	{
		typedef typename T::value_type value_type;
		typedef typename T::iterator iterator;
		typedef typename T::const_iterator const_iterator;
		typedef typename T::size_type size_type;
		typedef typename T::reference reference;
		typedef typename T::const_reference const_reference;

		DeferredContainer(T* value) : Deferred(value) {}

		DeferredContainer(const Deferred& rhs) : Deferred(rhs) {}

		template <typename... Parameters>
		DeferredContainer(const Parameters&... p) : Deferred(p...) {}

		template <typename U>
		DeferredContainer(std::initializer_list<U> list) : Deferred(list) {}

		template <typename = std::enable_if<DefaultConstructible>>
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

	template <typename T, bool DefaultConstructible = true>
	struct DeferredIterator: public Deferred<T, DefaultConstructible>
	{
		typedef typename T::value_type value_type;
		typedef typename T::difference_type difference_type;
		typedef typename T::pointer pointer;
		typedef typename T::reference reference;

		DeferredIterator(T* value) : Deferred(value) {}

		DeferredIterator(const Deferred& rhs) : Deferred(rhs) {}

		template <typename... Parameters>
		DeferredIterator(const Parameters&... p) : Deferred(p...) {}

		template <typename U>
		DeferredIterator(std::initializer_list<U> list) : Deferred(list) {}

		template <typename = std::enable_if<DefaultConstructible>>
		DeferredIterator() : Deferred() {}
	};

	template<typename T>
	T* AddRefGet(Deferred<T> obj)
	{
		obj.Content->AddRef();
		return obj.Content.get();
	}

	//template <typename U>
	//inline U* GetAs(void) const { return dynamic_cast<U*>(boost::intrusive_ptr<T>::get()); }
}
#endif /* _DEFERRED_H */
