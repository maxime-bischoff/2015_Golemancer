--zeus:shadingGroup(5)
-- // :: CONSTANTS :: // --
STATE_PURSUIT = 0
STATE_SLACKING = 1

STATE_EYE_CHARGE = 2
STATE_EYE_FIRE = 3
STATE_EYE_RECOVER = 4
STATE_EYE_STUN = 5
STATE_EYE_WALKING = 8

STATE_STRONG_CHARGING = 10
STATE_STRONG_ATTACK = 6
STATE_STRONG_STUN = 12

FAST_AND_RANGED = 0
STRONG_AND_SLOW = 1


merge("Data/Characters/Golem_Eye.pak")
merge("Data/Characters/Golem_Strong_2.pak")

merge("Data/Characters/Golem_Idle.anm")
merge("Data/Characters/Golem_Run.anm")
merge("Data/Characters/Golem_Strong_Walk.anm")
merge("Data/Characters/Golem_Strong_Smash.anm")
merge("Data/Characters/Golem_Strong_Death.anm")
merge("Data/Characters/Golem_Vegetable.anm")
merge("Data/Characters/Golem_Strong_Stun.anm")

merge("Data/Characters/Golem_Eye_Beam.anm")
merge("Data/Characters/Golem_Eye_Charge.anm")
merge("Data/Characters/Golem_Eye_Death.anm")
merge("Data/Characters/Golem_Eye_Idle.anm")
merge("Data/Characters/Golem_Eye_Stun.anm")
merge("Data/Characters/Golem_Eye_Walk.anm")


tGolemToErase = {}

tGolemStrong = {}
tGolemStrong.shader = ShaderFx.find("GolemCGFX")
tGolemStrong.skinMesh = SkinMesh.find ( "Golem")
tempStrong = tGolemStrong.skinMesh:clone("Golem_EyeDoubleClone")
tGolemStrong.skinMesh:release()
tGolemStrong.skinMesh = tempStrong
tGolemStrong.skinMesh:shadingGroup(-1)

tGolemEye = {}
tGolemEye.shader = ShaderFx.find("GolemLaserCGFX")
tGolemEye.skinMesh = SkinMesh.find ( "Golem_Eye")
tempEye = tGolemEye.skinMesh:clone("Golem_EyeDoubleClone")
tGolemEye.skinMesh:release()
tGolemEye.skinMesh = tempEye
tGolemEye.skinMesh:shadingGroup(-1)





GolemStrongWalk = AnimData.find("Golem_Walk")

GolemStrongSlowWalk = GolemStrongWalk:clone("Golem_slow_walk")
GolemStrongSlowWalk:speed(0.5)




GolemStrongSmash = AnimData.find("Golem_Smash")
GolemStrongSmash:speed ( 0.85 );
GolemStrongStun = AnimData.find("Golem_Strong_Stun")
GolemStrongStun:speed ( 0.40 );


GolemEyeDeath = AnimData.find ( "Golem_Eye_Death")
GolemEyeDeath:speed( 0.5)
GolemEyeStun = AnimData.find ( "Golem_Eye_Stun")
GolemEyeStun:speed ( 0.35)

tGolemStrong.skinMesh:visible ( false )
tGolemEye.skinMesh:visible ( false )

-- Integrer le deuxieme petit + les anims selon leur deplacement -- :: //
tEnemy = {}

nEnemyY = 0
iSkinmesh = 0
function CreateEnemy(Index, PositionX, PositionZ, Radius, Angle, Type, Height)

	Index = Index+1


	tEnemy [Index] = nil
	tEnemy [Index] ={}
	tEnemy[Index].transform = Transform.new ()

	if Type == 0 then -- Little
		tEnemy[Index].transform:scale ( 0.5,0.5,0.5)
		tEnemy[Index].skinMesh = tGolemEye.skinMesh:clone("Golem_EyeClone" .. Index )
		tEnemy[Index].skinMesh:parent ( tEnemy[Index].transform )
		tEnemy[Index].transform:animPlay("Golem_Eye_Idle",true)
		tEnemy[Index].particules = particleSparks:clone();
		tEnemy[Index].particules:parent( tEnemy[Index].transform )
		tEnemy[Index].particules:position( 0,1.5,1)
		--tEnemy[Index].skinMesh:shader(tGolemEye.shader:clone("GolemEyeShaderClone"..Index) );
		tEnemy[Index].type = FAST_AND_RANGED
	else -- Big one
		iSkinmesh = iSkinmesh+1
		tEnemy[Index].skinMesh = tGolemStrong.skinMesh:clone("Golem_StrongClone" .. iSkinmesh)
		tEnemy[Index].skinMesh:parent ( tEnemy[Index].transform )
		print ("Create enemy ".. Index)
		if Type == 1 then
		tEnemy[Index].transform:animPlay(GolemStrongWalk,true)
		else
			tEnemy[Index].transform:animPlay("Golem_Idle",true)
		end
		--tEnemy[Index].skinMesh:shader(tGolemStrong.shader:clone("GolemShaderClone"..Index) );
		tEnemy[Index].type = STRONG_AND_SLOW
	end

	tEnemy[Index].skinMesh:visible(true)
	tEnemy[Index].skinMesh:shadingGroup(-1)
	tEnemy[Index].transform:position( PositionX , Height + nEnemyY, -PositionZ)
	tEnemy[Index].transform:rotation( 0,  math.deg( Angle ), 0 )
	tEnemy[Index].state =  1

