
-- // :: un seul tableau pour les walls :: // -- 
tWallActive = {} ;
tWallDisactive = {} ; 

tEnvi = {} ; 
tEnviTrigger = {} ;  
tEnviTrap = {} ; 

nTriggerHeight = 0.5 ; 

local TYPETRIGGER = 0 ; 
local TYPETRAP = 2 ; 
local TYPEBLOCK = 1 ; 

BLUE = Vector ( 0,0,1,1 ) ; 
ORANGE = Vector ( 0.7,0.4,0,1 ); 
RED = Vector ( 1,0,0,1 ) ; 

function UpdateEnvi (Type, Index, bActivated )
-- // :: TRIGGER :: // -- 
	if Type == TYPETRIGGER then 
		if bActivated == true and tEnviTrigger[Index].Update == false then 
		-- // :: Show the wall envi :: // -- 
			for i =1 , #tEnviTrigger[Index].tWall do
				tEnviTrigger[Index].tWall[i]:visible ( true ) ; 
			end 
			for i=1, #tEnviTrigger[Index].tWallDisactive do 
				EnviTrigger[Index].tWallDisactive[i]:visible ( false ) ; 
			end
			tEnviTrigger[Index].Update = true ; 
		elseif bActivated == false and tEnviTrigger[Index].Update == true then 
		-- // :: Hide the wall envi :: // -- 
			for i =1 , #tEnviTrigger[Index].tWall do
				tEnviTrigger[Index].tWall[i]:visible ( false ) ; 
			end 
			for i=1, #tEnviTrigger[Index].tWallDisactive do 
				EnviTrigger[Index].tWallDisactive[i]:visible ( true ) ; 
			end
			tEnviTrigger[Index].Update = false ; 
		end

-- // :: TRAP :: // -- 
	elseif Type == TYPETRAP then 
		if bActivated == true and tEnviTrap[Index].Update == false then 
		-- // :: Show the Trap :: // -- 
			for i =1 , #tEnviTrap[Index].tWall do
				tEnviTrap[Index].tWall[i]:visible ( true ) ; 
			end
				tEnviTrap[Index].Update = true ; 
		elseif bActivated == false and tEnviTrap[Index].Update == true then 
		-- // :: Hide the Trap :: // -- 
			for i =1 , #tEnviTrap[Index].tWall do
				tEnviTrap[Index].tWall[i]:visible ( false ) ; 
			end
			tEnviTrap[Index].Update = false ; 
		end 
	end
end 

function CreateEnvi(Type, PositionX, PositionZ, SizeX, SizeZ)
	if Type == TYPETRIGGER then 
		tEnviTrigger[#tEnviTrigger+1] = Mesh.createBox( SizeX, nTriggerHeight, SizeZ, Vector(1,1,1,0.5))
		tEnviTrigger[#tEnviTrigger]:shader(): matEmissive(Vector(0.5,0.2,0,1))
		tEnviTrigger[#tEnviTrigger]:position( PositionX , 0.5, -PositionZ)
		tEnviTrigger[#tEnviTrigger] = { tWall = {}, tWallDisactive = {} , Update = false } ; 
	elseif Type == TYPETRAP then 
		tEnviTrap[#tEnviTrap+1] = Mesh.createBox( SizeX, nTriggerHeight, SizeZ, Vector(1,1,1,0.5))
		tEnviTrap[#tEnviTrap]:shader(): matEmissive(Vector(0,0.9,0.5,1))
		tEnviTrap[#tEnviTrap]:position( PositionX , 0.5, -PositionZ)
		tEnviTrap[#tEnviTrap] = { tWall = {} , Update = false };
	elseif Type == TYPEBLOCK then 
		tEnvi[#tEnvi+1] = Mesh.createBox( SizeX, nTriggerHeight, SizeZ, Vector(1,1,1,0.5))
		tEnvi[#tEnvi]:shader(): matEmissive(Vector(0,0,1,1))
		tEnvi[#tEnvi]:position( PositionX ,0.5, -PositionZ)
	end 
end

function CreateWallActive ( Type, iIndexTarget , PositionX, PositionZ, SizeX, SizeZ )
	if Type == TYPETRIGGER then 
		tEnviTrigger[iIndexTarget].tWall[#tEnviTrigger[iIndexTarget].tWall] = Mesh.createBox ( SizeX, nTriggerHeight, SizeZ, ( Vector ( 1,1,1,0.5))); 
		tEnviTrigger[iIndexTarget].tWall[#tEnviTrigger[iIndexTarget].tWall]:shader(): matEmissive(Vector(0,0.9,0.5,1))
		tEnviTrigger[iIndexTarget].tWall[#tEnviTrigger[iIndexTarget].tWall]:position( PositionX , 0.5, -PositionZ)
	elseif Type == TYPETRAP then 
		tEnviTrigger[iIndexTarget].tWall[#tEnviTrigger[iIndexTarget].tWall] = Mesh.createBox ( SizeX, nTriggerHeight, SizeZ, ( Vector ( 1,1,1,0.5))); 
		tEnviTrigger[iIndexTarget].tWall[#tEnviTrigger[iIndexTarget].tWall]:shader(): matEmissive(Vector(0,0.9,0.5,1))
		tEnviTrigger[iIndexTarget].tWall[#tEnviTrigger[iIndexTarget].tWall]:position( PositionX , 0.5, -PositionZ)
	end 
end 


function CreateWallDisactive ( iIndexTarget , PositionX, PositionZ, SizeX, SizeZ )
	tEnviTrap[iIndexTarget].tWall[#tEnviTrap[iIndexTarget].tWall] = Mesh.createBox ( SizeX, nTriggerHeight, SizeZ, ( Vector ( 1,1,1,0.5))); 
	tEnviTrap[iIndexTarget].tWall[#tEnviTrap[iIndexTarget].tWall]:shader(): matEmissive(Vector(0,0.9,0.5,1))
	tEnviTrap[iIndexTarget].tWall[#tEnviTrap[iIndexTarget].tWall]:position( PositionX , 0.5, -PositionZ)
end 


function DestroyTriggerWall ( iIndex )
	tWallActive[iIndex +1 ]:release () ; 
end 

-- tenvi[1] = {} pour dire que yaura les wallup et walldown et tenvi[1][1] = {} pour que les wallup et wall down soient plusieus 