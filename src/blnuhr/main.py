# Copyright (c) 2009 - 2016, Pete Jemian.
# See LICENSE file for details.

import datetime
import os
import sys
from PyQt4 import QtCore, QtGui
import resources


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


class Clock_blnuhr(QtGui.QWidget):
    '''create a widget for the clock and start it running'''

    def __init__(self, **_kwargs):
        QtGui.QWidget.__init__(self)
        resources.loadUi(CLOCK_UI_FILE, self)
        self.last_t = None
        self.update(None)
    
    def start(self):
        '''begin the periodic update of the clock'''
        self.timer = QtCore.QTimer(self)
        QtCore.QObject.connect(self.timer, 
                               QtCore.SIGNAL('timeout()'), 
                               self.update)
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
        set_LIGHT(self.l_s, ORANGE, t.second % 2)
        self.l_s.setToolTip(str(t))

        # 5-hour markers
        if self.last_t is None or self.last_t.hour != t.hour: 
            set_LIGHT(self.l_h5_1, RED, t.hour > 4)
            set_LIGHT(self.l_h5_2, RED, t.hour > 9)
            set_LIGHT(self.l_h5_3, RED, t.hour > 14)
            set_LIGHT(self.l_h5_4, RED, t.hour > 19)

            # 1-hour markers
            h = t.hour % 5
            set_LIGHT(self.l_h1_1, RED, h > 0)
            set_LIGHT(self.l_h1_2, RED, h > 1)
            set_LIGHT(self.l_h1_3, RED, h > 2)
            set_LIGHT(self.l_h1_4, RED, h > 3)
        
        # 5-minute markers
        if self.last_t is None or self.last_t.minute != t.minute: 
            set_LIGHT(self.l_m5_1,  ORANGE, t.minute > 4)
            set_LIGHT(self.l_m5_2,  ORANGE, t.minute > 9)
            set_LIGHT(self.l_m5_3,  RED,    t.minute > 14)
            set_LIGHT(self.l_m5_4,  ORANGE, t.minute > 19)
            set_LIGHT(self.l_m5_5,  ORANGE, t.minute > 24)
            set_LIGHT(self.l_m5_6,  RED,    t.minute > 29)
            set_LIGHT(self.l_m5_7,  ORANGE, t.minute > 34)
            set_LIGHT(self.l_m5_8,  ORANGE, t.minute > 39)
            set_LIGHT(self.l_m5_9,  RED,    t.minute > 44)
            set_LIGHT(self.l_m5_10, ORANGE, t.minute > 49)
            set_LIGHT(self.l_m5_11, ORANGE, t.minute > 54)
    
            # 1-minute markers
            m = t.minute % 5
            set_LIGHT(self.l_m1_1, ORANGE, m > 0)
            set_LIGHT(self.l_m1_2, ORANGE, m > 1)
            set_LIGHT(self.l_m1_3, ORANGE, m > 2)
            set_LIGHT(self.l_m1_4, ORANGE, m > 3)
        
        # 5-second markers
        if self.last_t is None or self.last_t.second != t.second: 
            set_LIGHT(self.l_s5_1,  ORANGE, t.second > 4)
            set_LIGHT(self.l_s5_2,  ORANGE, t.second > 9)
            set_LIGHT(self.l_s5_3,  RED,    t.second > 14)
            set_LIGHT(self.l_s5_4,  ORANGE, t.second > 19)
            set_LIGHT(self.l_s5_5,  ORANGE, t.second > 24)
            set_LIGHT(self.l_s5_6,  RED,    t.second > 29)
            set_LIGHT(self.l_s5_7,  ORANGE, t.second > 34)
            set_LIGHT(self.l_s5_8,  ORANGE, t.second > 39)
            set_LIGHT(self.l_s5_9,  RED,    t.second > 44)
            set_LIGHT(self.l_s5_10, ORANGE, t.second > 49)
            set_LIGHT(self.l_s5_11, ORANGE, t.second > 54)
    
            # 1-second markers
            s = t.second % 5
            set_LIGHT(self.l_s1_1, ORANGE, s > 0)
            set_LIGHT(self.l_s1_2, ORANGE, s > 1)
            set_LIGHT(self.l_s1_3, ORANGE, s > 2)
            set_LIGHT(self.l_s1_4, ORANGE, s > 3)
        
        # for next time
        self.last_t = t


def main ():
    '''entry point to run standalone'''
    app = QtGui.QApplication(sys.argv)
    clock = Clock_blnuhr()
    clock.show()
    clock.start()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
