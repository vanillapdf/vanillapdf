#ifndef _SEMANTIC_EXCEPTIONS_H
#define _SEMANTIC_EXCEPTIONS_H

#include "exception.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class OptionalEntryMissingException : public ExceptionBase
		{
		public:
			explicit OptionalEntryMissingException(const syntax::DictionaryObjectPtr& container, const syntax::NameObjectPtr& entry)
				: ExceptionBase(container->ToString() + "does not contain optional entry: " + entry->ToString(), Type::OptionalEntryMissing) {}
		};

		class SemanticContextException : public ExceptionBase
		{
		public:
			explicit SemanticContextException(const char * const & msg) : ExceptionBase(msg, Type::SemanticContext) {}
			explicit SemanticContextException(const std::string& msg) : ExceptionBase(msg, Type::SemanticContext) {}
		};

		class SemanticContextExceptionFactory
		{
		public:
			template <typename SyntacticObjectT, typename SemanticObjectT>
			static SemanticContextException Construct(const SyntacticObjectT& obj)
			{
				std::string msg(
					std::string("Could not convert object of type ")
					+ typeid(SyntacticObjectT).name()
					+ " to type "
					+ typeid(SemanticObjectT).name()
					+ ". Data: "
					+ obj.ToString()
					);

				return SemanticContextException(msg);
			}
		};
	}
}

#endif /* _SEMANTIC_EXCEPTIONS_H */
