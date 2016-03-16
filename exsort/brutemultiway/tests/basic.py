import unittest
import tempfile
import sys
import os
# sys.path.insert(0, '..')

from .. import brutemultiway as multiwayexsort

from ..task10 import Student
from ..task10 import StudentReader as sr
from ..task10 import StudentWriter as sw

def lines2temp(lines):
    f = tempfile.NamedTemporaryFile(delete=False)
    name = f.name
    f.write('\n'.join(lines).encode('utf8'))
    f.close()
    return name

class BasicTest(unittest.TestCase):
    def test_io(self):
        f = tempfile.NamedTemporaryFile()
        studentsstr = ['A 1', 'B 2', 'C 3', 'D 4']
        students = [Student(x) for x in studentsstr]
        w = sw(f.name)
        for s in students: w.write(s)
        w.close()
        r = sr(f.name)
        # print('peeked: %s; type: %s; means %s;'%(r.peek(), type(r.peek()), bool(r.peek())))
        self.assertFalse(r.eof())
        while not r.eof():
            s = r.read()
            self.assertIsNotNone(s)
            self.assertIn(str(s), studentsstr)
            studentsstr.remove(str(s))
        self.assertEqual(len(studentsstr), 0)
        r.close()
        f.close()
    def test_merging(self):
        run1src = ['A 1', 'B 3', 'C 33', 'E 117']
        run2src = ['Y 0', 'V 313', 'W 331', 'X 379']
        expected = ['Y 0', 'A 1', 'B 3', 'C 33',\
                    'E 117', 'V 313', 'W 331', 'X 379']
        run1 = lines2temp(run1src)
        run2 = lines2temp(run2src)
        o = tempfile.NamedTemporaryFile()
        inputs = [sr(run) for run in [run1, run2]]
        w = sw(o.name)
        multiwayexsort.merge(inputs, w)
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
        multiwayexsort.sort(f.name, o.name, 64, sr, sw)
        with open(o.name, mode='r', encoding='utf8') as result:
            self.assertTrue(result.readline().strip() == 'C 1')
            self.assertTrue(result.readline().strip() == 'D 2')
            self.assertTrue(result.readline().strip() == 'A 3')
            self.assertTrue(result.readline().strip() == 'B 4')
            self.assertTrue(not result.readline())
        o.close()
        f.close()
if __name__ == '__main__':
    unittest.main()
