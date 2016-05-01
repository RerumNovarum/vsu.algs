import tempfile
import os

from functools import total_ordering

EOF = ''


@total_ordering
class InputStream:
    name = None
    cache = EOF
    prev = EOF

    def __init__(self, name):
        self.filename = name

    def next(self):
        """next()
returns current object from stream and advances current position
        or None if reached sentinel or end of stream"""
        s, self.cache = self.peek(), EOF
        self.prev = s
        return s

    def peek(self):
        """peek()
returns current object but doesn't advance position
        None if met sentinel or EOF"""
        raise NotImplemented()

    def close(self):
        """close()"""
        raise NotImplemented()

    def __lt__(self, that):
        v1, v2 = self.peek(), that.peek()
        if v1 == EOF:
            return False
        if v2 == EOF:
            return True
        return v1 < v2

    def eor(self):
        p, v = self.prev, self.peek()
        eor = self.eof() or (bool(p) and v < p)
        if eor and p and v:
            print("v='%s' p='%s' v<p -> %s" % (v, p, v < p))
        return eor

    def eof(self):
        return is_sentinel(self.peek())

    def __str__(self):
        return ' v="%s" ' % (self.peek())

    def __repr__(self): return "'%s'" % str(self)


class OutputStream:
    def __init__(self, name):
        self.filename = name

    def push(self, v):
        """push(v)
append's `v` to the stream"""
        raise NotImplemented()

    def flush(self):
        raise NotImplemented()

    def close(self):
        """close()"""
        raise NotImplemented()


def is_sentinel(s):
    return s in [None, '\n', EOF]


def mktemp():
    tmp = tempfile.NamedTemporaryFile(delete=False)
    name = tmp.name
    tmp.close()
    return name


def rm(filename):
    os.remove(filename)
