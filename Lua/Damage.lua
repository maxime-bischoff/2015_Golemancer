tDamage = {}
merge("Data/SFX/Impact_Heavy.pak")
merge("Data/SFX/ParticlePlane.pak")
merge ( "Data/SFX/Laser_Charge.pak")
merge("Data/SFX/Laser_Charge.anm")
merge("Data/Particles/HoloExplosion.pak")
tHeavyImpact = Transform.find("Impact_Heavy")
tHeavyImpact:scale(4,4,4)
tHeavyImpact:visible(false)

tHeavyImpacts = {}


tEnergyBall = Mesh.find("ParticlePlane")
tEnergyBall:visible(false)
tEnergyBall:shader():flags({ depthWrite = false },0)

Shader_Laser_Damage = ShaderFx.find("Shader_Laser")

LaserParent = Transform.find("LaserParent")
LaserMesh = Mesh.find( "LaserMesh")
LaserMesh:visible(false)
Laser_Charge = AnimData.find ( "Laser_Charge")
Laser_Charge:speed( 2.5 )

ExplosionSphere = Mesh.find("ExplosionSphere")
ExplosionSphere:shaderPriority( 300 )
ExplosionSphere:visible( false )

--[[
shader1 = Shader.new()
shader1:texture(0,Nuage)
shader1:blendMode("SRCALPHA_BLEND")
]]--
function CreateDamage(Index, PositionX, PositionZ, Height, SizeX, SizeZ, Angle, bActive, iForm, iStyle)
	tDamage[Index]  = {}
	tDamage[Index].transform = Transform.new()
	tDamage[Index].particules = particleSparks:clone();
	tDamage[Index].height = Height
	if iForm == 0 then --(SizeX == SizeZ and bActive) then
		--print(iStyle)
		if iStyle == 0 then
		tDamage[Index].mesh = tEnergyBall:clone()
		tDamage[Index].mesh:parent ( tDamage[Index].transform )
		tDamage[Index].mesh:shader( tEnergyBall:shader():clone() )
		tDamage[Index].transform:position( PositionX , Height + 0.4, -PositionZ)
		--tDamage[Index]:scale(SizeX * 2 , SizeZ * 2, 1)
		tDamage[Index].mesh:visible(true)
		elseif iStyle == 2 then
			--print("Explosiooon")
			--print(Height)
			tDamage[Index].mesh = ExplosionSphere:clone("Explosion "..Index)--Mesh.createSphere( 0.5, Vector( 0 , 1 , 1 , 0.5) )
			tDamage[Index].mesh:visible( true )
			tDamage[Index].mesh:position ( 0,0,0 )
			tDamage[Index].mesh:parent ( tDamage[Index].transform )
			tDamage[Index].transform:position( PositionX , Height, -PositionZ)
		elseif iStyle == 3 then
			--print("Explosiooon")
			--print(Height)
			tDamage[Index].mesh = Mesh.createBox( 1, 0.3, 1, Vector(1,0,0,0.5))
			tDamage[Index].mesh:visible( false )
		else
			tDamage[Index].mesh = Mesh.createBox( 1, 0.3, 1, Vector(1,0,0,0.5))
			tDamage[Index].mesh:parent ( tDamage[Index].transform )
			tDamage[Index].transform:scale(SizeX * 2 , SizeZ * 2, 1)

		end

	else
		tDamage[Index].mesh = LaserMesh:clone()
		tDamage[Index].mesh:visible(true)
		tDamage[Index].mesh:parent (tDamage[Index].transform )
		tDamage[Index].transform:animPlay(Laser_Charge,true)

		tDamage[Index].mesh:shader(Shader_Laser_Damage:clone())
		tDamage[Index].transform:position( PositionX , Height + 0.1, -PositionZ)

		tDamage[Index].particules:position( 0,0,0,0)
		tDamage[Index].particules:parent ( tDamage[Index].transform )
		tDamage[Index].particules:play()

		if bActive ~= true then
			tDamage[Index].transform:position( PositionX ,Height + 0.1, -PositionZ)
		end

		tDamage[Index].transform:rotation( 0,  math.deg( Angle ), 0 )
		tDamage[Index].transform:scale(0.3,0.3, 1)
	end


end


function DestroyDamage( Index , Count )
	tDamage[Index].mesh:release()
	tDamage[Index].particules:release();
	tDamage[Index] = tDamage[ Count ]
	tDamage[Count] = nil
end

function UpdateDamage( Index, PositionX, PositionZ, SizeX, SizeZ, Angle, bActive, iStyle )
	tDamage[Index ].transform:rotation( 0,  math.deg( Angle ), 0 )


if bActive == true then
		tDamage[Index].transform:position( PositionX , tDamage[Index].height + 0.95, -PositionZ)
	end
	if iStyle == 0 then
		tDamage[Index].transform:scale(SizeX * 2, SizeZ * 2, 1 )
	elseif iStyle == 2 then
		--print(	tDamage[Index].transform:position( ))
				tDamage[Index].transform:scale(SizeX*2, SizeZ*2 , SizeZ*2 )
	else
		tDamage[Index].transform:scale(0.3,0.3 , SizeZ*2 )
		tDamage[Index].transform:position( PositionX , tDamage[Index].height + 0.90, -PositionZ)
	end



end


function CreateHeavyImpact(posX, posZ  )
	tHeavyImpacts[ #tHeavyImpacts +1] = tHeavyImpact:clone()
	tHeavyImpacts[ #tHeavyImpacts]:position(posX, tPlayer:position().y, -posZ )
	tHeavyImpacts[ #tHeavyImpacts]:animPlay("Impact_Anim")
	tHeavyImpacts[ #tHeavyImpacts]:visible(true)
end

function UpdateHeavyImpacts()
	i = 1
	while i <= #tHeavyImpacts  do
		if ( not tHeavyImpacts[i]:animPlaying())	 then
			tHeavyImpacts[i]:release()
			tHeavyImpacts[i] = tHeavyImpacts[#tHeavyImpacts]
			tHeavyImpacts[#tHeavyImpacts] = nil
		end
		i = i + 1
	end
end
