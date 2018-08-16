#!/bin/bash

ROOT_DIRS=$(cd $(dirname $(readlink -f "${BASH_SOURCE[0]}")) && pwd )
export ROOT_DIRS

CROSSTOOLDIR=${ROOT_DIRS}/cross-tool
echo ${CROSSTOOLDIR}

if [ -f ${CROSSTOOLDIR}/ql-ol-crosstool/ql-ol-crosstool-env-init -a -x ${CROSSTOOLDIR}/ql-ol-crosstool/ql-ol-crosstool-env-init ]
then
	source ${CROSSTOOLDIR}/ql-ol-crosstool/ql-ol-crosstool-env-init
elif [ -f ${CROSSTOOLDIR}/ql-ol-crosstool.tar.bz2 ]
then
	pushd ${CROSSTOOLDIR}/
	tar -xvf ${CROSSTOOLDIR}/ql-ol-crosstool.tar.bz2
	source ${CROSSTOOLDIR}/ql-ol-crosstool/ql-ol-crosstool-env-init
	popd
else
	echo "error : crosstool inexistence !"
fi

