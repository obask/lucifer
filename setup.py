from distutils.core import setup, Extension

module1 = Extension('lucifer',
                    sources = ['lucifer_module.c', 'lucifer_core.cpp',
                    		   'inverted_index.cpp', 'common.cpp',
                    		   'porter_stemmer.c'])

setup (name = 'PackageName',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])


