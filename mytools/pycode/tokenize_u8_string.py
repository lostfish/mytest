#!/usr/bin/python
#coding:utf-8

import sys

def get_common_prefix(s1,s2):
	n = min(len(s1),len(s2))
	count = 0
	for i in range(n):
		if s1[i] == s2[i]:
			count += 1
		else:
			break
	return count

def tokenize_u8_string(s):
	words = []
	i = 0
	n = len(s)
	while i < n:
		value = ord(s[i])
		if value < 0x80:
			words.append(s[i])
			i += 1
			continue
		else:
			bytes = 0
			mask = 0x80
			while value & mask == mask:
				bytes += 1
				mask = mask >> 1
			words.append(s[i:i+bytes])
			i += bytes
	return words

if __name__ == '__main__':
	s = '''`~!@#$%^&*()_+=-{}[]\|;':",./<>?·～！@#￥%……&×（）——-=+【】、；‘，。/『』|：“《》？'''
	print "s:",s
	words = tokenize_u8_string(s)
	for w in words:
		print w

