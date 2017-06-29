#!/usr/bin/python3
# -*- coding:utf-8 -*-

import time
import inspect
from functools import wraps

def get_line_number():
	return inspect.currentframe().f_back.f_lineno


def logger(loglevel):
	def log_decorator(func):
		@wraps(func)
		def wrapper(*args,**kw):
			ts = time.time()
			result = func(*args,**kw)
			te = time.time()
			print("     function = " + func.__name__)
			print("    arguments = {0} {1}".format(args, kwargs))
            print("    return    = {0}".format(result))
            print("    time      = %.6f sec" % (te-ts))
            if loglevel == 'debug':
            	print("called from line:" + str(get_line_number()))
            return result
        return wrapper
    return log_decorator

