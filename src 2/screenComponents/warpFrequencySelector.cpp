#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "warpFrequencySelector.h"

GuiWarpFrequencySelector::GuiWarpFrequencySelector(GuiContainer* owner, string id)
: GuiSelector(owner, id, [](int index, string value) { if (my_spaceship) my_spaceship->commandSetWarpFrequency(index); })
{
    for(int n=0; n<10; n++)
    {
        if (gameGlobalInfo->layer[n].defined)
            addEntry(gameGlobalInfo->layer[n].title, gameGlobalInfo->layer[n].title);
    }
    if (my_spaceship)
        setSelectionIndex(my_spaceship->warp_frequency);
    if (!gameGlobalInfo->use_warp_terrain)
        hide();
}

void GuiWarpFrequencySelector::onHotkey(const HotkeyResult& key)
{
    if (key.category == "HELMS" && my_spaceship && gameGlobalInfo->use_warp_terrain)
    {
        if (key.hotkey == "WARP_FREQUENCY_INCREASE")
        {
            if (getSelectionIndex() >= (int)entries.size() - 1)
                setSelectionIndex(0);
            else
                setSelectionIndex(getSelectionIndex() + 1);
            callback();
        }
        if (key.hotkey == "WARP_FREQUENCY_DECREASE")
        {
            if (getSelectionIndex() <= 0)
                setSelectionIndex(entries.size() - 1);
            else
                setSelectionIndex(getSelectionIndex() - 1);
        }
    }
}
