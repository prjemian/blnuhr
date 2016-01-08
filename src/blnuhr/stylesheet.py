

class StyleSheet(object):
    '''
    manage style sheet settings for a Qt widget
    
    Example::

        widget = QtGui.QLabel('example label')
        sty = bcdaqwidgets.StyleSheet(widget)
        sty.updateStyleSheet({
            'font': 'bold',
            'color': 'white',
            'background-color': 'dodgerblue',
            'qproperty-alignment': 'AlignCenter',
        })
    
    '''

    def __init__(self, widget, sty={}):
        '''
        :param obj widget: the Qt widget on which to apply the style sheet
        :param dict sty: starting dictionary of style sheet settings
        '''
        self.widget = widget
        widgetclass = str(type(widget)).strip('>').split('.')[-1].strip("'")
        self.widgetclass = widgetclass
        self.style_cache = dict(sty)

    def clearCache(self):
        '''clear the internal cache'''
        self.style_cache = {}

    def updateStyleSheet(self, sty={}):
        '''change specified styles and apply all to widget'''
        self._updateCache(sty)
        if self.widget is not None:
            self.widget.setStyleSheet(str(self))

    def _updateCache(self, sty={}):
        '''update internal cache with specified styles'''
        for key, value in sty.items():
            self.style_cache[key] = value

    def __str__(self):
        '''returns a CSS text with the cache settings'''
        s = self.widgetclass + ' {\n'
        for key, value in sorted(self.style_cache.items()):
            s += '    %s: %s;\n' % (key, value)
        s += '}'
        return s
