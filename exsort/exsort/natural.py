import exsort.exsortio as io

def prep_runs(src, o1, o2):
    prev = io.EOR
    c1, c2 = 0, 0
    inv = False
    while True:
        v = src.next()
        if   io.is_sentinel(v): break
        elif io.is_sentinel(prev) or prev <= v:
            o1.push(v)
            c1 += 1
        else:
            o1.push(io.EOR)
            o2.push(v)
            c2 += 1
            o1, o2 = o2, o1
            c1, c2 = c2, c1
            inv = not inv
        prev = v
    return (c2, c1) if inv else (c1, c2)
def merge(t1, t2, o):
    while True:
        v1, v2 = t1.peek(), t2.peek()
        if v1 == io.EOF and v2 == io.EOF:
            break
        elif io.is_sentinel(v1) and io.is_sentinel(v2):
            t1.next()
            t2.next()
        elif io.is_sentinel(v1): o.push(t2.next())
        elif io.is_sentinel(v2): o.push(t1.next())
        elif v1 <= v2: o.push(t1.next())
        else:
#            assert v1 > v2
            o.push(t2.next())
def sort(src, dst,  openread, openwrite):
    def reopenwrite(f, fname=None):
        fname = fname or f.fname
        f.close()
        return openwrite(fname)
    def reopenread(f, fname=None):
        fname = fname or f.fname
        f.close()
        return openread(fname)
    s = openread(src)
    t1name, t2name = io.mktemp(), io.mktemp()
    t1, t2 = openwrite(t1name), openwrite(t2name)
    while True:
        c1, c2 = prep_runs(s, t1, t2)
        if c1 == 0 or c2 == 0: break
        s = reopenwrite(s, dst)
        t1, t2 = reopenread(t1), reopenread(t2)
        merge(t1, t2, s)
        s = reopenread(s)
        t1, t2 = reopenwrite(t1), reopenwrite(t2)
    for f in [s, t1, t2]: f.close()
    for tmp in [t1name, t2name]:
        io.rm(tmp)
