#!/bin/bash

version=$1

cd target

rm -rf apps
mkdir apps

cd release
rm -rf app_out
ninja app
mv app_out/block.app "../apps/block-amd64-v${version}.app"

cd ../macosx-arm64-release
rm -rf app_out
ninja app
mv app_out/block.app "../apps/block-arm64-v${version}.app"

cd ..
cd apps

zip -rq "block-amd64-v${version}.zip" "block-amd64-v${version}.app"
zip -rq "block-arm64-v${version}.zip" "block-arm64-v${version}.app"

rm -rf "block-amd64-v${version}.app"
rm -rf "block-arm64-v${version}.app"

open .
