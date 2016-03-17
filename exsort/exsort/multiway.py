import exsort.exsortio as io
import heapq

class Ring:
    def __init__(self, streams):
        self.streams = streams
        self.i = 0
        self.runs_no = 1
    def index(self, j):
        return (self.i+j)%len(self.streams)
    def new_run(self):
        self.i = (self.i + 1)%len(self.streams)
        self.runs_no += 1
    def write(self, v):
        self.streams[self.i].push(v)
    def peek(self):
        return self.streams[self.i].peek()
    def read(self):
        return self.streams[self.i].next()
# def prep_runs(inputs, outputs):
#     runs = Ring(outputs)
#     p = None
#     while True:
#         v = input.peek()
#         if v == io.EOF: break
#         eor = p and p>v
#         if eor:
#             runs.new_run()
#         else:
#             runs.write(input.next())
#         p = v
#     return runs.runs_no

def merge(inputs, outputs):
    ins = [i for i in inputs]
    next = []
    outs = Ring(outputs)
    heapq.heapify(ins)
    last = None
    while ins or next:
        if not ins:
            ins, next, last = next, ins, None
            outs.new_run()
        input = heapq.heappop(ins)
        v = input.peek()
        if v == io.EOF: continue
        if last and v < last:
            heapq.heappush(next, input)
        elif v != io.EOF:
            last = v
            outs.write(input.next())
            heapq.heappush(ins, input)
    return outs.runs_no
def sort(src, dst, n, openread, openwrite):
    s = openread(src)
    in_names = [io.mktemp() for i in range(n)]
    out_names = [io.mktemp() for i in range(n)]
    ins = [s]
    outs = [openwrite(f) for f in out_names]
    while True:
        c = merge(ins, outs)
        for f in ins: f.close()
        for f in outs: f.close()
        in_names, out_names = out_names, in_names
        ins = [openread(f) for f in in_names]
        if c == 1: break
        outs = [openwrite(f) for f in out_names]
    out = openwrite(dst)
    merge(ins, [out])
    out.close()
    for f in ins: f.close()
    for f in in_names + out_names: io.rm(f)
