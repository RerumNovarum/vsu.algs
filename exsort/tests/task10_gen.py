import random

def random_name(length):
    offset = ord('0')
    radix  = 79
    return ''.join( chr(offset + random.randint(0, radix)) for i in range(length))
def random_name_score():
    return (random_name(10), random.randint(0, 10**4))

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('-o', dest='out', type=argparse.FileType('w'), default='task10.rnd.in')
    parser.add_argument('-n', dest='n', type=int, default=4096, help='number of students')
    args = parser.parse_args()
    for i in range(args.n):
        args.out.write('%s %d\n'%random_name_score())
    args.out.close()
if __name__ == '__main__': main()
