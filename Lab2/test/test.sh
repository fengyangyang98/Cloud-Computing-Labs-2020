#!/bin/bash
set -xeuo pipefail
# code start here.

# code end here.
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM EXIT
