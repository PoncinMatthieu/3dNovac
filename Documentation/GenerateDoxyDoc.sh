#!/bin/sh
cd Build/ && doxygen doxy.conf && cd ../ && firefox html/index.html &