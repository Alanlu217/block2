#!/bin/bash

version=$1

cd target

rm -rf apps
mkdir apps

cd release
rm -rf app_out
cmake --build . --target app
mv app_out/block.app "../apps/block-macos-amd64-v${version}.app"

cd ../macosx-arm64-release
rm -rf app_out
cmake --build . --target app
mv app_out/block.app "../apps/block-macos-arm64-v${version}.app"

cd ..
cd apps

zip -rq "block-macos-amd64-v${version}.zip" "block-macos-amd64-v${version}.app"
zip -rq "block-macos-arm64-v${version}.zip" "block-macos-arm64-v${version}.app"

rm -rf "block-macos-amd64-v${version}.app"
rm -rf "block-macos-arm64-v${version}.app"

open .
