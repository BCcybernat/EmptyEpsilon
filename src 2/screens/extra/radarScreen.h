#ifndef RADAR_SCREEN_H
#define RADAR_SCREEN_H

#include "gui/gui2_overlay.h"

class GuiRadarView;

class RadarScreen : public GuiOverlay
{
private:
    GuiRadarView* radar;
public:
    string type;
    RadarScreen(GuiContainer* owner, string type);
    void onDraw(sf::RenderTarget& window);
    void onHotkey(const HotkeyResult& key);
};

#endif//RADAR_SCREEN_H
