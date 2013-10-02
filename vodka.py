#!/usr/bin/env python3

import lucifer
import json
from html2text import html2text
from pprint import pprint
import os
import time


from bottlex import route, run, Bottle, view, SimpleTemplate, static_file
from bottlex import get, post, request # or route




INDEX_PATH = "idx.bin"
METADATA_PATH = "meta.json"

def load_all_metainfo(load_from):
    with open(load_from, 'r') as r:
        return json.loads(r.read())


metadata = load_all_metainfo(METADATA_PATH)


app = Bottle()


@app.post('/search') # or @route('/login', method='POST')
@view('hello.tpl')
def do_login():
    query = request.forms.get('query')
    res = list(metadata.values())[:3]
    for x in res:
    	print(x.get('title'))

    # !!!
    hh = dict(  paginator = {'page': 1,
    	                     'total_pages': 3},
    	        posts = res)

    	 #        [{'title': "qwe",
						# 'author': "ttt",
						# 'creators': ['xxx1', 'xx2x'],
						# 'cover': None,
						# 'url': None,
						# 'perface': None,
						# 'book_path': 'bbb1'},
					 #   {'title': "qwe",
						# 'author': "ttt",
						# 'cover': None,
						# 'url': None,
						# 'perface': None,
						# 'creators': ["xxx2"],
						# 'book_path': 'bbb2'}
      #        	 ])
    return hh




@app.route('/helloxxx')
def hello1():
    return "Hello World!"


@app.get('/search')
@view('hello.tpl')
def hello(name='World'):
	return {}
    # return "<p>Your login information was correct.</p>"


@app.route('/searchxxx')
def tesxts(name='World'):
	return dict(name='World')

@app.route('/assets/<filename>')
def send_image(filename):
    return static_file(filename, root='./assets/')


def main():
	lucifer.loadIndex(INDEX_PATH)
	run(app, host='localhost', port=8080, debug=True)





main()







