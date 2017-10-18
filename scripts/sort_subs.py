#!/usr/bin/env python
import os
import subprocess

class cd:
    """Context manager for changing the current working directory"""
    def __init__(self, newPath):
        self.newPath = os.path.expanduser(newPath)

    def __enter__(self):
        self.savedPath = os.getcwd()
        os.chdir(self.newPath)

    def __exit__(self, etype, value, traceback):
        os.chdir(self.savedPath)

def tryinflate(dir, name):
    if not name.endswith('gz'): return

    with cd(dir):
        subprocess.call(["tar", "xzf", name])

for i in os.listdir('.'):
    if os.path.isdir(i): continue
    name = i.split('_')[0]
    if not os.path.isdir(name):
        os.mkdir(name)
    os.rename(i, name+'/' + i)

    tryinflate(name, i)
