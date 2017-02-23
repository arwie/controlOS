#!/bin/sh

SCRIPT=$(readlink -f $0)
BASEDIR=$(dirname $SCRIPT)


SSHHOME_TRGT="$BASEDIR/../root/projectroot/root/.ssh"
SSHHOME_HOST="./ssh"

KEYNAME="id_rsa"


mkdir  $SSHHOME_HOST
rm -rf $SSHHOME_HOST/*

ssh-keygen  -f $SSHHOME_HOST/$KEYNAME  -N ""


cp  -f  $SSHHOME_HOST/$KEYNAME.pub  $SSHHOME_TRGT/authorized_keys
