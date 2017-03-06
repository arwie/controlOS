#!/bin/sh

# Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


SCRIPT=$(readlink -f $0)
BASEDIR=$(dirname $SCRIPT)


SSHHOME_HOST="$BASEDIR/ssh"
SSHHOME_TRGT="$BASEDIR/../root/projectroot/root/.ssh"
SSHHOME_TRGT_MC_RT="$BASEDIR/../mc/conf/root/.ssh"
SSHHOME_TRGT_MC_MC="$BASEDIR/../mc/conf/home/mc/.ssh"

KEYNAME="id_rsa"


mkdir  -p  $SSHHOME_HOST
rm     -rf $SSHHOME_HOST/*

ssh-keygen  -f $SSHHOME_HOST/$KEYNAME  -N ""


mkdir  -p  $SSHHOME_TRGT
rm     -rf $SSHHOME_TRGT/*

ssh-keygen  -f $SSHHOME_TRGT/$KEYNAME  -N ""


cp  -f  $SSHHOME_HOST/$KEYNAME.pub  $SSHHOME_TRGT/authorized_keys


mkdir  -p  $SSHHOME_TRGT_MC_RT
cp  -f  $SSHHOME_TRGT/$KEYNAME.pub  $SSHHOME_TRGT_MC_RT/authorized_keys

mkdir  -p  $SSHHOME_TRGT_MC_MC
cp  -f  $SSHHOME_TRGT/$KEYNAME.pub  $SSHHOME_TRGT_MC_MC/authorized_keys
