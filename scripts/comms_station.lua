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
    mergeTables(
        comms_target.comms_data,
        {
            friendlyness = random(0.0, 100.0),
            weapons = {
                Homing = "neutral",
                HVLI = "neutral",
                Mine = "neutral",
                Nuke = "friend",
                EMP = "friend"
            },
            weapon_cost = {
                Homing = 50,
                HVLI = 25,
                Mine = 50,
                Nuke = 150,
                EMP = 50
            },
            services = {
                supplydrop = "friend",
                reinforcements = "friend"
            },
            service_cost = {
                supplydrop = 100,
                reinforcements = 150
            },
            reputation_cost_multipliers = {
                friend = 1.0,
                neutral = 2.5
            },
            max_weapon_refill_amount = {
                friend = 1.0,
                neutral = 0.5
            }
        }
    )

    -- comms_data is used globally
    comms_data = comms_target.comms_data

    if player:isEnemy(comms_target) then
        return false
    end

    if comms_target:areEnemiesInRange(5000) then
        setCommsMessage("We are under attack! No time for chatting!")
        return true
    end
    if not player:isDocked(comms_target) then
        handleUndockedState()
    else
        handleDockedState()
    end
    return true
end

--- Handle communications while docked with this station.
function handleDockedState()
    if player:isFriendly(comms_target) then
        setCommsMessage("Good day, officer! Welcome to " .. comms_target:getCallSign() .. ".\nWhat can we do for you today?")
    else
        setCommsMessage("Welcome to our lovely station " .. comms_target:getCallSign() .. ".")
    end

    if player:getWeaponStorageMax("Homing") > 0 then
        addCommsReply("Do you have spare homing missiles for us? ("..getWeaponCost("Homing").."rep each)", function()
            handleWeaponRestock("Homing")
        end)
    end
    if player:getWeaponStorageMax("HVLI") > 0 then
        addCommsReply("Can you restock us with HVLI? ("..getWeaponCost("HVLI").."rep each)", function()
            handleWeaponRestock("HVLI")
        end)
    end
    if player:getWeaponStorageMax("Mine") > 0 then
        addCommsReply("Please re-stock our mines. ("..getWeaponCost("Mine").."rep each)", function()
            handleWeaponRestock("Mine")
        end)
    end
    if player:getWeaponStorageMax("Nuke") > 0 then
        addCommsReply("Can you supply us with some nukes? ("..getWeaponCost("Nuke").."rep each)", function()
            handleWeaponRestock("Nuke")
        end)
    end
    if player:getWeaponStorageMax("EMP") > 0 then
        addCommsReply("Please re-stock our EMP missiles. ("..getWeaponCost("EMP").."rep each)", function()
            handleWeaponRestock("EMP")
        end)
    end
end

--- handleWeaponRestock
function handleWeaponRestock(weapon)
    if not player:isDocked(comms_target) then
        setCommsMessage("You need to stay docked for that action.")
        return
    end
    if not isAllowedTo(comms_data.weapons[weapon]) then
        if weapon == "Nuke" then
            setCommsMessage("We do not deal in weapons of mass destruction.")
        elseif weapon == "EMP" then
            setCommsMessage("We do not deal in weapons of mass disruption.")
        else
            setCommsMessage("We do not deal in those weapons.")
        end
        return
    end
    local points_per_item = getWeaponCost(weapon)
    local item_amount = math.floor(player:getWeaponStorageMax(weapon) * comms_data.max_weapon_refill_amount[getFriendStatus()]) - player:getWeaponStorage(weapon)
    if item_amount <= 0 then
        if weapon == "Nuke" then
            setCommsMessage("All nukes are charged and primed for destruction.")
        else
            setCommsMessage("Sorry, sir, but you are as fully stocked as I can allow.")
        end
        addCommsReply("Back", mainMenu)
    else
        if not player:takeReputationPoints(points_per_item * item_amount) then
            setCommsMessage("Not enough reputation.")
            return
        end
        player:setWeaponStorage(weapon, player:getWeaponStorage(weapon) + item_amount)
        if player:getWeaponStorage(weapon) == player:getWeaponStorageMax(weapon) then
            setCommsMessage("You are fully loaded and ready to explode things.")
        else
            setCommsMessage("We generously resupplied you with some weapon charges.\nPut them to good use.")
        end
        addCommsReply("Back", mainMenu)
    end
