-- Name: Tweaking tweaking
-- Description: I like to tweak it tweak it, I like to tweak it tweak it, I like to tweak it tweak it, you like to - TWEAK IT 
-- Type: Basic

require("utils.lua")

function init()
    havock = PlayerSpaceship():setFaction("UCN"):setTemplate("UCS Skirmish Class Frigate")
        :setPosition(0, 1000)
        :setCallSign("havock")

    warspite = CpuShip():setFaction("UCN"):setTemplate("UCS King George Battleship")
        :setPosition(0, 0)
        :setCallSign("ucs warspite")

    warspite:orderAttack(havock)
end

function cleanup()
end

function update(delta)
end
