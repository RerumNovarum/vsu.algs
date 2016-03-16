import tempfile
import os

EOR='\n'
EOF=''

class InputStream:
    def __init__(self, name):
        self.filename = name
    def next(self):
        """next()
returns current object from stream and advances current position
        or None if reached sentinel or end of stream"""
        raise NotImplemented()
    def peek(self):
        """peek()
returns current object but doesn't advance position
        None if met sentinel or EOF"""
        raise NotImplemented()
    def close(self):
        """close()"""
        raise NotImplemented()

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
    return s in [EOR, EOF]

def mktemp():
    tmp  = tempfile.NamedTemporaryFile(delete=False)
    name = tmp.name
    tmp.close()
    return name

def rm(filename):
    os.remove(filename)
