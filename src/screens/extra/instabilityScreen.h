#ifndef SYSTEM_SCREEN_H
#define SYSTEM_SCREEN_H

#include "gui/gui2_overlay.h"
#include "shipTemplate.h"
#include "playerInfo.h"

class GuiPanel;
class GuiImage;
class GuiSlider;
class GuiLabel;
class GuiProgressbar;
class GuiKeyValueDisplay;
class GuiAutoLayout;
class GuiSelector;
class GuiElement;
class GuiSystemEffectsList;
class GuiToggleButton;

class InstabilityControlScreen : public GuiOverlay
{
private:
    GuiOverlay* background_crosses;
    GuiAutoLayout* system_info;
    GuiAutoLayout* instability_info;
    GuiElement* instability_container;
    GuiSystemEffectsList* system_effects_container;
    
    GuiAutoLayout* system_layout;
    GuiToggleButton* system_selector[SYS_COUNT];
    
    GuiProgressbar* damage_bar;
    GuiProgressbar* heat_bar;
    GuiProgressbar* power_bar;
    GuiProgressbar* coolant_bar;
    GuiProgressbar* repair_bar;
    GuiProgressbar* effectiveness_bar;
    
    GuiLabel* damage_title;
    GuiLabel* damage_label;
    GuiLabel* heat_title;
    GuiLabel* heat_label;
    GuiLabel* power_title;
    GuiLabel* power_label;
    GuiLabel* coolant_title;
    GuiLabel* coolant_label;
    GuiLabel* repair_title;
    GuiLabel* repair_label;
    GuiLabel* effectiveness_title;
    GuiLabel* effectiveness_label;
    GuiLabel* instability_sum_label;
    GuiLabel* instability_label[4];
    GuiSlider* instability_slider[4];
    
    GuiImage* damage_icon;
    GuiImage* heat_icon;
    GuiImage* power_icon;
    GuiImage* coolant_icon;
    GuiImage* repair_icon;
    GuiImage* effectiveness_icon;
    
    ESystem selected_system = SYS_None;
    void selectSystem(ESystem system);
    
public:
    InstabilityControlScreen(GuiContainer* owner, ECrewPosition crew_position=instabilityControl);
    
    void onDraw(sf::RenderTarget& window) override;
    void updateInstability(sf::RenderTarget& window, GuiElement* container);
    void onHotkey(const HotkeyResult& key) override;
};

#endif//SYSTEM_SCREEN_H
