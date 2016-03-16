import argparse
import exsort.natural  as natural
import exsort.exsortio as io
from functools import total_ordering

"""task10
External natural mergesort
--------------------------

Given file consisting of records
about students credits,
produces score-ordered version of file.

File format:
    Records are of form "NAME SCORE" (name and score, separated by space)
    and are separated by '\\n'
"""

@total_ordering
class Student:
    def __init__(self, rawline):
        input = rawline.split(' ')
        # print('splitted: %s'%input)
        name, score = input
        self.name, self.score = name, int(score)
    def __eq__(self, that):
        return type(that) == Student and self.score == that.score
    def __lt__(self, that):
        return self.score < that.score
    def __str__(self):
        return self.name + ' ' + str(self.score)
    def __repr__(self): return "'" + self.__str__() + "'"
class StudentReader(io.InputStream):
    cache   = io.EOF
    def __init__(self, filename):
        self.fname = filename
        self.f = open(filename, mode='r', encoding='utf8')
        # print('self.f.tell() -> %d; name: %s'%(self.f.tell(), filename))
    def peek(self):
        if self.cache: return self.cache
        s = self.f.readline()
        self.cache = s if io.is_sentinel(s) else Student(s)
        return self.cache
    def next(self):
        s, self.cache = self.peek(), io.EOF
        return s
    def close(self): self.f.close()
    def __enter__(self): pass
    def __exit__(self): self.close()
class StudentWriter:
    def __init__(self, filename):
        self.fname = filename
        self.f = open(filename, mode='w', encoding='utf8')
    def push(self, val):
        # print('val: %s'%repr(val))
        self.f.write(str(val))
        self.f.write('\n')
        self.f.flush() # perhaps it's better to buffer than to flush each time?
    def close(self):
        self.f.flush()
        self.f.close()
    def __enter__(self): pass
    def __exit__(self): self.close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--in', dest='infile')
    parser.add_argument('-o', '--out', dest='outfile')
    args   = parser.parse_args()
    natural.sort(args.infile, args.outfile, StudentReader, StudentWriter)
