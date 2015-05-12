#!/usr/bin/env python
# coding:utf-8

from bs4 import BeautifulSoup
import sys
import re
import os


def resolve_page(dirPath):
    for filename in os.listdir(dirPath):
        if not filename.endswith('html'):
            continue
        data = file(os.path.join(dirPath,filename)).read()
        soup = BeautifulSoup(data)
        #x = soup.find_all('div',class_='pl2') #for movie
        x = soup.find_all('div',class_='info') #for book for tag in x:
            info = tag.getText().encode('utf-8')
            s = [line.strip() for line in info.split('\n') if len(line.strip()) > 2]
            s.append(tag.a['href'])
            info = '\t'.join(s)
            print info

if __name__ == '__main__':
    resolve_page(sys.argv[1])

