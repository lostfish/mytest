#!/usr/bin/env python
# coding:utf-8

#from bs4 import BeautifulSoup
import urllib2
import os
import sys
import re
import httplib
import time

httplib.HTTPConnection._http_vsn= 10
httplib.HTTPConnection._http_vsn_str = 'HTTP/1.0'


def get_proxy(filepath="./proxy.txt"):
    '''
    Read file to get proxy list

    '''
    my_proxy = []
    for line in file(filepath):
        s = line.strip().split('\t',2) # line format: ip port ...
        ip = "http://"+s[0]+":"+s[1]
        my_proxy.append(ip)
    return my_proxy

def set_proxy(ip, use_proxy = True):
    '''
    Set proxy
    '''
    handler = urllib2.ProxyHandler({"http":ip})
    null_handler = urllib2.ProxyHandler({})
    if use_proxy:
        opener = urllib2.build_opener(handler)
    else:
        opener = urllib2.build_opener(null_handler)
    urllib2.install_opener(opener)


def crawl(urls):
    '''
    Crawl the webpages of the urls
    '''
    my_proxy = get_proxy()
    n = len(my_proxy)
    proxy_index = 0
    set_proxy(my_proxy[proxy_index], False)
    myheaders = {"User-Agent": "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)"}
    start = False
    stack = urls
    stack.reverse()
    while len(stack) > 0:
        line = stack.pop()
        cur_url = line.split('\t')[-1]
        filename = cur_url.split('/')[-1]+".html"
        #if filename == '2252333.html':
        #    start = True
        #if not start:
        #    continue
        print cur_url
        try:
            f = open(filename,'w')
            req = urllib2.Request(cur_url, headers=myheaders)
            data = urllib2.urlopen(req, timeout=20).read()
            f.write(data)
            f.close()
            time.sleep(1)
        except urllib2.HTTPError, e:
            print "Error Code:", e.code
            stack.append(line)
            if proxy_index < n:
                proxy_index += 1
                set_proxy(my_proxy[proxy_index])
            else:
                break
        except urllib2.URLError, e:
            print "Error Reason:",e.reason
            stack.append(line)
            if proxy_index < n:
                proxy_index += 1
                set_proxy(my_proxy[proxy_index])
            else:
                break
        except:
            print "Error",filename
            break
if __name__ == '__main__':
    urls = []
    for line in file(sys.argv[1]):
        urls.append(line.strip())
    crawl(urls)
