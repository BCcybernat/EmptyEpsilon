#ifndef SYSTEM_EFFECTS_LIST_H
#define SYSTEM_EFFECTS_LIST_H

#include "gui/gui2_autolayout.h"
#include "gui/gui2_keyvaluedisplay.h"

class GuiAutoLayout;
class GuiKeyValueDisplay;

class GuiSystemEffectsList : public GuiAutoLayout
{
private:
    std::vector<GuiKeyValueDisplay*> system_effects;
    unsigned int system_effects_index;
    ESystem selected_system;
    bool primary_info_visible;
    bool background;
    
    void addSystemEffect(string key, string value);

public:
    GuiSystemEffectsList(GuiContainer* owner, string id, GuiAutoLayout::ELayoutMode layout);
    void selectSystem(ESystem system);
    void addSystemEnergy(ESystem system);
    void setPrimaryInfoVisible(bool visible);
    void isBackground(bool background);
    virtual void onDraw(sf::RenderTarget& window) override;
};

#endif//SYSTEM_EFFECTS_LIST_H
