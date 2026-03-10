
from .main import *

from email.utils import parseaddr as _parseaddr
from importlib.metadata import metadata as _metadata

_meta = _metadata("blnuhr")

__version__ = _meta["Version"]
__package_name__ = _meta["Name"]
__description__ = _meta["Summary"]
__author_name__, __author_email__ = _parseaddr(_meta["Author-email"])
__author__ = __author_name__
__email__ = __author_email__

# not represented in standard package metadata
__institution__ = "Late-Nite(tm) Software"
__copyright__ = "2009-2026, Pete R. Jemian"

# as shown in the About box ...
__credits__ = "\n".join([
    f"author: {__author__}",
    f"email: {__email__}",
    f"institution: {__institution__}",
])
