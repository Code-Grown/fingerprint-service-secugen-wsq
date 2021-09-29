#!/usr/local/bin/python
# -*- coding: utf-8 -*-
from typing import AnyStr
from datetime import datetime
from pytz import timezone
from contextlib import closing

import sys

sys.stdout.flush()


# from logging import FileHandler, WARNING

class Log:

    def __init__(self):
        pass

    @staticmethod
    def debug(text: AnyStr) -> None: Log.write_log('DEBUG', 'debug.log', text)

    @staticmethod
    def info(text: AnyStr) -> None: Log.write_log('INFO', 'info.log', text)

    @staticmethod
    def success(text: AnyStr) -> None: Log.write_log('SUCCESS', 'success.log', text)

    @staticmethod
    def warning(text: AnyStr) -> None: Log.write_log('WARNING', 'warning.log', text)

    @staticmethod
    def danger(text: AnyStr) -> None: Log.write_log('DANGER', 'danger.log', text)

    @staticmethod
    def critical(text: AnyStr) -> None: Log.write_log('CRITICAL', 'critical.log', text)

    @staticmethod
    def write_log(logtype: AnyStr, filename: AnyStr, text: AnyStr) -> None:
        date = datetime.now(timezone('America/Santiago'))
        frmt = "%d/%m/%Y %H:%M:%S"
        with closing(open(f'/main/storage/logs/app.log', 'a')) as l:
            l.write(
                '[%-10s] - [%-5s] - %-10s \n' %
                (date.strftime(frmt), logtype, text)
            )
        with closing(open(f'/main/storage/logs/{filename}', 'a')) as l:
            l.write(
                '[%-10s] - [%-5s] - %-10s \n' %
                (date.strftime(frmt), logtype, text)
            )
