#!/usr/bin/env python
import os
import subprocess
import sys

out = subprocess.check_output(['find . -name \"*txt\"'], shell=True)

stats = out.split('\n')

import hashlib
stats_md5 = [(fname, hashlib.md5(open(fname, 'rb').read()).digest())
             for fname in stats if fname]

for i in stats_md5:
    for j in stats_md5:
        if i[0]==j[0]: continue
        if i[1]==j[1]:
            namei = i[0].split('/')[1]
            namej = j[0].split('/')[1]
            if namei != namej:
                print "Match!", i[0], j[0]
