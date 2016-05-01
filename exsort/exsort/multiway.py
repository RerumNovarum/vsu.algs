import exsort.exsortio as io
import heapq


class Ring:
    def __init__(self, streams, buffsize=None):
        self.streams = streams
        self.i = 0
        self.runs_no = 1
        self.buff = []
        self.buffsize = buffsize

    def index(self, j):
        return (self.i+j) % len(self.streams)

    def new_run(self):
        if len(self.buff) < self.buffsize:
            self.flush()
            self.i = (self.i + 1) % len(self.streams)
            self.runs_no += 1

    def write(self, v):
        heapq.heappush(self.buff, v)
        if len(self.buff) == self.buffsize:
            self.flush()

    def peek(self):
        return self.streams[self.i].peek()

    def read(self):
        return self.streams[self.i].next()

    def flush(self):
        buff = self.buff
        o = self.streams[self.i]
        while buff:
            v = heapq.heappop(buff)
            o.push(v)


def merge(inputs, outputs, buffsize):
    ins = [i for i in inputs]
    next = []
    outs = Ring(outputs, buffsize)
    heapq.heapify(ins)
    last = None
    while ins or next:
        if not ins:
            ins, next, last = next, ins, None
            outs.new_run()
        input = heapq.heappop(ins)
        v = input.peek()
        if v == io.EOF:
            continue
        if last and v < last:
            heapq.heappush(next, input)
        elif v != io.EOF:
            last = v
            outs.write(input.next())
            heapq.heappush(ins, input)
    outs.flush()
    return outs.runs_no


def sort(src, dst, n, openread, openwrite, buffsize=1024):
    s = openread(src)
    in_names = [io.mktemp() for i in range(n)]
    out_names = [io.mktemp() for i in range(n)]
    ins = [s]
    outs = [openwrite(f) for f in out_names]
    while True:
        c = merge(ins, outs, buffsize)
        for f in ins:
            f.close()
        for f in outs:
            f.close()
        in_names, out_names = out_names, in_names
        ins = [openread(f) for f in in_names]
        if c == 1:
            break
        outs = [openwrite(f) for f in out_names]
    out = openwrite(dst)
    merge(ins, [out], buffsize=1)
    out.close()
    for f in ins:
        f.close()
    for f in in_names + out_names:
        io.rm(f)
