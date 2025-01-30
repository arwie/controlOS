#!/bin/sh

BASEDIR=$(dirname $(realpath $0))

cd $BASEDIR/..


PLATFORM=configs/platform-$(ptxdist print PTXCONF_PLATFORM)

cat base/base/$PLATFORM/kernelconfig.d/* > $PLATFORM/kernelconfig
cat base/$PLATFORM/kernelconfig.d/* >> $PLATFORM/kernelconfig
cat $PLATFORM/kernelconfig.d/* >> $PLATFORM/kernelconfig
rm -f $PLATFORM/kernelconfig.diff

ptxdist kernelconfig
