

tRoomPatterns = {}
tRooms ={}


pParticlesHUB = {} 

vColorDeep =  Vector(0,0.8,0.8,1);
vColorCeiling =  Vector(0,0,0,1);

--tLightDir = new Transform();
--tLightDir:rotation(54,0,35);


tGround = Mesh.createBox( 400, 1, 400, Vector(0,0,0,1))
tGround:shader():matEmissive(vColorDeep)
tGround:position(0, -30 , 0)

function LoadRoomPak(sName)
print("Loading : "..sName)
	merge("Data/Maps/"..sName..".pak")
	tRoomPatterns[#tRoomPatterns+1] =  Transform.find("Scene")
	tRoomPatterns[#tRoomPatterns]:visible(false)

--	mapShader = ShaderFx.find("WallCGFX")
--	mapShader:setParam("beneathColor", vColorDeep )
--	mapShader:setParam("ceilingColor", vColorCeiling )
	--mapShader:bindDirection("lightDr", tLightDir)
end

function CreateRoom(id, posX, posY, posZ)
	tRooms[#tRooms + 1] = tRoomPatterns[id+1]:clone()
		tRooms[#tRooms]:position(posX, posY ,-posZ)
		tRooms[#tRooms]:visible(true)		
	--	print( "Creating room " .. id .. " at " .. posX .. "-" .. posY);
end

function ShowRoom(id, bShow)

	id = id +1

	print ("Luashowroom "..id .."  to ");

	if(tRooms[id] == nil) then print("Error the room ".. id.. "doesn't exist")
	else	tRooms[id]:visible(bShow)
	end

end

function CleanRooms()


end

function PrepareMap()

	local shaders = ShaderFx.instances()


	for i = 1 , #shaders do 

	shader = shaders[i]

	
		if(shader:name() == "WallBottomCGFX" or shader:name() == "PillarCGFX") then
	
			
			--shader.beneathColor = vColorDeep
			--shader.beneathLimit= -10.645  
			--shader.beneathStrength = 0.142  
			--shader.beneathPower = 0.0  

	--		shader:setParam("beneathColor", vColorDeep)
	--		shader:setParam("beneathLimit", -10.645 )
	--		shader:setParam("beneathStrength", 0.142  )
		


		
			--shader.beneathLimit=  -10.645 
			--shader.beneathStrength = 0.142  
			--shader.beneathPower = 0.0  
		elseif (shader:name() == "WallTopCGFX"  or  shader:name() == "EntranceTopCGFX" or shader:name() == "PillarCGFX") then

	--		shader:setParam("ceilingLimit", 4)
	--		shader:setParam("ceilingStrength", 0.052  )		

		elseif (shader:name() == "GroundCGFX")  then
			--shader:setParam("lightDir", Vector.normal( Vector(0,-0.5,1,0) ))
		end

	end
end


function AddFxTrigger ( posX, posY, height )
particleCircle:position( 0,0,0);

	pParticlesHUB[#pParticlesHUB + 1 ] = particleCircle:clone();
	pParticlesHUB[#pParticlesHUB]:position( posX, height, -posY);
	pParticlesHUB[#pParticlesHUB]:play();
end 

function StopFxTrigger()

for i = 1, #pParticlesHUB do
	pParticlesHUB[i]:release();
end
pParticlesHUB = nil
pParticlesHUB = {} 
end
function AddFxHome(id,posX, posY)
--[[particleCircle:position( 0,0,0);
	
	particleCircle1 = particleCircle:clone();
	particleCircle2 = particleCircle:clone();
	particleCircle3 = particleCircle:clone();

	Teleporter1 = Mesh.find("pCylinder1");
	Teleporter2 = Mesh.find("pCylinder2");
	Teleporter3 = Mesh.find("pCylinder3");

	particleCircle1:parent(Teleporter1);
	particleCircle2:parent(Teleporter2);
	particleCircle3:parent(Teleporter3);

	particleCircle1:play();
	particleCircle2:play();
	particleCircle3:play();]]--

end