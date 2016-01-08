'''
This module is *only* for building the documentation at readthedocs.org
'''

class QMockObject(object):
        def __init__(self, *args, **kwargs):
            super(QMockObject, self).__init__()

        def __call__(self, *args, **kwargs):
            return None

class QApplication(QMockObject): pass
class QCheckBox(QMockObject): pass
class QColor(QMockObject): pass
class QDesktopServices(QMockObject): pass
class QDialog(QMockObject): pass
class QFileDialog(QMockObject): pass
class QGridLayout(QMockObject): pass
class QGroupBox(QMockObject): pass
class QInputDialog(QMockObject): pass
class QLabel(QMockObject): pass
class QLineEdit(QMockObject): pass
class QMainWindow(QMockObject): pass
class QMessageBox(QMockObject): pass
class QPalette(QMockObject): pass
class QPlainTextEdit(QMockObject): pass
class QSlider(QMockObject): pass
class QWidget(QMockObject): pass
