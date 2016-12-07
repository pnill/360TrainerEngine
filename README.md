# 360TrainerEngine
An xbox360 trainer(cheat) engine, the first with a UI/passcode system.

This trainer engine is also the same one in use by TeamXPG today, it was given to them around 2012-2013.

# BlackOps2_D3D.h
This is outdated, the game has been updated since this was pushed there are two sets of code in it...

1 set of code actually hooks DirectX and messes with draw functions when specific objects are being drawn in order to acheive "chams" as well as a 'wallhack' (seeing people through the wall).

Later on I determined that there was no efficient way of doing this with just DirectX so I manipulated the actual engine of the game in order to draw enemy name tags and bodies through walls.

# HudD3D.h
An incomplete HUD hook attempt for a completed one I would review FreeStyleDash(aka phoenix, aka Aurora)

# Kameo.h
Is one of the traditional game cheats and probably the easiest to learn from most of the operations are just overwriting (poking) memory at specific addresses...
In this instance however a hook was performed in order to accomplish both 1 hit KO/ and inf health.

The 'coin' or 'rune' hack in the game also shows a demonstration of dealing with DMA (Dynamic Memory Allocation)
*(DWORD*)(**(DWORD**)0x82B62568+0x18) = 0xFFFF;

Essentially by breakpointing the area of memory on write and back tracing I was able to determine how the game found the exact pointer to the area in memory after it allocated it,
Allowing me to statically set the runes to a specific value.
	
(A demonstration of this one can also be seen here: https://www.youtube.com/watch?v=xljqmfnD0w0 ) - Thanks WizardHax.

# Others
There were hacks created for Alan Wake, Kingdoms of Amular and a few others...

Alan wake was a bit special as it was done completely with static reversing and debugging no memory was searched,
everything found was string reference (no breakpoints).

Kindoms of Amular can be seen here (https://www.youtube.com/watch?v=2DSulwDZDfg) - video quality sucks.

Alan wake videos can be seen here (This was the first trainer I ever made for the 360):

https://www.youtube.com/watch?v=AH6N5fpWXNE
https://www.youtube.com/watch?v=iIvBUB5Dozc
