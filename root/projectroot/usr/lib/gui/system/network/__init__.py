# SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
# SPDX-License-Identifier: MIT

from shared.utils import import_all_in_package


from . import status
from . import syswlan
from . import lan
from . import wlan
from . import smtp


import_all_in_package(__file__, __name__)
