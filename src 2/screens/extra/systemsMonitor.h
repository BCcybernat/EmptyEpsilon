#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include "gui/gui2_overlay.h"
#include "shipTemplate.h"

class GuiPanel;
class GuiKeyValueDisplay;
class GuiSystemEffectsList;

class SystemsMonitorScreen : public GuiOverlay
{
private:
    class SystemRow
    {
    public:
        GuiPanel* box;
        GuiSystemEffectsList* system_effects_container;
    };
    SystemRow systems[SYS_COUNT];
    
public:
    SystemsMonitorScreen(GuiContainer* owner);
    
    void onDraw(sf::RenderTarget& window) override;
};

#endif//SYSTEM_MONITOR_H
