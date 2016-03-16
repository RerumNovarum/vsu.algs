import tempfile

import exsortio as io

def sort(src, dst, tempfiles_half_no, openread, openwrite):
    N = tempfiles_half_no
