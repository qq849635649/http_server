#!/bin/sh
alias cp='cp'
cp etc / -arf
cp usr / -arf
chkconfig --add adv
chkconfig --level 2345 adv on
