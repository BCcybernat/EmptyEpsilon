#include "scanTargetButton.h"
#include "playerInfo.h"
#include "targetsContainer.h"
#include "spaceObjects/playerSpaceship.h"
#include "powerDamageIndicator.h"
#include "gui/gui2_button.h"
#include "gui/gui2_progressbar.h"

GuiScanTargetButton::GuiScanTargetButton(GuiContainer* owner, string id, TargetsContainer* targets)
: GuiElement(owner, id), targets(targets)
{
    button = new GuiButton(this, id + "_BUTTON", tr("Scan"), [this]() {
        if (my_spaceship && this->targets && this->targets->get())
            my_spaceship->commandScan(this->targets->get());
    });
    button->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    progress = new GuiProgressbar(this, id + "_PROGRESS", 0, PlayerSpaceship::max_scanning_delay, 0.0);
    progress->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    pdi = new GuiPowerDamageIndicator(this, id + "_DPI", SYS_Scanner, ACenterLeft, my_spaceship);
    pdi->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
}

void GuiScanTargetButton::onUpdate()
{
    setVisible(my_spaceship && my_spaceship->getCanScan());
}

void GuiScanTargetButton::onDraw(sf::RenderTarget& window)
{
    if (!my_spaceship)
        return;

    if (my_spaceship->scanning_delay > 0.0)
    {
        progress->show();
        progress->setValue(my_spaceship->scanning_delay);
        button->hide();
    }
    else
    {
        P<SpaceObject> obj;
        if (targets)
            obj = targets->get();

        button->show();
        if (obj && obj->canBeScannedBy(my_spaceship) && my_spaceship->getSystemEffectiveness(SYS_Scanner) > 0.1)
            button->enable();
        else
            button->disable();
        progress->hide();
    }
}
