#ifndef WARP_FREQUENCY_SELECTOR_H
#define WARP_FREQUENCY_SELECTOR_H

#include "gui/gui2_selector.h"

class GuiWarpFrequencySelector : public GuiSelector
{
public:
    GuiWarpFrequencySelector(GuiContainer* owner, string id);

    virtual void onHotkey(const HotkeyResult& key) override;
};

#endif//WARP_FREQUENCY_SELECTOR_H
