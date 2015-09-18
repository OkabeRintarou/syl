#! /usr/bin/python3
# -*- coding:utf -*-

import urllib.request as request
import urllib.parse
import urllib.error
import html.parser

__global_user_agent__ = 'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36'

class QiuShiHTMLParser(html.parser.HTMLParser):
	def __init__(self,page,url):
		self.current_page = page
		self.next_page_url = url
		self.in_div = False
		self.in_current = False
		self.in_next_page = False
		self.count = 1
		html.parser.HTMLParser.__init__(self)
	def handle_starttag(self,tag,attrs):
		if tag == 'div' and len(attrs) == 1 and\
				attrs[0] == ('class','content'):
			self.in_div = True
		if tag == 'a' and len(attrs) == 1 and\
				attrs[0] == ('class','current'):
			self.in_current = True
		if tag == 'a' and len(attrs) == 1 and self.in_next_page and\
				attrs[0][0] == 'href':
			index = self.next_page_url.index('.com') + 4
			self.next_page_url = self.next_page_url[0:index]
			self.next_page_url += attrs[0][1]
			self.in_next_page = False
	def handle_data(self,data):
		if self.in_div:
			print(str(self.count) + ':' + data)
			self.in_div = False
			self.count += 1
		elif self.in_current:
			self.in_current = False
			self.in_next_page = True
	def get_next_page_url(self):
		return self.next_page_url
	def feed(self,data):
		html.parser.HTMLParser.feed(self,data)
		return self.next_page_url

class QiuShiCrawl():
	def __init__(self):
		self.url = 'http://www.qiushibaike.com/8hr/page/1'
		self.decode = 'utf-8'
		self.current_page = 1

	def __crawl_url__(self,url):
		usger_agent  = __global_user_agent__
		headers = {'User-Agent':usger_agent}
		req = request.Request(url,headers = headers)
		req = request.urlopen(req)
		result = req.read()
		result = result.decode(self.decode)
		parser = QiuShiHTMLParser(self.current_page,self.url)
		next_page_url = parser.feed(result)
		return next_page_url

	def crawl(self):
		url = self.__crawl_url__(self.url)
		while True:
			print('输入Enter键浏览下一页:')
			input()
			url = self. __crawl_url__(url)

if __name__ == '__main__':
	crawl = QiuShiCrawl()
	crawl.crawl()
