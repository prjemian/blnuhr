'''
Created on Jan 8, 2016

@author: Pete
'''

import datetime
import time
import sys
from PyQt4 import QtGui, QtCore
import form_support


CLOCK_FORM = 'blnuhr.ui'
ORANGE = {0:'background-color: rgb(130, 85, 64);\ncolor: rgb(130, 85, 64);',
          1:'background-color: rgb(255, 170, 127);\ncolor: rgb(255, 170, 127);',
         }
RED = {0:'background-color: rgb(128, 42, 64);',
       1:'background-color: rgb(255, 85, 127);',
      }


class Clock_blnuhr(object):

    def __init__(self, parent=None):
        self.ui = form_support.load_form(CLOCK_FORM)
        self.display_time(None)
    
    def display_time(self, t=None):
        t = t or datetime.datetime.now()
        
        # seconds blinker
        self.ui.l_s.setStyleSheet(str(ORANGE[t.second % 2]))

        # 5-hour markers
        self.ui.l_h5_1.setStyleSheet(str(RED[t.hour > 4]))
        self.ui.l_h5_2.setStyleSheet(str(RED[t.hour > 9]))
        self.ui.l_h5_3.setStyleSheet(str(RED[t.hour > 14]))
        self.ui.l_h5_4.setStyleSheet(str(RED[t.hour > 19]))

        # 1-hour markers
        h = t.hour % 5
        self.ui.l_h1_1.setStyleSheet(str(RED[h > 0]))
        self.ui.l_h1_2.setStyleSheet(str(RED[h > 1]))
        self.ui.l_h1_3.setStyleSheet(str(RED[h > 2]))
        self.ui.l_h1_4.setStyleSheet(str(RED[h > 3]))
        
        # 5-minute markers
        self.ui.l_m5_1.setStyleSheet(str(ORANGE[t.minute > 4]))
        self.ui.l_m5_2.setStyleSheet(str(ORANGE[t.minute > 9]))
        self.ui.l_m5_3.setStyleSheet(str(RED[t.minute > 14]))
        self.ui.l_m5_4.setStyleSheet(str(ORANGE[t.minute > 19]))
        self.ui.l_m5_5.setStyleSheet(str(ORANGE[t.minute > 24]))
        self.ui.l_m5_6.setStyleSheet(str(RED[t.minute > 29]))
        self.ui.l_m5_7.setStyleSheet(str(ORANGE[t.minute > 34]))
        self.ui.l_m5_8.setStyleSheet(str(ORANGE[t.minute > 39]))
        self.ui.l_m5_9.setStyleSheet(str(RED[t.minute > 44]))
        self.ui.l_m5_10.setStyleSheet(str(ORANGE[t.minute > 49]))
        self.ui.l_m5_11.setStyleSheet(str(ORANGE[t.minute > 54]))

        # 1-minute markers
        m = t.minute % 5
        self.ui.l_m1_1.setStyleSheet(str(ORANGE[m > 0]))
        self.ui.l_m1_2.setStyleSheet(str(ORANGE[m > 1]))
        self.ui.l_m1_3.setStyleSheet(str(ORANGE[m > 2]))
        self.ui.l_m1_4.setStyleSheet(str(ORANGE[m > 3]))


def main ():
    app = QtGui.QApplication(sys.argv)
    main_window = Clock_blnuhr()
    main_window.ui.show()
    sys.exit(app.exec_())


if __name__ == '__main__':
    print time.time()
    print datetime.datetime.now()
    main()
