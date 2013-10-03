#!/usr/bin/env python3

import lucifer
import json
from html2text import html2text
from pprint import pprint
import os
import time
import yaml

INDEX_PATH = "idx.bin"
METADATA_PATH = "meta.json"
POSTS_PATH = "/Users/oleg/design/bookmine/_posts"


def parse_all_metainfo(posts_path, save_to=""):
    res = {}
    _,_,files = list(os.walk(posts_path))[0]
    for fp in files:
        if fp.endswith(".html"):
            curr_path = os.path.join(posts_path, fp)
            print(curr_path)
            with open(curr_path, 'r') as r:
                xx = list(r.readlines())
                zz = xx[1:xx.index('---\n', 1)]
                data = yaml.load(''.join(zz))
                if "book_date" in data:
                    del(data["book_date"])
                res[data.get('book_path', 'undefined')] = data
    if save_to:
        with open(save_to, 'w') as w:
            w.write(json.dumps(res))
    return res


def load_all_metainfo(load_from):
    with open(load_from, 'r') as r:
        return json.loads(r.read())



def find_id(document):
    pattern = 'name="TOC_id'
    s1 = document.find(pattern)
    if s1 == -1:
        return "undefined"
    s2 = document.find('"', s1 + len(pattern))
    return document[s1 + len('name="') : s2]


def split_text(data):
    text_lines = data.split('\n')
    sep = [i for i,ll in enumerate(text_lines) if "calibre_toc" in ll]
    sep.append(len(text_lines))
    for begin,end in zip(sep, sep[1:]):
        document = "\n".join(text_lines[begin:end])
        _id = find_id(document)
        yield _id, html2text(document)


def get_header(doc):
    if doc.startswith("## "):
        s1 = doc.find('\n')
        s2 = doc.find('\n', s1+1)
        return doc[2:s2].replace('\n', ' ').strip()
    elif doc.startswith("# "):
        return doc[1:doc.find('\n')].strip()
    else:
        return doc[:doc.find('\n')].strip()


def get_url(text):
    pattern = "book_path: "
    s1 = text.find(pattern)
    s2 = text.find("\n", s1 + len(pattern))
    return text[s1 + len(pattern) : s2]


def proc_file(path):
    f = open(path)
    txt1 = f.read()
    f.close()
    url = get_url(txt1)
    i = 0
    for _id, doc in split_text(txt1):
        chapter_id = url + "index.xhtml#" + _id
        hh = get_header(doc)
        # print(chapter_id)
        # print(">> ",  hh)
        lucifer.addDocument(chapter_id, doc)
        i += 1
    return i


def mk_index():
    # proc_file("../kalinin/index1.xhtml")
    # proc_file("../kalinin/index2.xhtml")
    # proc_file("../kalinin/index3.xhtml")
    i = 0
    books_path = "/Users/oleg/design/bookmine/books"
    for pref,_,files in os.walk(books_path):
        for fp in files:
            if fp.endswith(".xhtml"):
                curr_path = os.path.join(pref, fp)
                print(curr_path)
                i+=1
                proc_file(curr_path)
    print("i =", i)
    # lucifer.addDocument("OLOLO1", "Goethe saw the sea for the first time in his life when he came   ")

    # for x in ss.split():
    #     lucifer.showIndex(x)

    lucifer.saveIndex(INDEX_PATH)


def main():
    # zz = parse_all_metainfo(POSTS_PATH, save_to=METADATA_PATH)
    xx = load_all_metainfo(METADATA_PATH)
    mk_index()
    return

    lucifer.loadIndex(INDEX_PATH)

    ss = "Goethe saw the sea for the first time in his life when he"
    raw_res = lucifer.searchQuery(ss, 17, 1)
    print(raw_res)
    # res = json.loads(raw_res)
    # trunc = sorted(res.items(), key= lambda x: x[1], reverse=True)[:10]
    # pprint(trunc)
    # print("res size =", len(res))
    # print("index size = %d kB" % (os.path.getsize(INDEX_PATH) // 1024))
    # return



    return

    lucifer.addDocument(doc2)
    lucifer.addDocument(doc3)
    lucifer.showIndex("c++")
    lucifer.showIndex("garbage")


    # res = lucifer.saveIndex("kakao")
    # res = lucifer.addDocument(",.. ,.kakao,.. ---ololo xx")
    # res = 0
    # print(type(res), res)

    # for ll in text.split('\n'):
    #   lucifer.addDocument(ll)



main()











