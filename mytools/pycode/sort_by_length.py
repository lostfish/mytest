#!/usr/bin/python
#coding:utf-8

import sys

a = [(line,len(line)) for line in file(sys.argv[1])]

a.sort(key=lambda x:x[1])
for i,j in a:
	print i,
