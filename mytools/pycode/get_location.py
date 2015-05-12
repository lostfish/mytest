#!/usr/bin/env python
# encoding: utf-8

import sys
from collections import defaultdict
import marisa_trie
from langconv import *

conv = Converter('zh-hans') #繁体转简体

class LocationInfo:
    def __init__(self, id, name,  encode_num, level, parent):
        self.id = id
        self.name = name
        self.encode_num = encode_num
        self.level = level
        self.parent = parent
        self.parent2 = "" #一级地域name
    def __str__(self):
        return "%s\t%s\t%d\t%s\t%d" % (self.id, self.name, self.encode_num, self.parent, self.level)

class LocationFinder:
    def __init__(self, filepath):
        self.location_dict = self.read_location_file(filepath) #name -> LocationInfo
        self.mirror_dict = {}
        for v in self.location_dict.values():
            self.mirror_dict[v.id] = v
        words = [ key.decode('utf-8') for key in self.location_dict]
        self.trie = marisa_trie.Trie(words)
    def get_location_info(self, loc_id):
        return self.mirror_dict[loc_id]
    def read_location_file(self, filepath):
        location_dict = {}
        with open(filepath) as f:
            for line in f:
                s = line.strip().split('\t')
                if len(s) != 5:
                    print "Format error: " % line,
                name = s[0]
                id = s[1]
                encode = int(s[2])
                parent = s[3]
                level = int(s[4])
                if name in location_dict:
                    print "存在同名地名:",name
                    if level > location_dict[name]:
                        continue
                location_dict[name] = LocationInfo(id, name, encode, level, parent)
        id_name = {}
        for name in location_dict:
            id_name[location_dict[name].id] = name
        for k in location_dict:
            p = location_dict[k].parent
            if p:
                location_dict[k].parent = id_name[p] #id转成name
        for k in location_dict:
            if location_dict[k].level == 3:
                p = location_dict[k].parent
                location_dict[k].parent2 = location_dict[p].parent
        return location_dict

    def find_location(self, content):
        hits = []
        content = conv.convert(content.lower().decode('utf-8')) #规范化
        for i in range(len(content)):
            a = self.trie.prefixes(content[i:])
            hits.extend(a)
        if len(hits) == 0:
            return []
        locs = set()
        for loc_name in (hits):
            key = loc_name.encode('utf-8')
            locs.add(key)
            p = self.location_dict[key].parent
            p2 = self.location_dict[key].parent2
            if p:
                locs.add(p)
            if p2:
                locs.add(p2)
        result = [self.location_dict[w] for w in locs]
        #TODO:只返回唯一的1、2、3级地域元组
        count = [0 for i in range(4)]
        for info in result:
            count[info.level] += 1
        if count[1] > 1 or count[2] > 1 or count[3] > 1:
            return []
        return result

    def print_location(self):
        for k,v in self.location_dict.items():
            print str(v)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print "Usage: %s [location_info]" % __file__
        sys.exit(-1)
    finder =  LocationFinder(sys.argv[1])
    #finder.print_location()
    content = "禁止学生在教室、宿舍给手机、充电宝等充电，发现一律没收！这是攀枝花市经贸旅游学校的规定，称是为了安全和让学生专心学习。禁止学生充电，校内却有充电点，充电一次1至2元。学校称这是临聘宿管员私下所为。你怎么看？"
    result = finder.find_location(content)
    for loc in result:
        print str(loc)
    print finder.get_location_info("1042015:city_20510004")
