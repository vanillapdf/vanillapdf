#include "precompiled.h"

#include "semantics/objects/actions.h"
#include "semantics/objects/destinations.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

ActionBase::ActionBase(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
GoToAction::GoToAction(syntax::DictionaryObjectPtr root) : ActionBase(root) {}
URIAction::URIAction(syntax::DictionaryObjectPtr root) : ActionBase(root) {}
GoToRemoteAction::GoToRemoteAction(syntax::DictionaryObjectPtr root) : ActionBase(root) {}
NamedAction::NamedAction(syntax::DictionaryObjectPtr root) : ActionBase(root) {}
JavaScriptAction::JavaScriptAction(syntax::DictionaryObjectPtr root) : ActionBase(root) {}
LaunchAction::LaunchAction(syntax::DictionaryObjectPtr root) : ActionBase(root) {}

std::unique_ptr<ActionBase> ActionBase::Create(syntax::DictionaryObjectPtr root) {
    if (!root->Contains(constant::Name::S)) {
        throw GeneralException("Action dictionary does not contain /S entry");
    }

    syntax::ObjectPtr s_obj = root->Find(constant::Name::S);

    if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(s_obj)) {
        throw GeneralException("Invalid action type");
    }

    syntax::NameObjectPtr s = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(s_obj);

    if (s == constant::Name::GoTo) {
        return make_unique<GoToAction>(root);
    }

    if (s == constant::Name::GoToR) {
        return make_unique<GoToRemoteAction>(root);
    }

    if (s == constant::Name::URI) {
        return make_unique<URIAction>(root);
    }

    if (s == constant::Name::Launch) {
        return make_unique<LaunchAction>(root);
    }

    if (s == constant::Name::Named) {
        return make_unique<NamedAction>(root);
    }

    if (s == constant::Name::JavaScript) {
        return make_unique<JavaScriptAction>(root);
    }

    throw GeneralException("Unknown action type: " + s->ToString());
}

bool GoToAction::Destination(OutputDestinationPtr& result) const {
    if (!_obj->Contains(constant::Name::D)) {
        return false;
    }

    try {
        auto dest_obj = _obj->Find(constant::Name::D);
        auto destination = DestinationBase::ResolveDestination(dest_obj);
        result = destination;
        return true;
    }
    catch (ExceptionBase& ex) {
        spdlog::warn("Could not resolve GoTo action destination: {}", ex.what());
        return false;
    }
}

syntax::LiteralStringObjectPtr URIAction::GetURI() const {
    return _obj->FindAs<syntax::LiteralStringObjectPtr>(constant::Name::URI);
}

bool GoToRemoteAction::Destination(OutputDestinationPtr& result) const {
    if (!_obj->Contains(constant::Name::D)) {
        return false;
    }

    try {
        auto dest_obj = _obj->Find(constant::Name::D);

        // For GoToR, /D can be either an array or a name/string
        if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(dest_obj)) {
            auto array_obj = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(dest_obj);
            result = DestinationBase::CreateFromArray(array_obj);
            return true;
        }

        return false;
    }
    catch (ExceptionBase& ex) {
        spdlog::warn("Could not resolve GoToR action destination: {}", ex.what());
        return false;
    }
}

syntax::ObjectPtr GoToRemoteAction::GetFile() const {
    return _obj->Find(constant::Name::F);
}

syntax::NameObjectPtr NamedAction::GetName() const {
    return _obj->FindAs<syntax::NameObjectPtr>(constant::Name::N);
}

syntax::ObjectPtr JavaScriptAction::GetJS() const {
    return _obj->Find(constant::Name::JS);
}

} // semantics
} // vanillapdf
