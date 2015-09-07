#!/usr/bin/python3
# -*- coding:utf-8 -*-
import types	
import inspect

def info(obj,words_perline=4,width=20):
	def print_info(string,lists):
		string = string.ljust(width)
		space_width = len(string)
		list_len = len(lists)
		if list_len <= words_perline:
			print(string,lists)
		else:
			spaces = ' ' * space_width	
			print(string,lists[0:words_perline])
			i = words_perline
			while (i << 1) < list_len:
				print(spaces,lists[i:i << 1])
				i <<= 1
			print(spaces,[lists[i:]])
	functions = get_function(obj)
	modules = get_module(obj)
	classes = get_class(obj)

	if functions:
		print('functions:')
	if modules:
		print('module:')
	if classes:
		print('classes:')

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
