# SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
# SPDX-License-Identifier: MIT

import sys
from importlib import import_module
import socket
import asyncio
import web

import shared


import_module(sys.argv[-1])


async def main():
	asyncio.get_running_loop().set_task_factory(asyncio.eager_task_factory)

	systemd_socket = socket.fromfd(3, socket.AF_INET6, socket.SOCK_STREAM)
	systemd_socket.setblocking(False)
	
	server = web.server()
	server.add_socket(systemd_socket)

	await asyncio.Event().wait()


asyncio.run(main())