end

function CleanEnemy ()
	for i =1, #tGolemToErase do
		tGolemToErase[i].Mesh:visible(false)

		tGolemToErase[i] = nil
		tGolemToErase[i] = {}
	end
	tGolemToErase = nil
	tGolemToErase = {}
end


function KillEnemy( index  )

	index = index+1
	print("Kill enemy "..index)
	if tEnemy[index].type == STRONG_AND_SLOW then
		tEnemy[index].transform:animPlay("Golem_Strong_Death")

	else
		tEnemy[index].transform:animPlay(GolemEyeDeath)
		tEnemy[index].particules:release()
	end
end

function DestroyEnemy( Index )

	Index = Index + 1
print("destroy enemy ".. Index)
print(" enemy count ".. #tEnemy)
	tEnemy[Index].skinMesh:release()
	tEnemy[Index].transform:release()
	--tEnemy[Index] = nul
print(" enemy count ".. #tEnemy)
	if( Index < #tEnemy) then
		print("Replacing by  enemy ".. #tEnemy)
		tEnemy[Index] =  tEnemy[#tEnemy]

	end

	tEnemy[#tEnemy] = nil

end

function UpdateEnemyDead ( index, disintegrationValue)
	index = index + 1

--print("Update dead : "..index)
	tEnemy[index].skinMesh:shader():setParam("disintegrationPower",disintegrationValue)
end

function UpdateEnemyColor(index, fresnelStrength, fresnWidth, r, g ,b )
	index = index + 1
	tEnemy[index].skinMesh:shader():setParam("fresnelColor", Vector(r, g, b))
	tEnemy[index].skinMesh:shader():setParam("fresnelWidth", fresnelWidth)
	tEnemy[index].skinMesh:shader():setParam("fresnelStrength",fresnelStrength)
end


function UpdateEnemy( Index, PositionX, PositionZ, Angle, state, r, g, b , Height)
	Index = Index+1
--	print ("update enemy ".. Index)
	--print(tEnemy[Index].skinMesh:worldPosition())
	--print ("Sking mesh pos : " .. tEnemy[Index].skinMesh:position() )
	tEnemy[Index].transform:rotation( 0,  math.deg( Angle ) +90, 0 )
	tEnemy[Index].transform:position( PositionX , Height + nEnemyY, -PositionZ)


	if  tEnemy[Index].state ~= state then
		tEnemy[Index].state = state
	    if tEnemy[Index].type == STRONG_AND_SLOW then
    		tEnemy[Index].skinMesh:shader():setParam("glowColor", Vector(r,g,b) )
	 			if state == STATE_STRONG_CHARGING then
				tEnemy[Index].transform:animPlay("Golem_Run", true)
				elseif state == STATE_SLACKING then

	  			tEnemy[Index].transform:animPlay(GolemStrongWalk, true)
				elseif state == STATE_PURSUIT then
  				tEnemy[Index].transform:animPlay(GolemStrongWalk, true)
					GolemStrongWalk:speed(1)
	  		elseif state ==	STATE_STRONG_ATTACK then
	  			tEnemy[Index].transform:animPlay(GolemStrongSmash)
	  		elseif state == STATE_STRONG_STUN then
	  			tEnemy[Index].transform:animPlay(GolemStrongStun);
	  		end
	  	elseif tEnemy[Index].type == FAST_AND_RANGED then
				if state == STATE_PURSUIT then
					tEnemy[Index].transform:animPlay("Golem_Eye_Walk", true)
				elseif state == STATE_SLACKING then

		  			tEnemy[Index].transform:animPlay("Golem_Eye_Idle", true)
	  		elseif state == STATE_EYE_CHARGE then
					tEnemy[Index].transform:animPlay("Golem_Eye_Charge", true)
					tEnemy[Index].particules:play();

				elseif state == STATE_EYE_FIRE then
					tEnemy[Index].transform:animPlay("Golem_Eye_Beam",false)
					tEnemy[Index].particules:stop();
				elseif state == STATE_EYE_STUN then
		  			tEnemy[Index].transform:animPlay(GolemEyeStun);
		  			tEnemy[Index].particules:stop();

				elseif state ==	STATE_EYE_WALKING then

						tEnemy[Index].transform:animPlay("Golem_Eye_Walk", true)
	  		end
	  	end
	end
end

function Promote( id )
	tEnemy[id + 1].transform:scale( tEnemy[id + 1].transform:scale()*1.5 )
	print( "promote enemiiiiiiiiiiiii")
end
