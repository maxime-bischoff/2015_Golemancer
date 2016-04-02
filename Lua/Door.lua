
merge("Data/Props/Door.pak")
doorBase = Transform.find("Door")
locker_shader =  ShaderFx.find("Door_LockerCGFX")
door_shader =  	 ShaderFx.find("DoorCGFX")

doorBase:visible(false)
print("doorbase = ")
print(doorBase)

local lockedColor = Vector(1,0,0,1)
local unlockedColor = Vector(0,1,1,1)

tDoors = {}


function CreateDoor(posX, posY, posZ, angle)

print("Creating door : ".. posX .. " - "..posZ)

tDoors[#tDoors+1] = {}
tDoors[#tDoors].t =  doorBase:clone( "Door".. #tDoors )

tDoors[#tDoors].t:position(posX, posY, -posZ  )
tDoors[#tDoors].t:rotation(0,angle,0)
tDoors[#tDoors].t:visible(true)
tDoors[#tDoors].locker =  Mesh.find("locker_sphere")
--tDoors[#tDoors].locker:shader(locker_shader:clone("LockerShaderClone".. #tDoors) );
tDoors[#tDoors].locker:shadingGroup(-1)



tDoors[#tDoors].doorShader = door_shader:clone("DoorShaderClone".. #tDoors) 
tDoors[#tDoors].topMiddle =Mesh.find("Top_Middle")
tDoors[#tDoors].topMiddle:shader( tDoors[#tDoors].doorShader )
tDoors[#tDoors].topMiddle:shadingGroup(-1)

tDoors[#tDoors].topLeft =Mesh.find("Top_Left")
tDoors[#tDoors].topLeft:shader( tDoors[#tDoors].doorShader )
tDoors[#tDoors].topLeft:shadingGroup(-1)

tDoors[#tDoors].topRight =Mesh.find("Top_Right")
tDoors[#tDoors].topRight:shader( tDoors[#tDoors].doorShader )
tDoors[#tDoors].topRight:shadingGroup(-1)

tDoors[#tDoors].ornamentLeft =Mesh.find("Ornament_Left")
tDoors[#tDoors].ornamentLeft1 =Mesh.find("Ornament_Left1")
tDoors[#tDoors].plate =Mesh.find("Plate")




--[[ 
Mesh.find("Top_Right"):shader(tDoors[#tDoors].doorShader)
Mesh.find("Top_Left"):shader(tDoors[#tDoors].doorShader)
Mesh.find("Bottom_Right"):shader(tDoors[#tDoors].doorShader)
Mesh.find("Bottom_Left"):shader(tDoors[#tDoors].doorShader)
Mesh.find("Top_Middle"):shader(tDoors[#tDoors].doorShader)

Mesh.find("Top_Right"):shadingGroup(-1)
Mesh.find("Top_Left"):shadingGroup(-1)
Mesh.find("Bottom_Right"):shadingGroup(-1)
Mesh.find("Bottom_Left"):shadingGroup(-1)
Mesh.find("Top_Middle"):shadingGroup(-1)
]]


--	print( "Creating door " .. #tDoors .. " at " .. posX .. "  -  " .. posY .. "  -  " .. posZ);

end

function OpenDoor(index)


	index = index +1


	if(index <= #tDoors) then
	tDoors[index].t:animPlay("Door_Open")
end


 --doorBase:animPlay("Door_Open")
 --	meshTemp =  Mesh.createBox( 1, 1, 1, Vector(1,0,0,1))
 --	meshTemp:position(tDoors[index]:position())

end

function UnlockDoor(index)
	index = index + 1
	if(index <= #tDoors) then
     tDoors[index].locker:shader():setParam("glowColor", Vector(0,1,1) )
 end

end

function LockDoor(index)
	index = index + 1
	if(index <= #tDoors) then
    	 tDoors[index].locker:shader():setParam("glowColor", Vector(1,0,0) )
	 end

end

function SealDoor(index)
index = index + 1
	if(index <= #tDoors) then
     tDoors[index].locker:shader():setParam("glowColor", Vector(0.5,0.5,0.5) )
     tDoors[index].locker:shader():setParam("fresnelColor", Vector(0,0,0) )
     tDoors[index].locker:shader():setParam("glowStrength", 0 )

    tDoors[index].topMiddle:shader():setParam("emissiveStrenght", 0 )
    tDoors[index].topLeft:shader():setParam("emissiveStrenght", 0 )
    tDoors[index].topRight:shader():setParam("emissiveStrenght", 0 )


tDoors[index].ornamentLeft:visible(false)
tDoors[index].ornamentLeft1:visible(false)
tDoors[index].plate:visible(false)


  --  tDoors[index].topMiddle:shader():setParam("emissiveStrenght", 1 )
     --tDoors[index].doorShader:setParam("emissiveStrenght", 0 )

   --  tDoors[index].doorShader:setParam("emissiveStrenght", 0 )
     --  print (tDoors[index].doorShader)
 	end

 	--door_shader:setParam("emissiveStrenght", 0 )
end

function CloseDoor(index)
	
	index = index +1
	

	--doorBase:animPlay("Door_Close")
		tDoors[index].t:animPlay("Door_Close")
end
