from distutils.core import setup, Extension

module1 = Extension('wav_cancel', sources = ['module.c'])

setup (name = 'PackageName',        version = '1.0',        description = 'This is a demo package',        ext_modules = [module1])