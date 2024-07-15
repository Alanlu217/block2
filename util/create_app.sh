#!/bin/bash

src_dir=$1
dir=$2
name=$3

cd ${dir}
mkdir -p app_out/${name}.app/Contents/MacOS

cp ${name} app_out/${name}.app/Contents/MacOS
cp -r assets app_out/${name}.app/Contents/MacOS

cd app_out/${name}.app/Contents/

echo "<?xml version=\"1.0\" ?>
<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN" "https://www.apple.com/DTDs/PropertyList-1.0.dtd\">
<plist version=\"1.0\">
 <dict>
  <key>LSMinimumSystemVersion</key>
  <string>10.11</string>
  <key>CFBundleDevelopmentRegion</key>
  <string>English</string>
  <key>CFBundleAllowMixedLocalizations</key>
  <true/>
  <key>CFBundleExecutable</key>
  <string>main</string>
  <key>CFBundleIconFile</key>
  <string>Test.icns</string>
  <key>CFBundleInfoDictionaryVersion</key>
  <string>6.0</string>
  <key>CFBundleName</key>
  <string>${name}</string>
  <key>CFBundlePackageType</key>
  <string>APPL</string>
  <key>CFBundleShortVersionString</key>
  <string>1.0.0</string>
  <key>CFBundleSignature</key>
  <string>????</string>
  <!-- See https://developer.apple.com/app-store/categories/ for list of AppStore categories -->
  <key>CFBundleVersion</key>
  <string>1.0.0</string>
  <key>NSHighResolutionCapable</key>
  <string>true</string>
 </dict>
</plist>" > Info.plist

cd MacOS

echo "#!/bin/bash
path=\$(dirname \"\$0\");cd \$path;./${name}" > main
chmod +x main
