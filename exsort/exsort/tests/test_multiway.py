import unittest

from exsort.tests.task10 import multiwaysortfile as sortfile, test_sort


def wrap_sorting(tmp_no):
    def sort(fname):
        sortfile(fname, tmp_no)
    return sort


class MultiwayTest(unittest.TestCase):
    def test_random(self):
        for tmp_no_deg in range(2, 4):
            for i in range(12, 16):
                in_size = 2**i
                tmp_no = 2**tmp_no_deg
                print('[I] multiway sort on random input started;'
                      ' input_size=%d; tmpfiles_no=%d'
                      % (in_size, tmp_no))
                self.assertTrue(test_sort(wrap_sorting(tmp_no), n=in_size))


if __name__ == '__main__':
    unittest.main()
