from distutils.core import setup, Extension

ext = Extension('mktime', 
    sources=['mktime.c'],
    extra_compile_args=['-O3', '-std=c99', '-Ofast'],
    extra_link_args=["-s"],
    language="c",
)

setup(
    name='mktime', 
    author='S-YOU', author_email='S-YOU@users.noreply.github.com', 
    version='0.0.1', 
    url = 'https://github.com/S-YOU/mktime',
    download_url = 'https://github.com/S-YOU/mktime/tarball/master',
    ext_modules=[ext]
)