end

--- Handle communications when we are not docked with the station.
function handleUndockedState()
    if player:isFriendly(comms_target) then
        setCommsMessage("This is " .. comms_target:getCallSign() .. ". Good day, officer.\nIf you need supplies, please dock with us first.")
    else
        setCommsMessage("This is " .. comms_target:getCallSign() .. ". Greetings.\nIf you want to do business, please dock with us first.")
    end
    if isAllowedTo(comms_target.comms_data.services.supplydrop) then
        addCommsReply("Can you send a supply drop? ("..getServiceCost("supplydrop").."rep)", function()
            if player:getWaypointCount(0) < 1 then
                setCommsMessage("You need to set a waypoint on route 1 before you can request backup.");
            else
                setCommsMessage("To which waypoint on route 1 should we deliver your supplies?");
                for n=1,player:getWaypointCount(0) do
                    addCommsReply("WP" .. n, function()
                        if player:takeReputationPoints(getServiceCost("supplydrop")) then
                            local position_x, position_y = comms_target:getPosition()
                            local target_x, target_y = player:getWaypoint(n,0)
                            local script = Script()
                            script:setVariable("position_x", position_x):setVariable("position_y", position_y)
                            script:setVariable("target_x", target_x):setVariable("target_y", target_y)
                            script:setVariable("faction_id", comms_target:getFactionId()):run("supply_drop.lua")
                            setCommsMessage("We have dispatched a supply ship toward WP" .. n);
                        else
                            setCommsMessage("Not enough reputation!");
                        end
                        addCommsReply("Back", mainMenu)
                    end)
                end
            end
            addCommsReply("Back", mainMenu)
        end)
    end
    if isAllowedTo(comms_target.comms_data.services.reinforcements) then
        addCommsReply("Please send reinforcements! ("..getServiceCost("reinforcements").."rep)", function()
            if player:getWaypointCount(0) < 1 then
                setCommsMessage("You need to set a waypoint on route 1 before you can request reinforcements.");
            else
                setCommsMessage("To which waypoint on route 1 should we dispatch the reinforcements?");
                for n=1,player:getWaypointCount(0) do
                    addCommsReply("WP" .. n, function()
                        if player:takeReputationPoints(getServiceCost("reinforcements")) then
                            local ship = CpuShip():setFactionId(comms_target:getFactionId()):setPosition(comms_target:getPosition()):setTemplate("UCS Skirmish Frigate"):setCallSign("UCS Havock"):setScanned(true):setWarpDrive(true):setWarpSpeed(1000.00):orderDefendLocation(player:getWaypoint(n,0))
                            setCommsMessage("We have dispatched " .. ship:getCallSign() .. " to assist at WP" .. n);
                        else
                            setCommsMessage("Not enough reputation!");
                        end
                        addCommsReply("Back", mainMenu)
                    end)
                end
            end
            addCommsReply("Back", mainMenu)
        end)
    end
end

--- isAllowedTo
function isAllowedTo(state)
    if state == "friend" and player:isFriendly(comms_target) then
        return true
    end
    if state == "neutral" and not player:isEnemy(comms_target) then
        return true
    end
    return false
end

--- Return the number of reputation points that a specified weapon costs for the
-- current player.
function getWeaponCost(weapon)
    return math.ceil(comms_data.weapon_cost[weapon] * comms_data.reputation_cost_multipliers[getFriendStatus()])
end

--- Return the number of reputation points that a specified service costs for
-- the current player.
function getServiceCost(service)
    return math.ceil(comms_data.service_cost[service])
end

--- Return "friend" or "neutral".
function getFriendStatus()
    if player:isFriendly(comms_target) then
        return "friend"
    else
        return "neutral"
    end
end

mainMenu()
