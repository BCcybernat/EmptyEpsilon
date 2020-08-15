#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "systemScreen.h"

#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"

#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_togglebutton.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_progressslider.h"
#include "gui/gui2_arrow.h"
#include "gui/gui2_image.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_element.h"

SystemScreen::SystemScreen(GuiContainer* owner)
: GuiOverlay(owner, "SYSTEM_SCREEN", colorConfig.background), selected_system(SYS_Reactor)
{
    // Render the background decorations.
    background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));
    
    system_selector = new GuiSelector(this, "SYSTEM_SELECTOR", [this](int index, string value)
    {
        //selectSystem(ESystem(index + SYS_None));
        selectSystem(ESystem(index));
    });
    for(int n=0; n<SYS_COUNT; n++)
        system_selector->addEntry(getLocaleSystemName(ESystem(n)), string(n));
    system_selector->setSelectionIndex(0);
    system_selector->setPosition(20, 20, ATopLeft)->setSize(250, 50);
    
    system_title = new GuiLabel(this, "SYSTEM_TITLE", "", 60);
    system_title->setPosition(250, -450, ABottomLeft);
    system_title->setAlignment(ACenter)->setSize(30, 400);
    
    system_info = new GuiAutoLayout(this, "SYSTEM_INFO", GuiAutoLayout::LayoutHorizontalLeftToRight);
    system_info->setPosition(0, 0, ABottomLeft);
    system_info->setSize(500, 500);
    
    damage_title = new GuiLabel(system_info, "DAMAGE_TITLE", tr("slider", "Damage"), 30);
    damage_title->setVertical()->setAlignment(ACenterLeft)->setSize(30, 400);
    damage_bar = new GuiProgressbar(system_info, "SYSTEM_DAMAGE", 0.0, 1.0, 0.0);
    damage_bar->setSize(80, GuiElement::GuiSizeMax);
    damage_icon = new GuiImage(damage_bar, "", "gui/icons/system_health");
    damage_icon->setColor(colorConfig.log_generic)->setPosition(0, 50, ACenter)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMatchWidth);
    damage_label = new GuiLabel(damage_bar, "SYSTEM_DAMAGE_LABEL", "...", 30);
    damage_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    heat_title = new GuiLabel(system_info, "HEAT_TITLE", tr("slider", "Heat"), 30);
    heat_title->setVertical()->setAlignment(ACenterLeft)->setSize(30, 400);
    heat_bar = new GuiProgressbar(system_info, "SYSTEM_HEAT", 0.0, 1.0, 0.0);
    heat_bar->setSize(80, GuiElement::GuiSizeMax);
    heat_icon = new GuiImage(heat_bar, "", "gui/icons/status_overheat");
    heat_icon->setColor(colorConfig.log_generic)->setPosition(0, 50, ACenter)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMatchWidth);
    heat_label = new GuiLabel(heat_bar, "SYSTEM_HEAT_LABEL", "...", 30);
    heat_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    coolant_title = new GuiLabel(system_info, "COOLANT_TITLE", tr("slider", "Coolant"), 30);
    coolant_title->setVertical()->setAlignment(ACenterLeft)->setSize(30, 400);
    coolant_bar = new GuiProgressbar(system_info, "SYSTEM_COOLANT", 0.0, 10.0, 0.0);
    coolant_bar->setSize(80, GuiElement::GuiSizeMax);
    coolant_icon = new GuiImage(coolant_bar, "", "gui/icons/coolant");
    coolant_icon->setColor(colorConfig.log_generic)->setPosition(0, 50, ACenter)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMatchWidth);
    coolant_label = new GuiLabel(coolant_bar, "SYSTEM_COOLANT_LABEL", "...", 30);
    coolant_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    power_title = new GuiLabel(system_info, "POWER_TITLE", tr("slider", "Power"), 30);
    power_title->setVertical()->setAlignment(ACenterLeft)->setSize(30, 400);
    power_bar = new GuiProgressbar(system_info, id + "SYSTEM_POWER", 0.0, 3.0, 1.0);
    power_bar->setSize(80, GuiElement::GuiSizeMax);
    power_icon = new GuiImage(power_bar, "", "gui/icons/energy");
    power_icon->setColor(colorConfig.log_generic)->setPosition(0, 50, ACenter)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMatchWidth);
    power_label = new GuiLabel(power_bar, "SYSTEM_POWER_LABEL", "...", 30);
    power_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    effectiveness_title = new GuiLabel(system_info, "EFFECTIVENESS_TITLE", tr("slider", "Effectiveness"), 30);
    effectiveness_title->setVertical()->setAlignment(ACenterLeft)->setSize(30, 400);
    effectiveness_bar = new GuiProgressbar(system_info, "SYSTEM_EFFECTIVENESS", 0.0, 3.0, 0.0);
    effectiveness_bar->setSize(80, GuiElement::GuiSizeMax);
    effectiveness_icon = new GuiImage(effectiveness_bar, "", "gui/icons/system_reactor");
    effectiveness_icon->setColor(colorConfig.log_generic)->setPosition(0, 50, ACenter)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMatchWidth);
    effectiveness_label = new GuiLabel(effectiveness_bar, "SYSTEM_EFFECTIVENESS_LABEL", "...", 30);
    effectiveness_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
    // Instability Info and Slider
    instability_container = new GuiElement(this, "");
    instability_container->setPosition(-50, 0, ACenterRight)->setSize(600, 600);
    
    instability_info = new GuiAutoLayout(instability_container, "SYSTEM_INFO", GuiAutoLayout::LayoutHorizontalRows);
    instability_info->setSize(350, 250);
    instability_info->setPosition(0, 0, ACenter);
    
    instability_label = new GuiLabel(instability_info, "INSTABILITY_LABEL", "Instability Level : ", 30);
    
    instability_x_label = new GuiLabel(instability_info, "INSTABILITY_X_LABEL", "Mixed oxide fuel", 30);
    instability_x_slider = new GuiSlider(instability_info, "", -1.0, 1.0, 0.0, [this](float value) {
        if (my_spaceship) my_spaceship->setSystemInstabilityXValue(selected_system, value);
    });
    instability_x_slider->setSize(340, 25);

    instability_y_label = new GuiLabel(instability_info, "INSTABILITY_Y_LABEL", "Depleted uranium", 30);
    instability_y_slider = new GuiSlider(instability_info, "", -1.0, 1.0, 0.0, [this](float value) {
        if (my_spaceship) my_spaceship->systems[selected_system].instability_y_value = value;
    });
    instability_y_slider->setSize(340, 25);
    
    selectSystem(ESystem(0));
}

