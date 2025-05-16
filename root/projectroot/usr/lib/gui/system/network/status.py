# SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
# SPDX-License-Identifier: MIT

from asyncio import to_thread
import web
from shared import network


web.imports.add('system/network/status')


@web.handler
class info(web.RequestHandler):
	async def get(self):
		network_status = await to_thread(network.status)
		self.write(network_status.encode())
