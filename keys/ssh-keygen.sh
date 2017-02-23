#!/bin/sh

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
