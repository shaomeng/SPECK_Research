#!/bin/bash

ARCH=`uname`
HOSTNAME=`hostname`
USER=`whoami`

# Please source this file, rather than execute it.

if [[ "$ARCH" == 'Darwin' ]]; then
  QCC_DIR='/Users/shaomeng/Install/QccPack-0.61'
  export DYLD_LIBRARY_PATH=${QCC_DIR}/lib:$(echo $DYLD_LIBRARY_PATH)
  export PATH=${QCC_DIR}/bin:$(echo $PATH)
  export MANPATH=${QCC_DIR}/man:$(echo $MANPATH)
elif [[ $USER == 'shaomeng' ]]; then
  QCC_DIR='/glade/u/home/shaomeng/lib_gnu/QccPack-0.61'
  export LD_LIBRARY_PATH=${QCC_DIR}/lib:$(echo $LD_LIBRARY_PATH)
  export PATH=${QCC_DIR}/bin:$(echo $PATH)
  export MANPATH=${QCC_DIR}/man:$(echo $MANPATH)
else              # Alaska
  QCC_DIR='/home/users/samuelli/Install/QccPack-0.61'
  export LD_LIBRARY_PATH=${QCC_DIR}/lib:$(echo $LD_LIBRARY_PATH)
  export PATH=${QCC_DIR}/bin:$(echo $PATH)
  export MANPATH=${QCC_DIR}/man:$(echo $MANPATH)
fi
