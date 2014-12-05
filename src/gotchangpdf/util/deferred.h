#ifndef _DEFERRED_H
#define _DEFERRED_H

#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace gotchangpdf
{
	using boost::sp_adl_block::intrusive_ptr_add_ref;
	using boost::sp_adl_block::intrusive_ptr_release;

	template <typename T>
	inline T* Allocate() { return new T; }

	/*
	template <typename T>
	inline void Deallocate(T* ptr) { delete ptr; }

	template<typename T>
	inline void intrusive_ptr_add_ref(T* expr) { ++expr->references; }

	template<typename T>
	inline void intrusive_ptr_release(T* expr)
	{
	if (--expr->references == 0)
	Deallocate(expr);
	}
	*/

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
		typedef typename T value_type;

		Deferred() : Contents(reinterpret_cast<T*>(nullptr)){
			Content.Owner = this;
		}

		Deferred(const T& expr) : Contents(new (Allocate<T>()) T(expr)){
			Content.Owner = this;
		}

		Deferred(const Deferred& rhs) : Contents(rhs.Contents){
			Content.Owner = this;
		}

		template <typename VariantContentT>
		Deferred(const VariantContentT& content) : Contents(new (Allocate<T>()) T(content))
		{
			Content.Owner = this;
		}

		bool HasContents(void) const { return nullptr != Contents; }

		operator bool() const { return HasContents(); }

		operator T()
		{
			if (Contents)
				return *Contents;

			return T();
		}

		operator T() const
		{
			if (Contents)
				return *Contents;

			return T();
		}

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
				if (!Owner->Contents)
					Owner->Contents.reset(new (Allocate<T>()) T());

				return Owner->Contents.get();
			}

			Deferred* Owner;
		} Content;

		T* AddRefGet(void)
		{
			//auto ptr = boost::intrusive_ptr<T>::get();
			//auto ptr = (*obj.Content).get();
			auto ptr = Content.get();
			boost::sp_adl_block::intrusive_ptr_add_ref(ptr);
			return ptr;
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
	struct DeferredContainer
	{
		typedef typename T::value_type value_type;
		typedef typename T::iterator iterator;
		typedef typename T::const_iterator const_iterator;

		DeferredContainer()
			: Contents(reinterpret_cast<T*>(nullptr))
		{
			Content.Owner = this;
		}

		DeferredContainer(const T& expr)
			: Contents(new (Allocate<T>()) T(expr))
		{
			Content.Owner = this;
		}

		DeferredContainer(const DeferredContainer& rhs)
			: Contents(rhs.Contents)
		{
			Content.Owner = this;
		}

		//
		// Construction from a pair of some indeterminate values
		//
		// This is predominantly used when an iterator range is passed
		// to the constructor; to make the wrapper look as if it holds
		// an underlying iterator pair, we use this constructor.
		//
		template <typename ContentOfSomeKind>
		DeferredContainer(const ContentOfSomeKind& a, const ContentOfSomeKind& b)
			: Contents(new (Allocate<T>()) T())
		{
			Content.Owner = this;
			Contents->Container.push_back(value_type(a, b));
		}

		DeferredContainer& operator=(const DeferredContainer& rhs)
		{
			if (this != &rhs)
				Contents = rhs.Contents;

			return *this;
		}

		bool HasContents(void) const { return nullptr != Contents; }
		operator bool() const { return HasContents(); }

		// Support insertion as if this were itself a container
		void insert(const iterator& pos, const value_type& value)
		{
			Content->Container.insert(pos, value);
		}

		// Retrieve a starting iterator as if this were itself a container
		iterator begin()
		{
			return Content->Container.begin();
		}

		// Retrieve an ending iterator as if this were itself a container
		iterator end()
		{
			return Content->Container.end();
		}

		// Check if the wrapped container is empty
		bool empty() const
		{
			return Content->Container.empty();
		}

		//
		// Wrapper for accessing the content safely
		//
		// Constructs a default node in the parent wrapper if necessary,
		// or simply returns the existing content of the wrapper. This
		// helper is necessary for exposing a compatible interface with
		// boost::fusion adapters.
		//
		struct SafeContentAccess
		{
			// Dereference operator
			T& operator*() const
			{
				return *get();
			}

			// Indirection operator
			T* operator->() const
			{
				return get();
			}

			T* get(void) const
			{
				if (!Owner->Contents)
					Owner->Contents.reset(new (Allocate<T>()) T());

				return Owner->Contents.get();
			}

			// Linkage to the owning wrapper
			DeferredContainer* Owner;
		} Content;

	protected:
		// Internal state
		mutable boost::intrusive_ptr<T> Contents;
	};

	template<typename T>
	T* AddRefGet(Deferred<T> obj)
	{
		//auto ptr = boost::intrusive_ptr<T>::get();
		//auto ptr = (*obj.Content).get();
		auto ptr = obj.Content.get();
		boost::sp_adl_block::intrusive_ptr_add_ref(ptr);
		return ptr;
	}

	//template <typename U>
	//inline U* GetAs(void) const { return dynamic_cast<U*>(boost::intrusive_ptr<T>::get()); }
}
#endif /* _DEFERRED_H */
