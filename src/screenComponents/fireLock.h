#ifndef FIRE_LOCK_H
#define FIRE_LOCK_H

#include "gui/gui2_togglebutton.h"
#include "screenComponents/radarView.h"
#include "spaceObjects/playerSpaceship.h"

class FireLockButton : public GuiToggleButton
{
private:
    P<PlayerSpaceship> target_spaceship;

public:
    FireLockButton(GuiContainer* owner, string id, P<PlayerSpaceship> targetSpaceship);
    
    virtual void onDraw(sf::RenderTarget& window) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    void setTargetSpaceship(P<PlayerSpaceship> targetSpaceship){target_spaceship = targetSpaceship;}
private:
    void setFireLock(bool value);
};

#endif//FIRE_LOCK_H
