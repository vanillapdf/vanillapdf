#ifndef _ACTIONS_H
#define _ACTIONS_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/destinations.h"

namespace vanillapdf {
namespace semantics {

class ActionBase : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    enum class Type {
        Undefined = 0,
        GoTo,
        GoToRemote,
        URI,
        Launch,
        Named,
        JavaScript
    };

public:
    explicit ActionBase(syntax::DictionaryObjectPtr root);
    static ActionPtr Create(syntax::DictionaryObjectPtr root);

    virtual ActionBase::Type GetActionType() const noexcept = 0;
};

class GoToAction : public ActionBase {
public:
    explicit GoToAction(syntax::DictionaryObjectPtr root);
    virtual ActionBase::Type GetActionType() const noexcept override;

    bool Destination(OutputDestinationPtr& result) const;
};

class URIAction : public ActionBase {
public:
    explicit URIAction(syntax::DictionaryObjectPtr root);
    virtual ActionBase::Type GetActionType() const noexcept override;

    syntax::LiteralStringObjectPtr GetURI() const;
    bool GetURIText(PdfTextStringPtr& result) const;
};

class GoToRemoteAction : public ActionBase {
public:
    explicit GoToRemoteAction(syntax::DictionaryObjectPtr root);
    virtual ActionBase::Type GetActionType() const noexcept override;

    bool Destination(OutputDestinationPtr& result) const;
    syntax::ObjectPtr GetFile() const;
};

class NamedAction : public ActionBase {
public:
    explicit NamedAction(syntax::DictionaryObjectPtr root);
    virtual ActionBase::Type GetActionType() const noexcept override;

    syntax::NameObjectPtr GetName() const;
};

class JavaScriptAction : public ActionBase {
public:
    explicit JavaScriptAction(syntax::DictionaryObjectPtr root);
    virtual ActionBase::Type GetActionType() const noexcept override;

    syntax::ObjectPtr GetJS() const;
};

class LaunchAction : public ActionBase {
public:
    explicit LaunchAction(syntax::DictionaryObjectPtr root);
    virtual ActionBase::Type GetActionType() const noexcept override;
};

inline ActionBase::Type GoToAction::GetActionType() const noexcept { return ActionBase::Type::GoTo; }
inline ActionBase::Type URIAction::GetActionType() const noexcept { return ActionBase::Type::URI; }
inline ActionBase::Type GoToRemoteAction::GetActionType() const noexcept { return ActionBase::Type::GoToRemote; }
inline ActionBase::Type NamedAction::GetActionType() const noexcept { return ActionBase::Type::Named; }
inline ActionBase::Type JavaScriptAction::GetActionType() const noexcept { return ActionBase::Type::JavaScript; }
inline ActionBase::Type LaunchAction::GetActionType() const noexcept { return ActionBase::Type::Launch; }

} // semantics
} // vanillapdf

#endif /* _ACTIONS_H */
