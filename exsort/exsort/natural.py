import exsort.exsortio as io


def prep_runs(src, o1, o2):
    prev = None
    c1, c2 = 0, 0  # number of items written to o_i
    inv = False
    while True:
        v = src.next()
        if v == io.EOF:
            break
        elif prev is None or prev <= v:
            o1.push(v)
            c1 += 1
        else:
            o2.push(v)
            c2 += 1
            o1, o2 = o2, o1
            c1, c2 = c2, c1
            inv = not inv
        prev = v
    return (c2, c1) if inv else (c1, c2)


def merge(t1, t2, o):
    p1, p2 = None, None  # 'p' for 'previous'
    exh1, exh2 = False, False  # 'exh' means 'exhausted run'
    while True:
        v1, v2 = t1.peek(), t2.peek()
        if v1 == io.EOF and v2 == io.EOF:
            break
        exh1 = v1 == io.EOF or (p1 and p1 > v1)
        exh2 = v2 == io.EOF or (p2 and p2 > v2)
        if exh1 and exh2:
            p1, p2 = v1, v2
        elif exh1:
            o.push(t2.next())
            p2 = v2
        elif exh2:
            o.push(t1.next())
            p1 = v1
        elif v1 <= v2:
            o.push(t1.next())
            p1 = v1
        else:
            o.push(t2.next())
            p2 = v2


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
        if c1 == 0 or c2 == 0:
            break
        s = reopenwrite(s, dst)
        t1, t2 = reopenread(t1), reopenread(t2)
        merge(t1, t2, s)
        s = reopenread(s)
        t1, t2 = reopenwrite(t1), reopenwrite(t2)
    for f in [s, t1, t2]:
        f.close()
    for tmp in [t1name, t2name]:
        io.rm(tmp)
