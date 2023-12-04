#!/system/bin/sh
shcfile=$(pwd)/iris_helper
base_name_shcfile=$(basename $shcfile)
chown 0:0 $shcfile
chmod +x $shcfile
nohup $shcfile >$(pwd)/log.log 2>&1 &
for i in $(pidof $base_name_shcfile); do
    su -c cat /proc/$i/cmdline > "$(pwd)/shc解密$i.txt"
done
killall -15 $(basename $shcfile)
