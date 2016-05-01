import random
import time

from exsort import natural, multiway, exsortio as io

from exsort.task10 import StudentReader
from exsort.task10 import StudentReader as sr
from exsort.task10 import StudentWriter as sw


def random_name(length):
    offset = ord('0')
    radix = 79
    return ''.join(
            chr(offset + random.randint(0, radix))
            for i in range(length))


def random_name_score():
    return (random_name(10), random.randint(0, 10**4))


def is_sorted(sr):
    score = None
    i = 0
    while True:
        s = sr.next()
        if s == io.EOF:
            return True
        if score and score > s.score:
            return i
        score = s.score
        i += 1


def naturalsortfile(fname):
    """sorts file, using same file as source and destination"""
    natural.sort(fname, fname, sr, sw)


def multiwaysortfile(fname, tmp_n):
    multiway.sort(fname, fname, tmp_n, sr, sw)


def gen(f, n, close=False):
    if type(f) == str:
        f = open(f, mode='w', encoding='utf8')
        close = True
    for i in range(n):
        f.write('%s %d\n' % random_name_score())
    if close:
        f.close()


def test_sort(sorting, fname=None, n=16384):
        f = fname or io.mktemp()
        if not fname:
            gen(f, n, close=True)
        t = time.time()
        sorting(f)
        t = time.time() - t
        reader = sr(f)
        r = is_sorted(reader)
        reader.close()
        if r is True:
            print(
                    '[+] successfully sorted random'
                    'data with N=%d in %f sec.' % (n, t))
        else:
            print(
                    '[-] sorting on random input failed,'
                    'disorder: line %d' % r)
        io.rm(f)
        return r is True


def main_gen(args):
    gen(args.out, args.n)
    args.out.close()


def main_check(args):
    sr = StudentReader(args.file)
    r = is_sorted(sr)
    if r is True:
        print('[+] file is sorted')
    else:
        print('[-] disorder: line %d' % r)
        exit(1)


def main():
    import argparse
    parser = argparse.ArgumentParser(
            description='Task10'
                        '(students rating/external sorting)'
                        ' testing suite'
            )
    parser.set_defaults(cmd=lambda args: parser.print_help())
    subparsers = parser.add_subparsers()
    parser_gen = subparsers.add_parser('gen', help='generate random test')
    parser_gen.add_argument(
            '-o',
            dest='out',
            type=argparse.FileType('w'),
            default='task10.rnd.in')
    parser_gen.add_argument(
            '-n',
            dest='n',
            type=int,
            default=4096,
            help='number of students to generate')
    parser_gen.set_defaults(cmd=main_gen)

    parser_check = subparsers.add_parser(
            'check',
            help='check if file is sorted')
    parser_check.add_argument('file')
    parser_check.set_defaults(cmd=main_check)

    args = parser.parse_args()
    args.cmd(args)


if __name__ == '__main__':
    main()
