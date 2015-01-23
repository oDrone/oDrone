#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import termios
import sys
import contextlib
import socket


@contextlib.contextmanager
def raw(f):
    old_attr = termios.tcgetattr(sys.stdin)
    new_attr = list(old_attr)
    new_attr[3] = new_attr[3] & ~(termios.ECHO | termios.ICANON)
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, new_attr)
    yield f
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_attr)

def start(host, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((host, port))
        with raw(sys.stdin) as stdin:
            try:
                while True:
                    c = stdin.read(1)
                    sock.send(c.encode())
            except KeyboardInterrupt:
                pass

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('usage: %s host port' % sys.argv[0])
        sys.exit(1)
    host = sys.argv[1]
    port = int(sys.argv[2])
    start(host, port)

