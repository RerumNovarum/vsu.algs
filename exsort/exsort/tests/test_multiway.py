import unittest
import tempfile
import time
import sys
import os
# sys.path.insert(0, '..')

from exsort import natural, exsortio as io

from exsort.task10 import Student
from exsort.task10 import StudentReader as sr
from exsort.task10 import StudentWriter as sw
from exsort.tests.task10 import multiwaysortfile  as sortfile, test_sort, is_sorted, gen

def wrap_sorting(tmp_no):
    def sort(fname):
        sortfile(fname, tmp_no)
    return sort
class MultiwayTest(unittest.TestCase):
    def test_random(self):
        for tmp_no_deg in range(2, 4):
            for i in range(12, 16):
                in_size = 2**i
                tmp_no  = 2**tmp_no_deg
                print('[I] multiway sort on random input started;'\
                        ' input_size=%d; tmpfiles_no=%d'\
                        %(in_size, tmp_no))
                self.assertTrue(test_sort(wrap_sorting(tmp_no), n=in_size))
if __name__ == '__main__':
    unittest.main()
