from distutils.core import setup, Extension

module1 = Extension('wav_cancel', sources = ['module.c'])

setup (name = 'PackageName',