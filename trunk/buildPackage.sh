#!/bin/sh

./buildRelease.sh

sourcedirname=universalindentgui-1.0.1
sourcefilename=$sourcedirname.tar.gz
sourcefilenameorig=universalindentgui_1.0.1.orig.tar.gz

targetDir=packaging

echo "Delete old target dir and create new one"
echo "----------------------------------------"
if [ -d "$targetDir" ]; then
    rm -r $targetDir &> /dev/null
fi
if [ $? -gt 0 ]; then
    echo "ERROR: Deleting dir $targetDir failed!"
    exit 1
fi

# wait until the old target dir has been really deleted
while [ -d "$targetDir" ]
do
    sleep 1
done

mkdir $targetDir &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir $targetDir failed!"
    exit 1
fi

# wait until the new target dir has been really created
while [ ! -d "$targetDir" ]
do
    sleep 1
done


echo "Copying the original source files"
echo "---------------------------------"
cp ./$sourcefilename ./$targetDir/ &> /dev/null
if [ $? -gt 0 ]; then
echo "ERROR: Could not copy file!"
exit 1
fi
cp ./$sourcefilename ./$targetDir/$sourcefilenameorig &> /dev/null
if [ $? -gt 0 ]; then
echo "ERROR: Could not copy file!"
exit 1
fi
echo "Done"
echo ""

# wait until the copied files have been really created
while [ ! -f "./$targetDir/$sourcefilenameorig" ]
do
    sleep 1
done

cd $targetDir

tar -xzvf $sourcefilenameorig

cp -r ../debian ./$sourcedirname

cd $sourcedirname/debian
rm -Rfv .svn
debuild -S -us -uc

echo "Everything completed successfully!"
if [ "$WARNINGOCCURRED" = "true" ]; then
    echo "====> But there were some warnings. Please check that!"
fi
#read -p "press any key to continue"
