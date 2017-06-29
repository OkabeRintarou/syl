#!/usr/bin/python3
# -*- coding:utf-8 -*-

miss = object()

class _const:
	class ConstError(TypeError):pass

	def __setattr__(self,name,value):
		if self.__dict__.get(name,miss) != miss:
			raise self.ConstError("Can't rebind const(%s)" % name)
		self.__dict__[name] = value
import sys
sys.modules[__name__] = _const()
