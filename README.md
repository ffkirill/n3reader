n3reader
========
2013-10-12.
Started to work on Alti2 Neptune 3 protocol implementation.
Coded from Alexey Lobanov reverse engineered Altimaster N-series
communication protocol specification
(http://sites.google.com/site/lobanovsoftware/).

usage: $n3reader --port=<port_name> <command>

Currently two commands are supported:
  get_device_info - Read device model, s/n, s/w and h/w versions
  read_jumps_summary - Reads jumps summary data
