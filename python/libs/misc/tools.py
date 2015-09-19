#!/usr/bin/python3
# -*- coding:utf-8 -*-
import sys,os
import types	
import inspect
import linecache
import signal,functools

def info(obj,words_perline=4,width=20):
	def print_info(string,lists):
		string = string.ljust(width)
		space_width = len(string)
		list_len = len(lists)
		if list_len <= words_perline:
			print(string,end='')
			print(lists)
		else:
			spaces = ' ' * space_width	
			print(string,end='')
			print(lists[0:words_perline])
			i = words_perline
			while (i + words_perline) < list_len:
				print(spaces,end='')
				print(lists[i:i + words_perline])
				i += words_perline
			print(spaces,end='')
			print(lists[i:])
	functions = get_function(obj)
	modules = get_module(obj)
	classes = get_class(obj)

	if functions:
		print_info('functions:',functions)
	if modules:
		print_info('module:',modules)
	if classes:
		print_info('classes:',classes)

__target__ = {
				'function':types.FunctionType,
			    'module':types.ModuleType,
			 }

def __get_target__(obj,target):
	return [name for name in dir(obj) if callable(getattr(obj,name)) and not\
			name.startswith('_') and isinstance(getattr(obj,name),__target__[target])]

def get_class(obj):
	return [name for name in dir(obj) if inspect.isclass(getattr(obj,name))\
	 		 and not name.startswith('_')]

def get_module(obj):
	return __get_target__(obj,'module')

def get_function(obj):
	return __get_target__(obj,'function')

class TimeoutError(Exception):pass

def timeout(seconds,error_message = 'Function call timed out'):
	def _timeout(func):
		def _handle_timeout(signum,frame):
			raise TimeoutError(error_message)
		@functools.wraps(func)
		def wrapper(*args,**kw):
			signal.signal(signal.SIGALRM,_handle_timeout)
			signal.alarm(seconds)
			try:
				result = func(*args,**kw)
			finally:
				signal.alarm(0)
			return result
		return wrapper
	return _timeout

def trace(f):
	def globaltrace(frame,why,arg):
		if why == 'call':return localtrace
		return None
	def localtrace(frame,why,arg):
		if why == 'line':
			filename = frame.f_code.co_filename
			lineno = frame.f_lineno
			bname = os.path.basename(filename)
			print("{}({}):{}".format(bname,lineno,linecache.getline(filename,lineno)))
		return localtrace
	def _trace(*args,**kw):
		sys.settrace(globaltrace)
		result = f(*args,**kw)
		sys.settrace(None)
		return result
	return _trace

