'''
Created on Jan 8, 2016

@author: Pete
'''

import datetime
import os
import sys
import time
on_rtd = os.environ.get('READTHEDOCS', None) == 'True'
if on_rtd:
    from mock_PyQt4 import QtCore, QtGui
else:
    from PyQt4 import QtCore, QtGui
import resources


CLOCK_UI_FILE = 'blnuhr.ui'
ORANGE = {0:'background-color: rgb(130, 85, 64);\ncolor: rgb(130, 85, 64);',
          1:'background-color: rgb(255, 170, 127);\ncolor: rgb(255, 170, 127);',
         }
RED = {0:'background-color: rgb(128, 42, 64);',
       1:'background-color: rgb(255, 85, 127);',
      }
REFRESH_TIME__MS = 100


class Clock_blnuhr(QtGui.QWidget):
    '''create a GUI for the clock and start it running'''

    def __init__(self, parent=None):
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
        t = t or datetime.datetime.now()
        
        # seconds blinker
        self.l_s.setStyleSheet(str(ORANGE[t.second % 2]))
        self.l_s.setToolTip(str(t))

        # 5-hour markers
        if self.last_t is None or self.last_t.hour != t.hour: 
            self.l_h5_1.setStyleSheet(str(RED[t.hour > 4]))
            self.l_h5_2.setStyleSheet(str(RED[t.hour > 9]))
            self.l_h5_3.setStyleSheet(str(RED[t.hour > 14]))
            self.l_h5_4.setStyleSheet(str(RED[t.hour > 19]))

            # 1-hour markers
            h = t.hour % 5
            self.l_h1_1.setStyleSheet(str(RED[h > 0]))
            self.l_h1_2.setStyleSheet(str(RED[h > 1]))
            self.l_h1_3.setStyleSheet(str(RED[h > 2]))
            self.l_h1_4.setStyleSheet(str(RED[h > 3]))
        
        # 5-minute markers
        if self.last_t is None or self.last_t.minute != t.minute: 
            self.l_m5_1.setStyleSheet(str(ORANGE[t.minute > 4]))
            self.l_m5_2.setStyleSheet(str(ORANGE[t.minute > 9]))
            self.l_m5_3.setStyleSheet(str(RED[t.minute > 14]))
            self.l_m5_4.setStyleSheet(str(ORANGE[t.minute > 19]))
            self.l_m5_5.setStyleSheet(str(ORANGE[t.minute > 24]))
            self.l_m5_6.setStyleSheet(str(RED[t.minute > 29]))
            self.l_m5_7.setStyleSheet(str(ORANGE[t.minute > 34]))
            self.l_m5_8.setStyleSheet(str(ORANGE[t.minute > 39]))
            self.l_m5_9.setStyleSheet(str(RED[t.minute > 44]))
            self.l_m5_10.setStyleSheet(str(ORANGE[t.minute > 49]))
            self.l_m5_11.setStyleSheet(str(ORANGE[t.minute > 54]))
    
            # 1-minute markers
            m = t.minute % 5
            self.l_m1_1.setStyleSheet(str(ORANGE[m > 0]))
            self.l_m1_2.setStyleSheet(str(ORANGE[m > 1]))
            self.l_m1_3.setStyleSheet(str(ORANGE[m > 2]))
            self.l_m1_4.setStyleSheet(str(ORANGE[m > 3]))
        
        # for next time
        self.last_t = t


def main ():
    app = QtGui.QApplication(sys.argv)
    clock = Clock_blnuhr()
    clock.show()
    clock.start()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
