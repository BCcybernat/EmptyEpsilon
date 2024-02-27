-- Name: Engineering - Dock Manager and Drone Pilot Stations
-- Description: 
--- -------------------
--- [Info]
--- -------------------
--- Overview:
--- Managing your on-board drones and piloting them is one of the most important tasks of the Engineering team
---
--- Drone Manager - Overview: 
--- - The Drone manage screen allows you to launch, resupply, refuel and repair your drones 
--- - Your ship is supplied with six drones.
--- 
--- Mine Layer Drones:
--- - You have access to two Mine Layer Drones. They are equipped with a rear-facing tube and supply of four mines, which you can deploy. 
--- - When used well, they can be a devastating weapon, but be aware they do not have beam weapons equipped on board, and may be unable to defend themselves
---
--- Defence Drones:
--- - You have access to two Defence Drones. They are equipped with one beam weapon, and and one front-facing tubes, as well as a supply of four HVLI rounds.
--- - Defence Drones could offer basic support in combat, as well as act as deterrents in some situations. 
---
--- Scout Drone:
--- - Your ship is equipped with a scout drone. Scout drone does not have any weapons on board, but it is equipped with a reactor, capable of producing energy
--- - In an emergency, stripping the scount drone off energy can provide vital fuel for your ship to keep running.
--- 
--- Drone Pilot:
--- - The interface of the drone pilot closely resembles one of your own ship, and should be quite intuitive if you already know how to helm your vessel and have knowledge of the weapon stations. 
--- - Keep an eye on Connection information displayed at the bottom of the screen. If your connection drops to below 30%, consider getting closer to the main vessel. 
-- Type: Basic

require("utils.lua")

function init()
    player = PlayerSpaceship():setFaction("UCN"):setTemplate("UCS Hoplite Class Destroyer")
    tutorial:setPlayerShip(player)

    tutorial:showMessage([[Welcome to the bridge tutorial.
    Note that this tutorial is designed to give you a quick overview of the basic operations, but does not cover every single aspect.
    
    Press "Next" to continue]], true)
    tutorial_list = {
        dockTutorial,
        droneTutorial,
        endOfTutorial
    }
    tutorial:onNext(function()
        tutorial_list_index = 1
        startSequence(tutorial_list[tutorial_list_index])
    end)
end

function startSequence(sequence)
    current_sequence = sequence
    current_index = 1
    runNextSequenceStep()
end

function runNextSequenceStep()
    local data = current_sequence[current_index]
    current_index = current_index + 1
    if data == nil then
        tutorial_list_index = tutorial_list_index + 1
        if tutorial_list[tutorial_list_index] ~= nil then
            startSequence(tutorial_list[tutorial_list_index])
        else
            tutorial:finish()
        end
    elseif data["message"] ~= nil then
        tutorial:showMessage(data["message"], data["finish_check_function"] == nil)
        if data["finish_check_function"] == nil then
            update = nil
            tutorial:onNext(runNextSequenceStep)
        else
            update = function(delta)
                if data["finish_check_function"]() then
                    runNextSequenceStep()
                end
            end
            tutorial:onNext(nil)
        end
    elseif data["run_function"] ~= nil then
        local has_next_step = current_index <= #current_sequence
        data["run_function"]()
        if has_next_step then
            runNextSequenceStep()
        end
    end
end

function createSequence()
    return {}
end

function addToSequence(sequence, data, data2)
    if type(data) == "string" then
        if data2 == nil then
            table.insert(sequence, {message = data})
        else
            table.insert(sequence, {message = data, finish_check_function = data2})
        end
    elseif type(data) == "function" then
        table.insert(sequence, {run_function = data})
    end
end

function resetPlayerShip()
    player:setJumpDrive(false)
    player:setWarpDrive(false)
    player:setImpulseMaxSpeed(1)
    player:setRotationMaxSpeed(1)
    for _, system in ipairs({"reactor", "beamweapons", "missilesystem", "maneuver", "impulse", "warp", "jumpdrive", "frontshield", "rearshield"}) do
        player:setSystemHealth(system, 1.0)
        player:setSystemHeat(system, 0.0)
        player:setSystemPower(system, 1.0)
        player:commandSetSystemPowerRequest(system, 1.0)
        player:setSystemCoolant(system, 0.0)
        player:commandSetSystemCoolantRequest(system, 0.0)
    end
    player:setPosition(0, 0)
    player:setRotation(0)
    player:commandImpulse(0)
    player:commandWarp(0)
    player:commandTargetRotation(0)
    player:commandSetShields(false)
end

-- end of utility functions

dockTutorial = createSequence()
    addToSequence(dockTutorial, function()
        tutorial:switchViewToScreen(11)
        tutorial:setMessageToBottomPosition()
        resetPlayerShip()
    end)
    
addToSequence(dockTutorial, [[Welcome to the Dock Manager screen. Here, you will be able to manage and launch your drones]])
addToSequence(dockTutorial, [[Drones are distruted across your Docker compartments randomly. You will be able to see whether each compartment is (Docked) or (Empty) on the right side of the screen.]])
addToSequence(dockTutorial, [[Click through the tabs for each compartment to familiarise yourself with them, then click "next".]])
addToSequence(dockTutorial, [[Is there a drone present in the Launcher compartment? If not, locate a closest drone, and at the top of the screen select Deliver:"Launcher-1" or "Launcher-2", then click "Deliver"]])
addToSequence(dockTutorial, [[Note that it takes some time to move the drones around.]])
addToSequence(dockTutorial, [[Now it's time to launch your drone. Click "Launch" to do so.]],
    function()
        for _, obj in ipairs(player:getObjectsInRange(300)) do
            if obj:getTypeName() == "Drone" then
                return true
            end
        end
    end)

droneTutorial = createSequence(
    addToSequence(droneTutorial, function()
    tutorial:switchViewToScreen(12)
    tutorial:setMessageToBottomPosition()
    end)
)
addToSequence(droneTutorial, [[This is your Drone Pilot screen. At the moment it's empty. Click your drone details at the top of the screen to connect to the drone.]])


endOfTutorial = createSequence()
addToSequence(endOfTutorial, function() tutorial:switchViewToMainScreen() end)
addToSequence(endOfTutorial, _([[This concludes the Relay and Navigation screens tutorial, there is plenty more for you to learn on the job about your ship's Operations.
Please don't press anything else on your screen, and let the officer taking you through the tutorial know that you have finished training.]]))