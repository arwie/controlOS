# SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
# SPDX-License-Identifier: MIT

import web
from shared.utils import import_all_in_package


web.imports.add('studio')


import system.update
import system.backup
import system.remote
import system.network

import diag.log
import diag.issue


import_all_in_package(__file__, __name__)
