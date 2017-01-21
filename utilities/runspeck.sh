#!/bin/bash

VAR='/flash_buffer/Sam/original_icb/vx.025'
i=0

#while [ $i -lt 64 ]; do
# 	ICB="$VAR.icb.cube$i"
# 	STREAM="$VAR.stream.cube$i"
# 	echo "working on $ICB..."

# 	speckencode3d -nl 5 8 $ICB $STREAM &
# 	let i=i+1
#done

#while [ $i -lt 64 ]; do
#	STREAM="$VAR.stream.cube$i"
#	ICBRECOVER="$VAR.icb.4to1.cube$i"
#	echo "working on $ICBRECOVER..."

#	speckdecode3d $STREAM $ICBRECOVER &
#	let i=i+1
#done

while [ $i -lt 64 ]; do
 	ICB="$VAR.icb.cube$i"
 	ICBRECOVER="$VAR.icb.16to1.cube$i"
	echo "working on $ICBRECOVER..."
	
	./bin/imagecubeDwtCompression $ICB $ICBRECOVER 16 & 
	let i=i+1
done
