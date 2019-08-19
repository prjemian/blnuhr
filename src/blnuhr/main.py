# Copyright (c) 2009 - 2019, Pete Jemian.
# See LICENSE file for details.

import datetime
import sys
from PyQt5 import QtCore, QtWidgets
from . import resources


CLOCK_UI_FILE = 'blnuhr.ui'
ORANGE = {0:'''background-color: rgb(130, 85, 64);
               color: rgb(130, 85, 64);''',
          1:'''background-color: rgb(255, 170, 127);
               color: rgb(255, 170, 127);''',
         }
RED = {0:'background-color: rgb(128, 42, 64);',
       1:'background-color: rgb(255, 85, 127);',
      }
REFRESH_TIME__MS = 100


class Clock_blnuhr(QtWidgets.QWidget):
    '''create a widget for the clock and start it running'''

    def __init__(self, **_kwargs):
        QtWidgets.QWidget.__init__(self)
        resources.loadUi(CLOCK_UI_FILE, self)
        self.last_t = None
        self.update(None)
    
    def start(self):
        '''begin the periodic update of the clock'''
        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.update)
        self.timer.start(REFRESH_TIME__MS)
    
    def update(self, t=None):
        '''
        manage a periodic update of the clock
        
        Show the time as a string on the seconds LED as a tool tip
        '''
        def set_LIGHT(w, color, choice):
            w.setStyleSheet(str(color[choice]))

        t = t or datetime.datetime.now()
        
        # seconds blinker
        set_LIGHT(self.l_s, ORANGE, int(t.microsecond / 500000))
        self.l_s.setToolTip(str(t))
        color_scheme = {0: ORANGE, 1: ORANGE, 2: RED}

        # 5-hour markers
        if self.last_t is None or self.last_t.hour != t.hour: 
            for n in range(4):
                label = "l_h5_{}".format(n+1)
                qlabel = self.__getattribute__(label)
                set_LIGHT(qlabel, RED, t.hour > 5*n+4)

            # 1-hour markers
            h = t.hour % 5
            for n in range(4):
                label = "l_h1_{}".format(n+1)
                qlabel = self.__getattribute__(label)
                set_LIGHT(qlabel, ORANGE, h > n)
        
        # 5-minute markers
        if self.last_t is None or self.last_t.minute != t.minute: 
            for n in range(11):
                label = "l_m5_{}".format(n+1)
                qlabel = self.__getattribute__(label)
                color = color_scheme[n % 3]
                set_LIGHT(qlabel, color, t.minute > 5*n+4)
    
            # 1-minute markers
            m = t.minute % 5
            for n in range(4):
                label = "l_m1_{}".format(n+1)
                qlabel = self.__getattribute__(label)
                set_LIGHT(qlabel, ORANGE, m > n)
        
        # 5-second markers
        if self.last_t is None or self.last_t.second != t.second: 
            for n in range(11):
                label = "l_s5_{}".format(n+1)
                qlabel = self.__getattribute__(label)
                color = color_scheme[n % 3]
                set_LIGHT(qlabel, color, t.second > 5*n+4)
    
            # 1-second markers
            s = t.second % 5
            for n in range(4):
                label = "l_s1_{}".format(n+1)
                qlabel = self.__getattribute__(label)
                set_LIGHT(qlabel, ORANGE, s > n)
        
        # for next time
        self.last_t = t


def kopf():
    '''entry point to run standalone'''
    app = QtWidgets.QApplication(sys.argv)
    clock = Clock_blnuhr()
    clock.show()
    clock.start()
    sys.exit(app.exec_())


if __name__ == '__main__':
    kopf()
