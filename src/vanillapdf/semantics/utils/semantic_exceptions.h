#ifndef _SEMANTIC_EXCEPTIONS_H
#define _SEMANTIC_EXCEPTIONS_H

#include "utils/exceptions.h"

namespace vanillapdf {
namespace semantics {

class SemanticContextException : public ExceptionBase {
public:
    explicit SemanticContextException(const char * const & msg) : ExceptionBase(msg) {}
    explicit SemanticContextException(const std::string& msg) : ExceptionBase(msg) {}
    virtual Type code() const noexcept { return Type::SemanticContext; }
};

class SemanticContextExceptionFactory {
public:
    template <typename SyntacticObjectT, typename SemanticObjectT>
    static SemanticContextException Construct(const SyntacticObjectT& obj) {
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

} // semantics
} // vanillapdf

#endif /* _SEMANTIC_EXCEPTIONS_H */
