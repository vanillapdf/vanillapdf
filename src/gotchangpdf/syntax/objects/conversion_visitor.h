#ifndef _CONVERSION_VISITOR_H
#define _CONVERSION_VISITOR_H

#include "object.h"
#include "exception.h"
#include "indirect_object_reference.h"
#include "dictionary_object.h"
#include "array_object.h"

#include <map>
#include <sstream>

#include <boost/variant/static_visitor.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		template <typename T>
		class ConversionVisitor : public boost::static_visitor<T>
		{
		public:
			T operator()(IndirectObjectReferencePtr& obj) const
			{
				auto found = visited.find(*obj);
				if (found != visited.end()) {
					std::stringstream ss;
					ss << "Cyclic reference was found for " << obj->GetReferencedObjectNumber() << " " << obj->GetReferencedGenerationNumber() << " R";
					throw Exception(ss.str());
				}

				visited[*obj] = true;

				auto direct = obj->GetReferencedObject();
				return direct.apply_visitor(*this);
			}

			inline T operator()(T& obj) const { return obj; }

			template <typename U>
			inline T operator()(const U&) const { throw Exception("Type cast error"); }

		private:
			mutable std::map<IndirectObjectReference, bool> visited;
		};

		template <>
		class ConversionVisitor<IndirectObjectReferencePtr> : public boost::static_visitor<IndirectObjectReferencePtr>
		{
		public:
			inline IndirectObjectReferencePtr operator()(const IndirectObjectReferencePtr& obj) const { return obj; }

			template <typename U>
			inline IndirectObjectReferencePtr operator()(const U&) const { throw Exception("Type cast error"); }
		};

		template <typename T>
		class ConversionVisitor<ArrayObjectPtr<T>> : public boost::static_visitor<ArrayObjectPtr<T>>
		{
		public:
			inline ArrayObjectPtr<T> operator()(MixedArrayObjectPtr& obj) const { return obj->CastToArrayType<T>(); }

			template <typename U>
			inline ArrayObjectPtr<T> operator()(const U&) const { throw Exception("Type cast error"); }
		};
	}
}

#endif /* _CONVERSION_VISITOR_H */
