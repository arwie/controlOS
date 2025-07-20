# SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
# SPDX-License-Identifier: MIT

import web
from shared.utils import import_all_in_package


web.document.imports.append('bootstrap')
web.document.stylesheets.append('bootstrap/bootstrap.css')

web.document.stylesheets.append('fontawesome/css/all.css')


web.document.imports.append('studio')


import system.update
import system.backup
import system.remote
import system.network

import diag.log
import diag.issue


import_all_in_package(__file__, __name__)
