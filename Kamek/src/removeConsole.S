.text
.align 4
#include <asm_setup.S>

.extern _Z16NoConsoleAllowedv
.extern LoaderDotBinPointer
.extern ContinueFromConsole

#Basically, on Console, Loader.bin is injected at 0x80001800, which isn't possible on emulator, that's how i detect if the player is playing on console or not.

.global PreventConsoleUse
PreventConsoleUse:
	lis		r5, LoaderDotBinPointer@h
	ori		r5, r5, LoaderDotBinPointer@l
	stw		r6, 0(r5)
	cmpwi	r6, 0x00F0
	bne		goInHell
	stwu	r1, -0x10(r1)
	b		ContinueFromConsole
goInHell:
	b		_Z16NoConsoleAllowedv