#!/bin/bash


RED='\033[0;31m'
clr='\033[0m'


kill_processes()
{

if [ -z "$1" ]; then
	echo "First argument is missing"
	return -1
fi

reg='/'"$1"'/ && ! /grep/ && ! /bash/  {print $2}'

ps -aux 2>&1 |  awk "$reg"  | while read line
do
	kill -SIGTERM  "$line" || true
	echo -e  "${RED}Process with pid :""$line"" terminated${clr}"
done

}

