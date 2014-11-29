#ifndef _OBJECT_VISITORS_H
#define _OBJECT_VISITORS_H

#include "object.h"
#include "direct_object.h"
#include "exception.h"

#include <boost/variant/static_visitor.hpp>

namespace gotchangpdf
{
	class IsNullVisitor : public boost::static_visitor<bool>
	{
	public:

		template <typename T>
		bool operator()(T obj) const
		{
			return std::is_same<T, NullObjectPtr>::value;
			//return (T == NullObject);
		}

		/*
		bool operator()(DirectObject obj) const
		{
			return (obj.which() == 0);
		}
		*/
	};

	class ObjectBaseVisitor : public boost::static_visitor<SmartPtr<Object>>
	{
	public:
		template <typename T>
		SmartPtr<Object> operator()(Deferred<T> obj) const
		{
			return obj.Content.get();
		}
		/*
		DirectObject operator()(StringObjectPtr obj) const
		{
			return obj.apply_visitor(*this);
		}
		*/
	};

	template <typename T>
	class ObjectVisitor : public boost::static_visitor<T>
	{
	public:
		Deferred<T> operator()(Deferred<T> obj) const
		{
			return obj;
		}

		template <typename U>
		T operator()(U obj) const
		{
			throw exceptions::Exception("Type cast error");
		}
	};

	class DirectToBaseVisitor : public boost::static_visitor<boost::intrusive_ptr<Object>>
	{
	public:
		template <typename T>
		boost::intrusive_ptr<Object> operator()(T obj) const
		{
			return obj.Content.get();
		}
	};
	/*
	class BaseToDirectVisitor : public boost::static_visitor<DirectObject>
	{
	public:
		DirectObject operator()(boost::intrusive_ptr<Object> obj) const
		{

			auto type = obj->GetType();
			switch (type)
			{
			case gotchangpdf::Object::Type::ArrayObject:
				return dynamic_cast<MixedArrayObject*>(obj.get());
			case gotchangpdf::Object::Type::Boolean:
				return dynamic_cast<BooleanObject*>(obj.get());
			case gotchangpdf::Object::Type::DictionaryObject:
				return dynamic_cast<DictionaryObject*>(obj.get());
			case gotchangpdf::Object::Type::Function:
				return dynamic_cast<FunctionObject*>(obj.get());
			case gotchangpdf::Object::Type::IntegerObject:
				return dynamic_cast<IntegerObject*>(obj.get());
			case gotchangpdf::Object::Type::NameObject:
				return dynamic_cast<NameObject*>(obj.get());
			case gotchangpdf::Object::Type::NullObject:
				return dynamic_cast<NullObject*>(obj.get());
			case gotchangpdf::Object::Type::RealObject:
				return dynamic_cast<RealObject*>(obj.get());
			case gotchangpdf::Object::Type::StreamObject:
				return dynamic_cast<StreamObject*>(obj.get());
			case gotchangpdf::Object::Type::HexadecimalString:
				return dynamic_cast<HexadecimalString*>(obj.get());
			case gotchangpdf::Object::Type::LiteralString:
				return dynamic_cast<LiteralString*>(obj.get());
			case gotchangpdf::Object::Type::IndirectObjectReference:
				return dynamic_cast<IndirectObjectReference*>(obj.get());
			case gotchangpdf::Object::Type::IndirectObject:
				return dynamic_cast<IndirectObject*>(obj.get());
			case gotchangpdf::Object::Type::Unknown:
			default:
				throw exceptions::Exception("Invalid type");
			}

		}
	};
	*/
	/*
	inline SmartPtr<Object> DirectObjectGetBase(DirectObject obj)
	{

	}
	*/
	template <typename T>
	inline Deferred<T> DirectObjectGetAs(DirectObject obj)
	{
		ObjectVisitor<T> visitor;
		return obj.apply_visitor(visitor);
	}
}

#endif /* _OBJECT_VISITORS_H */
