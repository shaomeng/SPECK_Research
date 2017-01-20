#!/bin/bash

VAR='/flash_buffer/Sam/vx.025'
i=0

# while [ $i -lt 64 ]; do
# 	ICB="$VAR.icb.cube$i"
# 	STREAM="$VAR.stream.cube$i"
# 	echo "working on $ICB..."

# 	speckencode3d 32 $ICB $STREAM
# 	let i=i+1
# done

while [ $i -lt 64 ]; do
	STREAM="$VAR.stream.cube$i"
	ICBRECOVER="$VAR.icb.4to1.cube$i"
	echo "working on $ICBRECOVER..."

	speckdecode3d -r 8 $STREAM $ICBRECOVER
	let i=i+1
done
