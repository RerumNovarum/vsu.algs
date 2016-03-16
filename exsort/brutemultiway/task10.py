import argparse
import brutemultiway.multiwayexsort as multiwayexsort

class Student:
    def __init__(self, rawline):
        input = rawline.split(' ')
        # print('splitted: %s'%input)
        name, score = input
        self.name, self.score = name, int(score)
    def __eq__(self, that):
        return self.score == that.score
    def __lt__(self, that):
        return self.score < that.score
    def __str__(self):
        return self.name + ' ' + str(self.score)
    def __repr__(self): return "'" + __str__() + "'"
class StudentReader:
    cache   = None
    def __init__(self, filename):
        self.f = open(filename, mode='r', encoding='utf8')
        # print('self.f.tell() -> %d; name: %s'%(self.f.tell(), filename))
    def peek(self):
        if self.cache: return self.cache
        s = self.f.readline()
        if s:
            self.cache = Student(s)
            return self.cache
    def read(self):
        s, self.cache = self.peek(), None
        if s: return s
    def eof(self):
        return not bool(self.peek())
    def close(self): self.f.close()
    def __enter__(self): pass
    def __exit__(self): self.close()
class StudentWriter:
    def __init__(self, filename):
        self.f = open(filename, mode='w', encoding='utf8')
    def write(self, val):
        # print('val: %s'%val)
        self.f.write(str(val))
        self.f.write('\n')
        self.f.flush()
    def close(self):
        self.f.flush()
        self.f.close()
    def __enter__(self): pass
    def __exit__(self): self.close()
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--infile', dest='infile')
    parser.add_argument('-o', '--outfile', dest='outfile')
    args   = parser.parse_args()
    multiwayexsort.sort(args.infile, args.outfile, 64, StudentReader, StudentWriter)