void SystemScreen::onDraw(sf::RenderTarget& window)
{
    GuiOverlay::onDraw(window);
    
    if (my_spaceship)
    {
        if (selected_system != SYS_None)
        {
            ShipSystem& system = my_spaceship->systems[selected_system];

            float health = system.health;
            if (health < 0.0)
                damage_bar->setValue(-health)->setColor(sf::Color(128, 32, 32, 192));
            else
                damage_bar->setValue(health)->setColor(sf::Color(64, 128 * health, 64 * health, 192));
            damage_label->setText(string(int(health * 100)) + "%");

            float heat = system.heat_level;
            heat_bar->setValue(heat)->setColor(sf::Color(128, 32 + 96 * (1.0 - heat), 32, 192));
            heat_label->setText(string(int(heat * 100)) + "%");

            float power = system.power_level;
            power_bar->setValue(power)->setColor(sf::Color(128, 128, 0, 192));
            power_label->setText(string(int(power * 100)) + "%");
            
            float coolant = system.coolant_level;
            coolant_bar->setValue(coolant)->setColor(sf::Color(0, 128, 128, 192));
            coolant_label->setText(string(int(coolant * 10)) + "%");
            
            float effectiveness = my_spaceship->getSystemEffectiveness(selected_system);
            effectiveness_bar->setValue(effectiveness)->setColor(sf::Color(64, 128 * health, 64 * health, 192));
            effectiveness_label->setText(string(int(effectiveness * 100)) + "%");
            
            instability_x_slider->setValue(system.instability_x_value);
            instability_y_slider->setValue(system.instability_y_value);
            
            instability_label->setText("Instability Level : "+ string(int(system.instability_level * 100)) + "%");
            
            updateInstability(window, instability_container);
        }
    }
}

