#! /usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os, array

def PrintUsage():
    executable = os.path.basename(__file__)
    print u'\nbin2header version 0.0.1 (python)\n2011 Jordan Irwin <antumdeluge@gmail.com>\n\n\tUsage:\t{} file\n'.format(executable)

def Bin2Header(argv):
    if not os.path.isfile(argv[1]):
        print u'\nFile "{}" does not exist'.format(argv[1])
        PrintUsage()
        sys.exit(1)
    
    ### Get Filenames and Path ###
    path = argv[1]
    filename = list(os.path.basename(path))
    hname = list(filename)
    
    ### Remove Unwanted Characters ###
    badchars = (u'\\', u'+', u'-', u'*', u' ')
    for x in range(len(hname)):
        if hname[x] in badchars or hname[x] == u'.':
            hname[x] = '_'
        if filename[x] in badchars:
            filename[x] = '_'
    
    filename = u''.join(filename)
    filename += u'.h'
    hname = u''.join(hname)
    
    ### Uppercase Name for Header ###
    hname_upper = hname.upper()
    hname_upper += u'_H'
    
    ### Read Data In ###
    data = array.array('B', open(path, 'rb').read())
    
    ### START Read Data Out to Header ###
    outfile = open(filename, 'w')
    
    text = u'#ifndef {}\n#define {}\n\nstatic const unsigned char {}[] = {}\n'.format(hname_upper, hname_upper, hname, u'{')
    
    current = 0
    data_length = len(data)
    for byte in data:
        if (current % 12) == 0:
            text += '    '
        text += u'0x%02x' % byte
    
        if (current + 1) < data_length:
            text += ', '
        if (current % 12) == 11:
            text += u'\n'
        
        current += 1
    
    text += u'\n};\n\n#endif\n'
    
    outfile.write(text)
    outfile.close()
    ### END Read Data Out to Header ###
    
    return 0


if __name__ == '__main__':
    if len(sys.argv) < 2:
        PrintUsage()
        sys.exit(1)
    
    Bin2Header(sys.argv)