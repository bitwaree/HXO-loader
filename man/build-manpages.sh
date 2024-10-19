#!/bin/bash

pandoc -s -t man -o hxo-loader.1 hxo-loader.man.md
pandoc -s -t man -o hxo-devl.3 hxo-devl.man.md
pandoc -s -t man -o hxo_cmpversion.3 hxo_cmpversion.man.md
pandoc -s -t man -o hxo_createthread.3 hxo_createthread.man.md