void SystemScreen::updateInstability(sf::RenderTarget& window, GuiElement* container)
{
    if (!my_spaceship)
        return;

    // Cap the number of signature points, which determines the raw data's
    // resolution.
    sf::FloatRect rect = container->getRect();
    const int point_count = 512;
    float width = rect.width;
    float height = rect.height;
    float left = rect.left;
    float top = rect.top;
    
    float radius = std::min(width, height) / 2.0f;
    
    // Create a vertex array containing each data point.
    sf::VertexArray a_r(sf::LinesStrip, point_count+1);

    // For each data point ...
    for(int n = 0; n < point_count; n++)
    {
        // ... make some noise ...
        float r = random(-1.0, 1.0);
        
        if (my_spaceship)
            r *= my_spaceship->systems[selected_system].instability_level * 30;

        // ... and add vectors for each point.
        a_r[n].position.x = left + width / 2.0f;
        a_r[n].position.y = top + height / 2.0f;
        a_r[n].position += sf::vector2FromAngle(float(n) / float(point_count) * 360.0f ) * (radius * (0.95f - r / 300));
        a_r[n].color = sf::Color(255, 0, 0, 255*(my_spaceship->systems[selected_system].instability_level+0.1));
    }

    // Set a zero value at the "end" of the data point array.
    a_r[point_count] = a_r[0];
    
    // Draw each band as a line.
    window.draw(a_r, sf::BlendAdd);
    //container->drawElements(a_r, sf::BlendAdd);
}

void SystemScreen::onHotkey(const HotkeyResult& key)
{
    if (key.category == "SYSTEM_CONTROL" && my_spaceship)
    {
        if (key.hotkey == "SELECT_REACTOR") selectSystem(SYS_Reactor);
        if (key.hotkey == "SELECT_BEAM_WEAPONS") selectSystem(SYS_BeamWeapons);
        if (key.hotkey == "SELECT_MISSILE_SYSTEM") selectSystem(SYS_MissileSystem);
        if (key.hotkey == "SELECT_MANEUVER") selectSystem(SYS_Maneuver);
        if (key.hotkey == "SELECT_IMPULSE") selectSystem(SYS_Impulse);
        if (key.hotkey == "SELECT_WARP") selectSystem(SYS_Warp);
        if (key.hotkey == "SELECT_JUMP_DRIVE") selectSystem(SYS_JumpDrive);
        if (key.hotkey == "SELECT_FRONT_SHIELDS") selectSystem(SYS_FrontShield);
        if (key.hotkey == "SELECT_REAR_SHIELDS") selectSystem(SYS_RearShield);
    }
}

void SystemScreen::selectSystem(ESystem system)
{
    if (my_spaceship && !my_spaceship->hasSystem(system))
        return;
    
    selected_system = system;
    
    if (!my_spaceship)
        return;
    
    if (my_spaceship->systems[selected_system].instability_x_label != "")
        instability_x_label->setText(my_spaceship->systems[selected_system].instability_x_label);
    if (my_spaceship->systems[selected_system].instability_y_label != "")
        instability_y_label->setText(my_spaceship->systems[selected_system].instability_y_label);
    system_title->setText(getLocaleSystemName(system));
}

