--- Basic station comms.
--
-- Station comms that allows buying ordnance, supply drop, and reinforcements.
-- Default script for any `SpaceStation`.
--
-- @script comms_station

-- uses `mergeTables`
require("utils.lua")

--- Main menu of communication.
function mainMenu()
    if comms_target.comms_data == nil then
        comms_target.comms_data = {}
    end

    -- comms_data is used globally
    comms_data = comms_target.comms_data
    if "UCN" == comms_target:getFaction() then
        UCNComms(comms_data)
    elseif "corsair" == comms_target:getFaction() then
        enemyComms(comms_data)
    elseif player:isEnemy (comms_target) and comms_target:isFriendOrFoeIdentifiedBy(player) then
        enemyComms(comms_data)
    else
        neutralComms(comms_data)
    end
    return true
end


--- UCN comms
function UCNComms(comms_data)
    callsignprint = comms_target:getCallSign()
    addGMMessage("Player opened comms with "..callsignprint)
    setCommsMessage("Comms link established with "..callsignprint.."\n\nHello officer, what do you require?")
        if player:isDocked(comms_target) then
            addCommsReply("Request supplies", function()
                setCommsMessage("Please specify required supplies")
                    addCommsReply("Homing missiles restock required", function()
                    if player:getWeaponStorage("Homing") >= player:getWeaponStorageMax("Homing") then
                        setCommsMessage("Docking readouts show your ships is fully stocked re: homing missiles.")
                        addCommsReply("Back", mainMenu)
                    else
                        player:setWeaponStorage("Homing", player:getWeaponStorageMax("Homing"))
                        setCommsMessage("Homing missiles restocked to maximum.")
                        addCommsReply("Back", mainMenu)
                    end
                end)
                addCommsReply("Mines restock required.)", function()
                    if player:getWeaponStorage("Mine") >= player:getWeaponStorageMax("Mine") then
                        setCommsMessage("Docking readouts show your ship is fully stocked re: mines.")
                        addCommsReply("Back", mainMenu)
                    else
                    player:setWeaponStorage("Mine", player:getWeaponStorageMax("Mine"))
                        setCommsMessage("Mines restocked to maximum.")
                        addCommsReply("Back", mainMenu)
                    end
                end)
                addCommsReply("Can you supply us with some nukes.", function()
                    if player:getWeaponStorage("Nuke") >= player:getWeaponStorageMax("Nuke") then
                        setCommsMessage("Docking readouts show your ship already has the regulated number of nukes on board.")
                        addCommsReply("Back", mainMenu)
                    else
                        player:setWeaponStorage("Nuke", player:getWeaponStorageMax("Nuke"))
                        setCommsMessage("Nukes restocked to maxmimum regulated number. Use with care.")
                        addCommsReply("Back", mainMenu)
                    end
                end)
                addCommsReply("EMP Missiles", function()
                    if player:getWeaponStorage("EMP") >= player:getWeaponStorageMax("EMP") then
                        setCommsMessage("Docking readouts show your ship is fully stocked re: EMP Missiles.")
                        addCommsReply("Back", mainMenu)
                    else
                        player:setWeaponStorage("EMP", player:getWeaponStorageMax("EMP"))
                        setCommsMessage("EMP Missiles restocked to maxmimum.")
                        addCommsReply("Back", mainMenu)
                    end
                end)
        end)
        addCommsReply("Establish direct link with Warspite crew", function() commsSwitchToGM() end)
    else
        addCommsReply("Request supply drop", function()
            if player:getWaypointCount(0) < 1 then
                setCommsMessage("Your vessel must place a waypoint to guide drones before a supply drop can be ordered.")
            else
                setCommsMessage("Please select the waypoint where supplies should be directed.")
                for n = 1, player:getWaypointCount(0) do
                    addCommsReply("Waypoint" .. n, function()
                        local position_x, position_y = comms_target:getPosition()
                        local target_x, target_y = player:getWaypoint(n, 0)
                        local script = Script()
                        script:setVariable("position_x", position_x):setVariable("position_y", position_y)
                        script:setVariable("target_x", target_x):setVariable("target_y", target_y)
                        script:setVariable("pCallsign", player:getCallSign())
                        script:setVariable("faction_id", comms_target:getFactionId()):run("supply_drop_new.lua")
                        print("running script")
                        setCommsMessage("A supply drone has been dispatched towards waypoint" .. n)
                        addCommsReply("Back", mainMenu)
                    end)
                end
            end
            addCommsReply("Back", mainMenu)
        end)
        addCommsReply("Establish direct link with "..callsignprint.." crew", function() commsSwitchToGM() end)
    end
    return true
end

--- Handle enemy communication.
function enemyComms(comms_data)
    callsignprint = comms_target:getCallSign()
    addGMMessage("Player opened comms with "..callsignprint)
    setCommsMessage("Basic comms link established.\n\nThis station (callsign "..callsignprint..") is formally listed as an enemy of the UCN. Do you wish to continue in establishing direct comms with the crew?")
               addCommsReply("Establish direct link with station crew", function()
            commsSwitchToGM()
            end)
end

--- Handle neutral communication.
function neutralComms(comms_data)
    callsignprint = comms_target:getCallSign()
    addGMMessage("Player opened comms with "..callsignprint)
    setCommsMessage("Comms link established with "..callsignprint.."\n\nThis is not a UCN station; appropriate comms protocols should be followed.")
        addCommsReply("Establish direct link with station crew", function()
            commsSwitchToGM()
            end)
end

mainMenu()