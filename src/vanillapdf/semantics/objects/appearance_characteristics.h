#ifndef _APPEARANCE_CHARACTERISTICS_H
#define _APPEARANCE_CHARACTERISTICS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/objects/dictionary_object.h"

namespace vanillapdf {
namespace semantics {

// Appearance characteristics dictionary (/MK) of a widget annotation,
// see Table 189. It describes how the widget shall be presented when the
// appearance streams are regenerated - only entries that are already
// modelled here are exposed, the remainder is reachable through GetObject.
class AppearanceCharacteristics : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    AppearanceCharacteristics() = default;
    explicit AppearanceCharacteristics(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {}

    // Number of degrees by which the widget is rotated counterclockwise
    // relative to the page - a multiple of 90 (/R)
    bool GetRotation(types::big_int& result) const;
    void SetRotation(types::big_int value);

    // Colour of the widget's border (/BC). An empty colour means no border
    // is painted, the number of components selects the colour space.
    bool GetBorderColor(OutputColorPtr& result) const;
    void SetBorderColor(ColorPtr value);

    // Colour of the widget's background (/BG), same encoding as /BC
    bool GetBackgroundColor(OutputColorPtr& result) const;
    void SetBackgroundColor(ColorPtr value);

    // Caption displayed when the widget is not interacting with the user (/CA)
    bool GetNormalCaption(syntax::OutputStringObjectPtr& result) const;
    void SetNormalCaption(syntax::StringObjectPtr value);

    // Caption displayed when the cursor enters the widget (/RC)
    bool GetRolloverCaption(syntax::OutputStringObjectPtr& result) const;
    void SetRolloverCaption(syntax::StringObjectPtr value);

    // Caption displayed when the mouse button is pressed within the widget (/AC)
    bool GetDownCaption(syntax::OutputStringObjectPtr& result) const;
    void SetDownCaption(syntax::StringObjectPtr value);
};

} // semantics
} // vanillapdf

#endif /* _APPEARANCE_CHARACTERISTICS_H */
