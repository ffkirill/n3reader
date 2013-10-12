n3reader
========
2013-10-12.<br/>
Started to work on Alti2 Neptune 3 protocol implementation.
Coded from Alexey Lobanov reverse engineered Altimaster N-series
communication protocol specification
(http://sites.google.com/site/lobanovsoftware/).

usage: $n3reader --port=&lt;port_name&gt; &lt;command&gt;

Currently two, commands are supported:<br/>
  -get_device_info - Read device model, s/n, s/w and h/w versions<br/>
  -read_jumps_summary - Reads jumps summary data
