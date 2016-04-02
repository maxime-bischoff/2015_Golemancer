Sound.preload("Data/Sound/Ambiance_Placeholder_stmartin");
Sound.preloadSFX("Data/Sound/HeyListen");
Sound.preload("Data/Sound/orbe_idle");
Sound.preload("Data/Sound/envi_base");
Sound.preloadSFX("Data/Sound/no_mana");
Sound.preloadSFX("Data/Sound/wrong_rune");
--orbe_idle = Sound.play3D("orbe_idle", false)
envi_base =  Sound.play2D("envi_base", false)

PlayerStepCoolDown = 0

--heyListen = Sound.play2D("HeyListen", false)
--Music = Sound.play2D("Ambiance_Placeholder_stmartin", false)

--envi_base:play ( true, true );
if  orbe_idle ~= nil then
--orbe_idle:parent( tFocusTransform )
--orbe_idle:play( true, true ) ;
--orbe_idle:volume (0.75);
end

print("Start Playing Ambiance Music")
if( Music ~= nill) then
	Music:volume( 0 )
end


bPlayingListen = false
function PlaySoundHeyListen( mustPlay )

	if mustPlay then
		if bPlayingListen == false then
		--	heyListen:play()
			bPlayingListen = true
		end
	else
		bPlayingListen = false
	end
end


function PlaySoundNoMana ()
	Sound.play2D ( "no_mana")
end

function PlaySoundWrongRune()
	Sound.play2D ( "wrong_rune") ;
end

function PlayFootStep()
--[[
	if (PlayerStepCoolDown <= 0  )then
		local StepSound = Sound.play2D ( "wrong_rune") ;
		if StepSound ~= nil then
			StepSound:pitch( math.random( 0.8, 1.2 ))
			StepSound:volume(0.5)
			--print("tap")
			PlayerStepCoolDown = 0.1
		end
	end
]]
end

function UpdateSoundCoolDown()

	PlayerStepCoolDown = PlayerStepCoolDown - Clock:deltaTime()
end
