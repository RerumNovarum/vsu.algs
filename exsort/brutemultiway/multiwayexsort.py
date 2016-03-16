import os
import heapq
import tempfile

def merge(runs, output):
    """merge(runs, output)
runs: list of streams (returned by openread()) with sorted runs
output: output stream returned by openwrite()"""
    runs = [ (run.peek(), run) for run in runs ]
    heapq.heapify(runs)
    while runs:
        val, run = heapq.heappop(runs)
        output.write(run.read())
        if not run.eof():
            heapq.heappush(runs, (run.peek(), run))
        else: run.close()
def sort(src, dst, bs, openread, openwrite):
    """exsort(src, dst, bs, openread, openwrite)
src: source file name
dst: output file name
bs:  blocksize
openread:   function
            receives filename
            returns  file-like object
                     that has methods
                        read() returns single comparable value from file and advances seek pointer
                        peek() returns single value, doesn't advance pointer
                        close()
openwrite:  function
            receives filename
            returns file-like object
                    with methods
                        write(val) appends a single value
                        close()"""
    s = openread(src)
    tempfiles = []
    try:
        def mktemp():
            f = tempfile.NamedTemporaryFile(delete=False)
            tempfiles.append(f.name)
            f.close()
            return tempfiles[-1]
        b = []
        runs = []
        while not s.eof():
            while not s.eof() and len(b) < bs:
                i = s.read()
                if i: b.append(i)
            b.sort()
            tmp = mktemp()
            w   = openwrite(tmp)
            for x in b: w.write(x)
            w.close()
            runs.append(tmp)
        runs = [ openread(run) for run in runs ]
        w = openwrite(dst)
        try: merge(runs, w)
        finally: w.close()
    finally:
        s.close()
        for f in tempfiles:
            os.remove(f)
