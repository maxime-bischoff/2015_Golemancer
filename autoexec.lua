RES_DIR=os.chdir(SCRIPT_FILENAME)
os.setGameDir(os.getScriptDir())

ShaderFx.setShaderSrcPathList("$(CurDir)/Data/Shaders/$(Name);$(Input);$(GameDir)/Data/Shaders/$(Name);$(PakDir)\\Shaders\\$(Name);");
ShaderFx.setShaderObjPathList("$(CurDir)\\Data\\Shaders\\$(Platform)\\$(Name);$(GameDir)\\Shaders\\$(Platform)\\$(Name);$(PakDir)\\Shaders\\$(Platform)\\$(Name);");

if TARGET=="PC" then
	Display.mode(1280,720)
	--Display.mode(1)

else
	Display.mode(1)
end
--Display:stats(0)
--Display:wireframe(true)
globals = Globals.first();

--globals.mGamma = 20


merge("Data/Fonts/agency.pak")
print("||-------INITIALIZE START-------||")

Input:padVibrate(false)
--Input.enableHandler(true)
function Lerp( From , To, Weight)
	 return From + (To - From) * Weight
end

dofile("Lua/Splashscreens.lua")
dofile("Lua/Sound.lua")
dofile("Lua/Particles.lua")
dofile("Lua/Menu.lua")
dofile("Lua/Player.lua")
dofile("Lua/Camera.lua")
dofile("Lua/Wall.lua")
dofile("Lua/Trigger.lua")
dofile("Lua/Enemy.lua")
dofile("Lua/Boss.lua")
dofile("Lua/Damage.lua")
dofile("Lua/Focus.lua")
dofile("Lua/FX.lua")
dofile("Lua/Tuto.lua")
dofile("Lua/Door.lua")
--dofile("Lua/Destructible.lua")
dofile("Lua/Environment.lua")
dofile("Lua/Collectible.lua")
dofile("Lua/Hud.lua")
dofile("Lua/Map.lua")


print("||-------INITIALIZE  END-------||")

function CleanScene()

	count = #tSolid

	if count > 0 then
		for i=1, count do


	    tSolid[i]:release()
	    tSolid[i] = nil
		end
	end


	iRoom = #tRooms
	while iRoom > 0 do
		tRooms[iRoom]:release()
		tRooms[iRoom] = nil
		iRoom = iRoom - 1
	end

	iDoor = #tDoors
	while iDoor > 0 do
		tDoors[iDoor].t:release()	
		tDoors[iDoor] = nil
		iDoor = iDoor - 1
	end

		iEnemy = #tEnemy
	while iEnemy > 0 do


		print("Clean enemy ".. iEnemy)
		tEnemy[iEnemy].skinMesh:release()
		tEnemy[iEnemy].transform:release()
		tEnemy[iEnemy] = nil
		iEnemy = iEnemy - 1
	end



		tEnemy = nil
		tEnemy = {}

		ClearFXs()

end

function GetTriggerRight()
		--print(Input:joypad(132,132)  )
		--print (Input:padKeyUsed 	( 0 	 ))
		--print (Input:padState(132, 0))
		value = Input:padState("AXE2-", 0)
		--print(value)
		--Input:padVibrate 	( 	0,	10,5 )
		return value



end

function GetTriggerLeftState()
		--print(Input:joypad(132,132)  )
		--print (Input:padKeyUsed 	( 0 	 ))
--[[
		if( Input:padKeyUsed(0) == 133) then
			print(true)
		else
			print(false)
		end]]--
		--print (Input:padState(132, 0))


		return Input:joypad(133,133)



end

local gammaSpeed = 1

function IncreaseGamma()
	globals.mGamma = math.min( 3, globals.mGamma + gammaSpeed * Clock:deltaTime() )


end

function DecreaseGamma()
	globals.mGamma = math.max( -3 , globals.mGamma - gammaSpeed * Clock:deltaTime())
end

---/// MAIN THREAD ///---
function main()
	while true do
		--camTransform:position(tPlayer:position())
		--cam:position( tPlayer:position().x ,2,tPlayer:position().z+2 )
		--cam:lookat(tPlayer:position())
		PlayerBlenderUpdate()
		yield()

		value = Input:padState("AXE2-", 0)
		if value >0.1 then
		--Input:padVibrate 	( 	0,	10,5 )
		end
	end
end


Thread.start(main)
