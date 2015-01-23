#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import socketserver


class Handler(socketserver.BaseRequestHandler):

    def handle(self):
        while True:
            data = self.request.recv(1)
            if not data:
                break
            elif data[0] == 0x1B:
                while len(data) != 3:
                    data += self.request.recv(1)
            #TODO: to remove and process data
            print(data)


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('usage: %s host port' % sys.argv[0])
        sys.exit(1)
    host = sys.argv[1]
    port = int(sys.argv[2])
    server = socketserver.TCPServer((host, port), Handler)
    server.serve_forever()

