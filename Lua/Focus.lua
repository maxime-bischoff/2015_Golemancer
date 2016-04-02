merge("Data/Orb/orb.pak")

tFocus = Transform.find("Orb")
tFocusTransform = Transform.new()
tFocus:parent( tFocusTransform )

tFocus:animPlay("Idle", true)
local orbScale = 0.11
tFocusTransform:scale(orbScale,orbScale,orbScale)
tFocusTransform: rotation( 0 ,90, 0)
tFocusHeight = 0.6;
lastState = 0;
tFocus:position (0,0,0)

local lastTimeIdle = 0
local idle2delay = 12.0
local idle2IsPlaying = false

nParticlesImpactTimer = 0
bParticlesImpactPlaying = false
	tFocus:animPlay("Idle", true)
function UpdateFocus(  PositionX, PositionZ, angle, state, Height )
	tFocusTransform:position( PositionX ,tFocusHeight + Height, -PositionZ)
	tFocusTransform:rotation(0,math.deg(angle),0)


	--tFocus:position (0,0,0)
	if state ~= lastState then
		if state ~= 0 then
			tFocus:animPlay("Orb_Attack", true)
		else
			tFocus:animPlay("Idle", true)


		end
		lastState = state
	end


	if state == 0 then
		lastTimeIdle = lastTimeIdle +  Clock:deltaTime()

		if(lastTimeIdle > idle2delay) then

			local randIdle = math.random(1,2)

			if randIdle == 1 then
					tFocus:animPlay("Orb_Deploy", false)
			else
					tFocus:animPlay("Orb_Idle2", false)
			end
					idle2IsPlaying = true
					lastTimeIdle = 0
		end

		if(idle2IsPlaying == true and not tFocus:animPlaying()) then
			tFocus:animPlay("Idle", true)
		end
	end

	if state == 2 then
		if not tFocus:animPlaying() then
		tFocus:animPlay("orb_attack", true)
		end
	end

	--tFocus:rotation( cam:rotation())

	--||| PARTICLES SHUT DOWN |||---
	if(	bParticlesImpactPlaying ) then
		if(nParticlesImpactTimer < 0 ) then
			particleImpact:stop()
			bParticlesImpactPlaying = false
			print( "stop particles" )
		else
			nParticlesImpactTimer = nParticlesImpactTimer - Clock:deltaTime(1)
		end
	end
end



function FocusWait( PosX, PosY, PosZ  )
	tFocusTransform:position (PosX, PosY, -PosZ)
end

function ShowFocus(bSHow)
	tFocus:visible(bShow)
end

function StartImpact()
	particleImpact:play()
	nParticlesImpactTimer = 0.1
    bParticlesImpactPlaying = true
end
