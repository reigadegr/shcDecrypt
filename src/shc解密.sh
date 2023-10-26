#!/system/bin/sh
shcfile=$(pwd)/service.sh.x
nohup $shcfile >$(pwd)/log.log 2>&1 &
m=$(ps -ef | grep "$shcfile" | grep -v grep| awk '{print $2}')
m2=$(ps -ef | grep "$shcfile" | grep -v grep| awk '{print $3}')
cat /proc/$m/cmdline > shc解密.txt
cat /proc/$m2/cmdline > shc解密2.txt
