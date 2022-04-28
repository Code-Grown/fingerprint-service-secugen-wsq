#!/usr/local/bin/python
# -*- coding: utf-8 -*-
from datetime import datetime


class Profiling:
    ini_time = None
    end_time = None

    def __init__(self):
        pass

    def init_profiling(self):
        self.ini_time = datetime.now()
        self.end_time = ""

    def end_profiling(self):
        self.end_time = datetime.now()
        return str(round((self.end_time - self.ini_time).total_seconds()))
