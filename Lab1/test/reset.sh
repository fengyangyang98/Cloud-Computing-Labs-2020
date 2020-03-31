#!/bin/bash
user_name=$(whoami| awk '{print $1}')
rm result*
rm test1.log
rm ${user_name}*
rm test*.test