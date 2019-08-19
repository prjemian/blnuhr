
from .main import *


__package_name__        = u'blnuhr'
__description__         = u"Python & Qt rendition of Berlin's quantity didactics clock"
__long_description__    = __description__

__author__              = u'Pete R. Jemian'
__email__               = u'prjemian@gmail.com'
__institution__         = u"Late-Nite(tm) Software"
__author_name__         = __author__
__author_email__        = __email__

__copyright__           = u'2016-2019, Pete R. Jemian'
# __license_url__         = u''
__license__             = u'OPEN SOURCE LICENSE'
__url__                 = u'https://github.com/prjemian/blnuhr/'
__download_url__        = __url__
__keywords__            = ['Python', 'Qt5', 'clock']
__requires__            = []

__classifiers__ = [
            'Development Status :: 4 - Beta',
            'Environment :: Console',
            'Environment :: Web Environment',
            'Intended Audience :: Science/Research',
            'License :: Free To Use But Restricted',
            'Operating System :: OS Independent',
            'Programming Language :: Python',
            'Topic :: Software Development :: Libraries :: Python Modules',
            'Topic :: Utilities',
                     ]

# as shown in the About box ...
__credits__ = u'author: ' + __author__
__credits__ += u'\nemail: ' + __email__
__credits__ += u'\ninstitution: ' + __institution__
__credits__ += u'\nURL: ' + __url__

from ._version import get_versions
__version__ = get_versions()['version']
del get_versions
