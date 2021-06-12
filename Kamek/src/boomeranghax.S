.text
.align 4
#include <asm_setup.S>

.extern DoFrogFootsteps
.extern DontDoFrogFootsteps
.extern NoSwimBegin
.extern YesSwimBegin
.extern NoSwimHip0a
.extern YesSwimHip0a
.extern NoSwimHip0b
.extern YesSwimHip0b
.extern NoSwimHip1
.extern YesSwimHip1
.extern NoSwimHip2
.extern YesSwimHip2
.extern NoSwimHip3a
.extern YesSwimHip3a
.extern NoSwimHip3b
.extern YesSwimHip3b

.global FrogFootsteps
FrogFootsteps:
	cmpwi r0, 5			#If current powerup isn't penguin
	bne notPenguin		#Then notPenguin
	b DoFrogFootsteps	#Otherwise, DoFrogFootsteps
notPenguin:
	cmpwi r0, 11				#If current powerup isn't frog neither
	bne _DontDoFrogFootsteps	#Then DontDoFrogFootsteps
	b DoFrogFootsteps			#Otherwise, DoFrogFootsteps
_DontDoFrogFootsteps:
	b DontDoFrogFootsteps
#So basically, Treeki was too lazy to implement rel type 11, so i can't "beq" or "bne" externs, so i have to do it on another function that then calls the extern in question.

.global FrogSwimBegin
FrogSwimBegin:
	cmpwi r0, 5
	bne stillNotPenguin
	b YesSwimBegin
stillNotPenguin:
	cmpwi r0, 11
	bne _NoSwimBegin
	b YesSwimBegin
_NoSwimBegin:
	b NoSwimBegin


.global FrogSwimHip0a
FrogSwimHip0a:
	cmpwi r0, 5
	bne nyehNotPenguinA
	b YesSwimHip0a
nyehNotPenguinA:
	cmpwi r0, 11
	bne _NoSwimHip0a
	b YesSwimHip0a
_NoSwimHip0a:
	b NoSwimHip0a


.global FrogSwimHip0b
FrogSwimHip0b:
	cmpwi r0, 5
	beq _YesSwimHip0b
	cmpwi r0, 11
	beq _YesSwimHip0b
	b NoSwimHip0b
_YesSwimHip0b:
	b YesSwimHip0b


.global FrogSwimHip1
FrogSwimHip1:
	cmpwi r0, 5
	bne bruhNotPenguin
	b YesSwimHip1
bruhNotPenguin:
	cmpwi r0, 11
	bne _NoSwimHip1
	b YesSwimHip1
_NoSwimHip1:
	b NoSwimHip1


.global FrogSwimHip2
FrogSwimHip2:
	cmpwi r0, 5
	bne grrNotPenguin
	b YesSwimHip2
grrNotPenguin:
	cmpwi r0, 11
	bne _NoSwimHip2
	b YesSwimHip2
_NoSwimHip2:
	b NoSwimHip2


.global FrogSwimHip3a
FrogSwimHip3a:
	cmpwi r0, 5
	beq _YesSwimHip3a
	cmpwi r0, 11
	beq _YesSwimHip3a
	b NoSwimHip3a
_YesSwimHip3a:
	b YesSwimHip3a


.global FrogSwimHip3b
FrogSwimHip3b:
	cmpwi r0, 5
	bne owoNotPenguin
	b YesSwimHip3b
owoNotPenguin:
	cmpwi r0, 11
	bne _NoSwimHip3b
	b YesSwimHip3b
_NoSwimHip3b:
	b NoSwimHip3b