#!/bin/bash
ls $1 >> main/fileList.txt
v2=$PWD
v2+="/main"
cd main
./main "$v2" new $1
./main "$v2" search $2
