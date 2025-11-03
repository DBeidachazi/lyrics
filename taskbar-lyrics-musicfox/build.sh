#!/bin/bash
# 交叉编译 go-musicfox 为 Windows 可执行文件

set -o errexit
set -o nounset
set -o pipefail
set -x

# 切换到项目目录
cd ../go-musicfox

# 构建参数
export GOOS=windows
export GOARCH=amd64
export CGO_ENABLED=1
export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-g++
export LDFLAGS="-s -w"

# =============================
# 调用 make（hack/build.sh）进行构建
# =============================
make build

# =============================
# 处理输出目录
# =============================
mkdir -p ../bin

# 删除旧文件
if [ -f ../bin/musicfox.exe ]; then
  rm -f ../bin/musicfox.exe
fi

# 移动新编译产物
if [ -f ./bin/musicfox ]; then
  mv ./bin/musicfox ../bin/musicfox.exe
elif [ -f ./bin/musicfox.exe ]; then
  mv ./bin/musicfox.exe ../bin/
else
  echo "❌ 未找到编译产物 ./bin/musicfox 或 ./bin/musicfox.exe"
  exit 1
fi

# 清理中间产物
rm -rf ./bin
