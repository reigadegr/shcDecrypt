#!/system/bin/sh
shcfile=$(pwd)/iris_helper
chmod +x $shcfile
nohup $shcfile >$(pwd)/log.log 2>&1 &
for i in $(pidof $(basename $shcfile)); do
    cat /proc/$i/cmdline > "$(pwd)/shc解密$i.txt"
done
killall -15 $(basename $shcfile)
