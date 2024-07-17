#!/bin/bash

BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

MACOS_SDK_PATH=$(xcrun --show-sdk-path --sdk macosx)
XNU_VER="xnu-11417.121.6"
DYLD_VER="dyld-1285.19"

# TODO: get dyld_cache_format.h

echo "[+] Fetching sources"
mkdir -p "./graft/downloads"
if [[ ! -f "./graft/downloads/${XNU_VER}.tar.gz" ]]
then
    curl -LsS -o "./graft/downloads/${XNU_VER}.tar.gz" "https://github.com/apple-oss-distributions/xnu/archive/${XNU_VER}.tar.gz"
fi

if [[ ! -f "./graft/downloads/${DYLD_VER}.tar.gz" ]]
then
    curl -LsS -o "./graft/downloads/${DYLD_VER}.tar.gz" "https://github.com/apple-oss-distributions/dyld/archive/${DYLD_VER}.tar.gz"
fi

echo "[+] Extracting sources"
mkdir -p "./graft/src/xnu"
tar xzf "./graft/downloads/${XNU_VER}.tar.gz" -C "./graft/src/xnu/" --strip-components=1 
mkdir -p "./graft/src/dyld"
tar xzf "./graft/downloads/${DYLD_VER}.tar.gz" -C "./graft/src/dyld/" --strip-components=1 


echo "[+] Extracting constants from xnu's fcntl.h"
mkdir -p "./graft/headers/"
cat "./graft/src/xnu/bsd/sys/fcntl.h" | grep -Eo "#define[ \t]*(F_OPENFROM|F_UNLINKFROM|F_CHECK_OPENEVT|F_MARKDEPENDENCY|F_SETSTATICCONTENT|F_MOVEDATAEXTENTS|F_GETDEFAULTPROTLEVEL|F_GETDEFAULTPROTLEVEL|F_MAKECOMPRESSED|F_SET_GREEDY_MODE|F_SETIOTYPE|F_RECYCLE|F_OFD_SETLK|F_OFD_SETLKW|F_OFD_GETLK|F_OFD_SETLKWTIMEOUT|F_OFD_GETLKPID|F_SETCONFINED|F_GETCONFINED|F_NOCACHE_EXT)[ \t]*[0-9]*" > "./graft/headers/xnu_constants.h"

echo "[+] Extracting dyld_cache_format.h from dyld"
cp "./graft/src/dyld/include/mach-o/dyld_cache_format.h" "./graft/headers/dyld_cache_format.h"


if [[ $1 == "IOS" ]]
then
    KTRACE_TBD="${MACOS_SDK_PATH}/System/Library/PrivateFrameworks/ktrace.framework/ktrace.tbd"

    echo "[+] Creating ktrace.tbd"
    mkdir -p "${BASE_DIR}/graft/private_tbd/ktrace.framework/"
    cp "${KTRACE_TBD}" "${BASE_DIR}/graft/private_tbd/ktrace.framework/ktrace.tbd"
    patch -p0 < patches/ios_ktrace_tdb.patch


    echo "[+] Copying iOS-specific headers"
    mkdir -p "./graft/ios_headers/sys/"
    cp "./graft/src/xnu/bsd/sys/disk.h" "./graft/ios_headers/sys/disk.h"
    cp "./graft/src/xnu/bsd/sys/kdebug.h" "./graft/ios_headers/sys/kdebug.h"
fi

