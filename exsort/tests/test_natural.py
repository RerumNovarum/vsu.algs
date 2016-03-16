import unittest
import tempfile
import sys
import os
# sys.path.insert(0, '..')

from exsort import natural, exsortio

from exsort.task10 import Student
from exsort.task10 import StudentReader as sr
from exsort.task10 import StudentWriter as sw

def lines2temp(lines):
    f = tempfile.NamedTemporaryFile(delete=False)
    name = f.name
    f.write('\n'.join(lines).encode('utf8'))
    f.close()
    return name

class NaturalTest(unittest.TestCase):
    def test_merging(self):
        run1src = ['A 1', 'B 3', 'C 33', 'E 117']
        run2src = ['Y 0', 'V 313', 'W 331', 'X 379']
        expected = ['Y 0', 'A 1', 'B 3', 'C 33',\
                    'E 117', 'V 313', 'W 331', 'X 379']
        run1 = lines2temp(run1src)
        run2 = lines2temp(run2src)
        o = tempfile.NamedTemporaryFile()
        sr1, sr2 = sr(run1), sr(run2)
        w = sw(o.name)
        natural.merge(sr1, sr2, w)
        with open(o.name, mode='r', encoding='utf8') as r:
            for line in expected:
                self.assertEqual(line, r.readline().strip())
            self.assertEqual(r.readline(), '')
        w.close()
        o.close()
        os.remove(run1)
        os.remove(run2)
    def test_sorting_small(self):
        f = tempfile.NamedTemporaryFile()
        o = tempfile.NamedTemporaryFile()
        f.write('A 3\nC 1\nB 4\nD 2\n'.encode('utf8'))
        f.flush()
        # print('f.name -> %s'%f.name)
        # print('o.name -> %s'%o.name)
        # print('o.tell() -> %d'%o.tell())
        natural.sort(f.name, o.name, sr, sw)
        expected = ['C 1', 'D 2', 'A 3', 'B 4']
        with open(o.name, mode='r', encoding='utf8') as result:
            for e in expected:
                r = result.readline().rstrip()
                print('[I] expected=%s output=%s'%(repr(e), repr(r)))
                self.assertTrue(r == e)
            self.assertTrue(not result.readline())
        o.close()
        f.close()
if __name__ == '__main__':
    unittest.main()
